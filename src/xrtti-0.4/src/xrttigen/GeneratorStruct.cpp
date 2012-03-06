/*****************************************************************************\
 *                                                                           *
 * Generator.cpp                                                             *
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


GeneratorStruct::GeneratorStruct(Generator &generator, const Struct &structRef)
    : GeneratorStructure(generator, (const Structure &) structRef),
      structM(structRef)
{
    u32 count = structRef.GetMethodCount();
    if (count) {
        vMethodsM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vMethodsM[i] = new GeneratorMethod
                (generator, structRef.GetMethod(i));
        }
    }
}


GeneratorStruct::~GeneratorStruct()
{
	for (u32 i = 0; i < vMethodsM.size(); i++) {
		delete vMethodsM[i];
	}
}


void GeneratorStruct::EmitXrttiAccess(FILE *file)
{
    this->GeneratorStructure::EmitXrttiAccess(file);

    u32 count = structM.GetMethodCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vMethodsM[i]->EmitXrttiAccess(file);
        }
    }
}


void GeneratorStruct::EmitNeededTypedefs(FILE *file)
{
    this->GeneratorStructure::EmitNeededTypedefs(file);

    u32 count = structM.GetMethodCount();
    for (u32 i = 0; i < count; i++) {
        vMethodsM[i]->EmitTypedefs(file);
    }
}


void GeneratorStruct::EmitExtraDeclarations(FILE *file)
{
    this->GeneratorStructure::EmitExtraDeclarations(file);

    u32 count = structM.GetMethodCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vMethodsM[i]->EmitDeclaration(file);
        }
    }
}


void GeneratorStruct::EmitExtraDefinitions(FILE *file)
{
    this->GeneratorStructure::EmitExtraDefinitions(file);

    u32 count = structM.GetMethodCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vMethodsM[i]->EmitDefinition(file);
        }

        fprintf(file, "    static Xrtti::Method *_%lu_methods[] ="
                "\n    {\n", (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vMethodsM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }

        fprintf(file, "    };\n\n");
    }
}


void GeneratorStruct::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorStructure::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    // isAbstract
    Generator::EmitBooleanArgument(file, structM.IsAbstract(), true);

    // methodCount
    // pMethods
    Generator::EmitArrayArgument
        (file, structM.GetMethodCount(), "methods", this->GetNumber(), false);
}



}; // namespace Xrtti
