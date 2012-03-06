/*****************************************************************************\
 *                                                                           *
 * GeneratorEnumeration.cpp                                                  *
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


GeneratorEnumeration::GeneratorEnumeration(Generator &generator, 
                                           const Enumeration &enumeration)
    : enumerationM(enumeration)
{
    generator.RegisterGeneratorEnumeration(&enumeration, this);

    pContextM = generator.GetGeneratorContext(&(enumeration.GetContext()));

    u32 count = enumerationM.GetValueCount();
    if (count) {
        vValuesM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vValuesM[i] = new GeneratorEnumerationValue
                (enumerationM.GetValue(i));
        }
    }
}


GeneratorEnumeration::~GeneratorEnumeration()
{
	for (u32 i = 0; i < vValuesM.size(); i++) {
		delete vValuesM[i];
	}
}


void GeneratorEnumeration::EmitExtraDeclarations(FILE *file)
{
    u32 count = enumerationM.GetValueCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vValuesM[i]->EmitDeclaration(file);
        }
    }
}


void GeneratorEnumeration::EmitExtraDefinitions(FILE *file)
{
    u32 count = enumerationM.GetValueCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vValuesM[i]->EmitDefinition(file);
        }
        fprintf(file, "    static Xrtti::EnumerationValue *_%lu_values[] ="
                "\n    {\n", (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vValuesM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }
}


void GeneratorEnumeration::EmitDefinitionArguments(FILE *file)
{
    Generator::EmitAccessTypeArgument(file, enumerationM.GetAccessType(), true);

    fprintf(file, "        &_::_%lu,\n", 
            (unsigned long) pContextM->GetNumber());

    Generator::EmitStringArgument(file, enumerationM.GetName(), true);

    Generator::EmitArrayArgument(file, enumerationM.GetValueCount(), 
                                 "values", this->GetNumber(), false);
}


}; // namespace Xrtti
