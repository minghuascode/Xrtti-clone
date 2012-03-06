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


#include "private/Generator.h"


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


GeneratorContext::GeneratorContext(Generator &generator, const Context &context)
    : typedefsEmittedM(false), contextM(context)
{
    generator.RegisterGeneratorContext(&context, this);

    const Context *pContext = context.GetContext();
    if (pContext) {
        pContextM = generator.GetGeneratorContext(pContext);
    }
    else {
        pContextM = 0;
    }
}


bool GeneratorContext::IsAccessible()
{
    return Generator::IsAccessible(&contextM);
}


std::string GeneratorContext::GetTypeName()
{
    return Generator::GetTypeName(&contextM);
}


void GeneratorContext::EmitTypedefs(FILE *file)
{
    if (typedefsEmittedM) {
        return;
    }

    typedefsEmittedM = true;

    this->EmitNeededTypedefs(file);
}


void GeneratorContext::EmitDefinitionArguments(FILE *file)
{
    Generator::EmitStringArgument(file, contextM.GetName(), true);

    Generator::EmitStringArgument(file, contextM.GetFullName(), true);

    if (pContextM) {
        fprintf(file, "        &_::_%lu", 
                (unsigned long) pContextM->GetNumber());
    }
    else {
        fprintf(file, "        0");
    }
}


}; // namespace Xrtti
