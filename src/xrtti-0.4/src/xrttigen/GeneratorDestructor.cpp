/*****************************************************************************\
 *                                                                           *
 * GeneratorDestructor.cpp                                                   *
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


bool GeneratorDestructor::IsAccessible()
{
    return Generator::IsAccessible(&destructorM);
}


bool GeneratorDestructor::CanInvoke()
{
    // Only non-anonymous, accessible context constructors can be emitted
    const Structure &structure = 
        (const Structure &) (destructorM.GetContext());

    if (structure.IsAnonymous() || 
        (((structure.GetType() == Context::Type_Class) ||
          (structure.GetType() == Context::Type_Struct)) &&
         ((const Struct &) structure).IsAbstract()) ||
        !this->IsAccessible()) {
        return false;
    }

    return true;
}


void GeneratorDestructor::EmitXrttiAccess(FILE *file)
{
    if (!Generator::IsAccessible(&destructorM) || !this->CanInvoke()) {
        return;
    }

    fprintf(file, "    static void _%lu_invoke(void *pThis)\n    {\n",
            (unsigned long) this->GetNumber());

    fprintf(file, "        delete (%s *) pThis;\n", 
            Generator::GetTypeName(&(destructorM.GetContext())).c_str());

    fprintf(file, "    }\n\n");
}


void GeneratorDestructor::EmitExtraDeclarations(FILE *file)
{
    destructorSignatureM.EmitDeclaration(file);
}


void GeneratorDestructor::EmitExtraDefinitions(FILE *file)
{
    destructorSignatureM.EmitDefinition(file);
}


void GeneratorDestructor::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorMember::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    Generator::EmitBooleanArgument(file, destructorM.IsVirtual(), true);

    Generator::EmitBooleanArgument(file, destructorM.IsPureVirtual(), true);

    fprintf(file, "        &_::_%lu,\n", 
            (unsigned long) destructorSignatureM.GetNumber());

    if (Generator::IsAccessible(&destructorM) && this->CanInvoke()) {
        fprintf(file, "        &XrttiAccess::_%lu_invoke", 
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitU32Argument(file, 0, false);
    }
}


}; // namespace Xrtti
