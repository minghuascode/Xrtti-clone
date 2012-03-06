/*****************************************************************************\
 *                                                                           *
 * GeneratorField.cpp                                                        *
 *                                                                           *
 * ------------------------------------------------------------------------- *
 * Copyright (C) 2007 Bryan Ischo <bryan@ischo.com>                          *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License Version 2 as published  *
 * by the Free Software Foundation.                                          *
 *                                                                           *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General *
 * Public License for more details.                                          *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to:                                *
 * The Free Software Foundation, Inc.                                        *
 * 51 Franklin Street, Fifth Floor                                           *
 * Boston, MA 02110-1301, USA.                                               *
 * ------------------------------------------------------------------------- *
 *                                                                           *
\*****************************************************************************/


#include <private/Generator.h>


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


GeneratorField::GeneratorField(Generator &generator, const Field &field)
    : GeneratorMember(generator, (const Member &) field), fieldM(field)
{
    pTypeM = generator.GetGeneratorType(&(field.GetType()));
}


void GeneratorField::EmitTypedefs(FILE *file)
{
    if (this->CanEmitGet() && fieldM.IsStatic() && fieldM.GetType().IsConst()) {
        pTypeM->EmitTypedef(file);
    }    
}


void GeneratorField::EmitXrttiAccess(FILE *file)
{
    if (this->CanEmitOffsetof()) {
        const Context &context = fieldM.GetContext();

        fprintf(file, "    static const Xrtti::u32 _%lu_offset = "
                "offsetof(%s, %s);\n\n", (unsigned long) this->GetNumber(), 
                Generator::GetTypeName(&context).c_str(), fieldM.GetName());
    }

    if (this->CanEmitGet()) {
        this->EmitGet(file);
    }
}

void GeneratorField::EmitXrttiAccessStaticDefinitions(FILE *file)
{
    if (this->CanEmitGet() && fieldM.IsStatic() && fieldM.GetType().IsConst()) {
        fprintf(file, "XrttiAccess::_%lu_type XrttiAccess::_%lu_faked = "
                "%s::%s;\n\n", (unsigned long) pTypeM->GetNumber(), 
                (unsigned long) this->GetNumber(), 
                fieldM.GetContext().GetFullName(), fieldM.GetName());
    }
}


void GeneratorField::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorMember::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    fprintf(file, "        &_::_%lu,\n", (unsigned long) pTypeM->GetNumber());

    Generator::EmitU32Argument(file, fieldM.GetBitfieldBitCount(), true);
    
    if (this->CanEmitOffsetof()) {
        Generator::EmitBooleanArgument(file, true, true);
        fprintf(file, "        XrttiAccess::_%lu_offset,\n", 
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitBooleanArgument(file, false, true);
        Generator::EmitU32Argument(file, 0, true);
    }

    // get
    if (this->CanEmitGet()) {
        fprintf(file, "        &XrttiAccess::_%lu_get", 
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitU32Argument(file, 0, false);
    }
}

static bool HasVirtualInheritence(const Structure &structure)
{
    u32 count = structure.GetBaseCount();
    for (u32 i = 0; i < count; i++) {
        const Base &base = structure.GetBase(i);
        if (base.IsVirtual() || HasVirtualInheritence(base.GetStructure())) {
            return true;
        }
    }

    return false;
}

bool GeneratorField::CanEmitOffsetof()
{
    if (!fieldM.GetName()[0]) {
        return false;
    }

    if (fieldM.IsStatic()) {
        return false;
    }

    if (fieldM.GetBitfieldBitCount()) {
        return false;
    }

    if (!Generator::IsAccessible(&fieldM)) {
        return false;
    }

#if 0 // This code should be enabled if we want to enforce that only POD types
      // can have offsetof() applied to them.  According to the C++ standard,
      // offsetof() really can only be applied to POD types.  However, this is
      // extremely limiting and breaks the ability of the serializer library
      // to do anything useful at all, so we relax this requirement.  Really,
      // it doesn't make much sense for offsetof() to *not* work, except for
      // classes which have virtual inheritence, and we test for that.
    const Context &context = fieldM.GetContext();
    
    if ((context.GetType() == Context::Type_Namespace) ||
        !Generator::IsPod(&context)) {
        printf("Field %s cannot because class %s is not POD\n", 
               fieldM.GetName(), context.GetFullName());
    }
    return ((context.GetType() != Context::Type_Namespace) &&
            Generator::IsPod(&context));
#else
    const Context &context = fieldM.GetContext();
    if (context.GetType() == Context::Type_Namespace) {
        return true;
    }
    
    return !HasVirtualInheritence((const Structure &) context);
#endif
}


bool GeneratorField::CanEmitGet()
{
    if (!Generator::IsAccessible(&fieldM)) {
        return false;
    }

    if (!fieldM.GetName()[0]) {
        return false;
    }

    if (fieldM.GetBitfieldBitCount()) {
        return false;
    }

    return true;
}


void GeneratorField::EmitGet(FILE *file)
{
    // First fake the field if it's a static const; this is the only way to
    // have an address to return (even though it won't be the address of
    // the actual static const)
    if (fieldM.IsStatic() && fieldM.GetType().IsConst()) {
        fprintf(file, "    static _%lu_type _%lu_faked;\n\n",
                (unsigned long) pTypeM->GetNumber(), 
                (unsigned long) this->GetNumber()); 
    }

    fprintf(file, "    static void *_%lu_get(void *pThis)\n"
            "    {\n        (void) pThis;\n        ", 
			(unsigned long) this->GetNumber());

    if (fieldM.IsStatic()) {
        if (fieldM.GetType().IsConst()) {
            fprintf(file, "return (void *) & _%lu_faked;\n",
                    (unsigned long) this->GetNumber());
        }
        else {
            fprintf(file, "return (void *) & %s::%s;\n", 
                    fieldM.GetContext().GetFullName(), fieldM.GetName());
        }
    }
    else {
        fprintf(file, "return (void *) & ((%s *) pThis)->%s;\n", 
                Generator::GetTypeName(&(fieldM.GetContext())).c_str(), 
                fieldM.GetName());
    }
    
    fprintf(file, "    }\n\n");
}


}; // namespace Xrtti
