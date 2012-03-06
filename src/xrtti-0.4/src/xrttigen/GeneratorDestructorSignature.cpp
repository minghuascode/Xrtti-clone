/*****************************************************************************\
 *                                                                           *
 * GeneratorDestructorSignature.cpp                                          *
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


GeneratorDestructorSignature::
GeneratorDestructorSignature(Generator &generator,
                             const DestructorSignature &destructorSignature)
    : destructorSignatureM(destructorSignature)
{
    u32 count = destructorSignature.GetThrowCount();
    if (count) {
        vThrowsM.resize(count);
        for (u32 i = 0; i < count; i++) {
            vThrowsM[i] = generator.GetGeneratorType
                (&destructorSignature.GetThrow(i));
        }
    }
}


GeneratorDestructorSignature::~GeneratorDestructorSignature()
{
}


void GeneratorDestructorSignature::EmitExtraDefinitions(FILE *file)
{
    u32 count = destructorSignatureM.GetThrowCount();
    if (count) {
        fprintf(file, "    static Xrtti::Type *_%lu_throws[] =\n    {\n", 
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vThrowsM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        
        fprintf(file, "    };\n\n");
    }
}


void GeneratorDestructorSignature::EmitDefinitionArguments(FILE *file)
{
    Generator::EmitArrayArgument(file, destructorSignatureM.GetThrowCount(),
                                 "throws", this->GetNumber(), false);
}


}; // namespace Xrtti
