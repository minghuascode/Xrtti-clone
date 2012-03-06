/*****************************************************************************\
 *                                                                           *
 * GeneratorMethod.cpp                                                       *
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


GeneratorMethod::GeneratorMethod(Generator &generator, const Method &method)
    : GeneratorMember(generator, (const Member &) method), methodM(method),
      methodSignatureM(generator, method.GetSignature())
{
}


bool GeneratorMethod::CanInvoke()
{
    // Only non-anonymous, accessible context constructors can be emitted
    const Structure &structure = 
        (const Structure &) (methodM.GetContext());

    if (structure.IsAnonymous() || 
        (((structure.GetType() == Context::Type_Class) ||
          (structure.GetType() == Context::Type_Struct)) &&
         ((const Struct &) structure).IsAbstract())) {
        return false;
    }

    // Also, the method signature must be 

    return true;
}


void GeneratorMethod::EmitTypedefs(FILE *file)
{
    if (!Generator::IsAccessible(&methodM) || !this->CanInvoke()) {
        return;
    }

    methodSignatureM.EmitTypedefs(file);
}


void GeneratorMethod::EmitXrttiAccess(FILE *file)
{
    if (!Generator::IsAccessible(&methodM) || !this->CanInvoke()) {
        return;
    }

	// The "(void) xxx" text is to avoid compiler warnings about unused
	// variables in some cases
    fprintf(file, "    static void _%lu_invoke(void *pThis, void *pRet, "
            "void **pArgs)\n    {\n        (void) pThis, (void) pRet, "
			"(void) pArgs;\n        ", (unsigned long) this->GetNumber());

    const Type &returnType = methodM.GetSignature().GetReturnType();
    if ((returnType.GetBaseType() != Type::BaseType_Void) ||
        returnType.GetArrayOrPointerCount()) {
        bool refReturned = methodM.GetSignature().GetReturnType().IsReference();
        const char *constMod = 
            ((returnType.IsConst() && refReturned) ? "const " : "");
        fprintf(file, "* (%s_%lu_type *%s) pRet = %s", constMod,
                (unsigned long) methodSignatureM.GetReturnTypeNumber(),
                refReturned ? "*" : "", refReturned ? "& " : "");
    }

    fprintf(file, "((%s *) pThis)->%s%s(", 
            Generator::GetTypeName(&(methodM.GetContext())).c_str(),
            methodM.IsOperatorMethod() ? "operator " : "", methodM.GetName());

    methodSignatureM.EmitInvokeArguments(file);
    
    fprintf(file, ");\n    }\n\n");
}


void GeneratorMethod::EmitExtraDeclarations(FILE *file)
{
    methodSignatureM.EmitDeclaration(file);
}


void GeneratorMethod::EmitExtraDefinitions(FILE *file)
{
    methodSignatureM.EmitDefinition(file);

    u32 count = methodSignatureM.GetArgumentCount();
    if (count) {
        fprintf(file, "    static const char *_%lu_names[] =\n    {\n", 
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        \"%s\"%s\n", methodM.GetArgumentName(i),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "\n    };\n\n");
    }
}


void GeneratorMethod::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorMember::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    Generator::EmitBooleanArgument(file, methodM.IsOperatorMethod(), true);

    Generator::EmitBooleanArgument(file, methodM.IsConst(), true);

    Generator::EmitBooleanArgument(file, methodM.IsVirtual(), true);

    Generator::EmitBooleanArgument(file, methodM.IsPureVirtual(), true);

    fprintf(file, "        &_::_%lu,\n", 
            (unsigned long) methodSignatureM.GetNumber());

    if (methodSignatureM.GetArgumentCount()) {
        fprintf(file, "        _%lu_names,\n", 
                (unsigned long) this->GetNumber());
    }
    else {
        fprintf(file, "        0,\n");
    }

    if (Generator::IsAccessible(&methodM) && this->CanInvoke()) {
        fprintf(file, "        &XrttiAccess::_%lu_invoke", 
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitU32Argument(file, 0, false);
    }
}


}; // namespace Xrtti
