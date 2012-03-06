/*****************************************************************************\
 *                                                                           *
 * GeneratorTypeStructure.cpp                                              *
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


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


GeneratorTypeStructure::
GeneratorTypeStructure(Generator &generator, const TypeStructure &typeStructure)
    : GeneratorType(generator, (const Type &) typeStructure),
      typeStructureM(typeStructure)
{
    pStructureM = (GeneratorStructure *) generator.
        GetGeneratorContext(&(typeStructure.GetStructure()));
}


bool GeneratorTypeStructure::CanEmitTypedef()
{
    const Structure &structure = typeStructureM.GetStructure();

    return (Generator::IsAccessible(&structure) && !structure.IsAnonymous());
}


string GeneratorTypeStructure::GetTypedefTypeName()
{
    return Generator::GetTypeName(&(typeStructureM.GetStructure()));
}

void GeneratorTypeStructure::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorType::EmitDefinitionArguments(file);
    fprintf(file, ",\n");
    
    fprintf(file, "        &_::_%lu", (unsigned long) pStructureM->GetNumber());
}


}; // namespace Xrtti
