/*****************************************************************************\
 *                                                                           *
 * GeneratorConstructor.cpp                                                  *
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


#include "private/Generator.h"


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


bool GeneratorConstructor::IsAccessible()
{
    return Generator::IsAccessible(&constructorM);
}


bool GeneratorConstructor::CanInvoke()
{
    // Only non-anonymous, accessible context constructors can be emitted
    const Structure &structure = 
        (const Structure &) (constructorM.GetContext());

    if (structure.IsAnonymous() || 
        (((structure.GetType() == Context::Type_Class) ||
          (structure.GetType() == Context::Type_Struct)) &&
         ((const Struct &) structure).IsAbstract())) {
        return false;
    }

    // Check the signature
    return constructorSignatureM.CanEmitArguments();
}


void GeneratorConstructor::EmitTypedefs(FILE *file)
{
    if (!Generator::IsAccessible(&constructorM) || !this->CanInvoke()) {
        return;
    }

    constructorSignatureM.EmitTypedefs(file);
}


void GeneratorConstructor::EmitXrttiAccess(FILE *file)
{
    if (!Generator::IsAccessible(&constructorM) || !this->CanInvoke()) {
        return;
    }

	// The "(void) xxx" text is to avoid compiler warnings about unused
	// variables in some cases
    fprintf(file, "    static void *_%lu_invoke(void **pArgs)\n    {\n"
			"        (void) pArgs;\n        return new %s(",
            (unsigned long) this->GetNumber(), 
			Generator::GetTypeName(&(constructorM.GetContext())).c_str());

    constructorSignatureM.EmitInvokeArguments(file);
    
    fprintf(file, ");\n    }\n\n");
}


void GeneratorConstructor::EmitExtraDeclarations(FILE *file)
{
    constructorSignatureM.EmitDeclaration(file);
}


void GeneratorConstructor::EmitExtraDefinitions(FILE *file)
{
    constructorSignatureM.EmitDefinition(file);

    u32 count = constructorSignatureM.GetArgumentCount();
    if (count) {
        fprintf(file, "    static const char *_%lu_names[] =\n    {\n", 
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        \"%s\"%s\n", constructorM.GetArgumentName(i),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }
}


void GeneratorConstructor::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorMember::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    fprintf(file, "        &_::_%lu,\n", 
            (unsigned long) constructorSignatureM.GetNumber());

    if (constructorSignatureM.GetArgumentCount()) {
        fprintf(file, "        _%lu_names,\n", 
                (unsigned long) this->GetNumber());
    }
    else {
        fprintf(file, "        0,\n");
    }

    if (Generator::IsAccessible(&constructorM) && this->CanInvoke()) {
        fprintf(file, "        &XrttiAccess::_%lu_invoke", 
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitU32Argument(file, 0, false);
    }
}


}; // namespace Xrtti
