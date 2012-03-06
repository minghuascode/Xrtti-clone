/*****************************************************************************\
 *                                                                           *
 * GeneratorMember.cpp                                                       *
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


GeneratorMember::GeneratorMember(Generator &generator, const Member &member)
    : memberM(member)
{
    pContextM = generator.GetGeneratorContext(&(member.GetContext()));
}


bool GeneratorMember::IsAccessible()
{
    return Generator::IsAccessible(&memberM);
}


void GeneratorMember::EmitDefinitionArguments(FILE *file)
{
    Generator::EmitAccessTypeArgument(file, memberM.GetAccessType(), true);

    fprintf(file, "        &_::_%lu,\n", 
            (unsigned long) pContextM->GetNumber());
    
    Generator::EmitStringArgument(file, memberM.GetName(), true);

    Generator::EmitBooleanArgument(file, memberM.IsStatic(), false);
}


}; // namespace Xrtti
