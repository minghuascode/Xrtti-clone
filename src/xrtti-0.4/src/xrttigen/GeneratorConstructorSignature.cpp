/*****************************************************************************\
 *                                                                           *
 * GeneratorConstructorSignature.cpp                                         *
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


GeneratorConstructorSignature::
GeneratorConstructorSignature(Generator &generator,
                              const ConstructorSignature &sig)
    : GeneratorDestructorSignature(generator, 
                                   (const DestructorSignature &) sig),
      constructorSignatureM(sig)
{
    u32 count = sig.GetArgumentCount();
    if (count) {
        vArgumentsM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vArgumentsM[i] = new GeneratorArgument
                (generator, sig.GetArgument(i));
        }
    }
}


GeneratorConstructorSignature::~GeneratorConstructorSignature()
{
	for (u32 i = 0; i < vArgumentsM.size(); i++) {
		delete vArgumentsM[i];
	}
}


bool GeneratorConstructorSignature::CanEmitArguments()
{
    u32 count = constructorSignatureM.GetArgumentCount();

    for (u32 i = 0; i < count; i++) {
        if (!vArgumentsM[i]->CanEmitType()) {
            return false;
        }
    }

    return true;
}


void GeneratorConstructorSignature::EmitInvokeArguments(FILE *file)
{
    u32 count = constructorSignatureM.GetArgumentCount();

    for (u32 i = 0; i < count; i++) {
        // The const-ness of the type was not included in its typedef, because
        // it is not needed in all cases.  But we do need it for arguments.
        const char *constMod = 
            (vArgumentsM[i]->TypeIsConstNonPointer() ? "const " : "");

        if (vArgumentsM[i]->IsPointer()) {
            fprintf(file, "(%s_%lu_type &) pArgs[%lu]%s", constMod,
                    (unsigned long) vArgumentsM[i]->GetTypeNumber(), 
                    (unsigned long) i, (i < (count - 1)) ? ", " : "");
        }
        else {
            fprintf(file, "* (%s_%lu_type *) pArgs[%lu]%s", constMod,
                    (unsigned long) vArgumentsM[i]->GetTypeNumber(), 
                    (unsigned long) i, (i < (count - 1)) ? ", " : "");
        }
    }
}


void GeneratorConstructorSignature::EmitTypedefs(FILE *file)
{
    u32 count = constructorSignatureM.GetArgumentCount();
    for (u32 i = 0; i < count; i++) {
        vArgumentsM[i]->EmitTypedefs(file);
    }
}


void GeneratorConstructorSignature::EmitArgumentTypeList(FILE *file)
{
    u32 count = constructorSignatureM.GetArgumentCount();
    for (u32 i = 0; i < count; i++) {
        fprintf(file, "_%lu_type%s", 
                (unsigned long) vArgumentsM[i]->GetTypeNumber(),
                (i < (count - 1)) ? ", " : "");
    }
}


void GeneratorConstructorSignature::EmitExtraDeclarations(FILE *file)
{
    this->GeneratorDestructorSignature::EmitExtraDeclarations(file);

    u32 count = constructorSignatureM.GetArgumentCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vArgumentsM[i]->EmitDeclaration(file);
        }
    }
}


void GeneratorConstructorSignature::EmitExtraDefinitions(FILE *file)
{
    this->GeneratorDestructorSignature::EmitExtraDefinitions(file);

    u32 count = constructorSignatureM.GetArgumentCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vArgumentsM[i]->EmitDefinition(file);
        }
        fprintf(file, "    static Xrtti::Argument *_%lu_arguments[] ="
                "\n    {\n", (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vArgumentsM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }
}


void GeneratorConstructorSignature::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorDestructorSignature::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    Generator::EmitArrayArgument
        (file, constructorSignatureM.GetArgumentCount(), "arguments", 
         (unsigned long) this->GetNumber(), true);

    Generator::EmitBooleanArgument
        (file, constructorSignatureM.HasEllipsis(), false);
}


}; // namespace Xrtti
