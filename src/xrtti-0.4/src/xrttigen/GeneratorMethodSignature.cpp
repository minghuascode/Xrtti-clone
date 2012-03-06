/*****************************************************************************\
 *                                                                           *
 * GeneratorMethodSignature.cpp                                              *
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


GeneratorMethodSignature::GeneratorMethodSignature(Generator &generator,
                                                   const MethodSignature &sig)
    : GeneratorConstructorSignature(generator, 
                                    (const ConstructorSignature &) sig),
      methodSignatureM(sig)
{
    pReturnTypeM = generator.GetGeneratorType(&sig.GetReturnType());
}


u32 GeneratorMethodSignature::GetReturnTypeNumber()
{
    return pReturnTypeM->GetNumber();
}


bool GeneratorMethodSignature::CanEmitReturnTypeAndArguments()
{
    if (!pReturnTypeM->CanEmitTypedef()) {
        return false;
    }

    return this->CanEmitArguments();
}


void GeneratorMethodSignature::EmitTypedefs(FILE *file)
{
    this->GeneratorConstructorSignature::EmitTypedefs(file);

    pReturnTypeM->EmitTypedef(file);
}


void GeneratorMethodSignature::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorConstructorSignature::EmitDefinitionArguments(file);
    fprintf(file, ",\n");

    fprintf(file, "        &_%lu", (unsigned long) pReturnTypeM->GetNumber());
}


}; // namespace Xrtti
