/*****************************************************************************\
 *                                                                           *
 * GeneratorObject.cpp                                                       *
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


void GeneratorObject::EmitDeclaration(FILE *file)
{
    fprintf(file, "    static %s _%lu;\n", this->GetCompiledTypeName(),
            (unsigned long) numberM);

    this->EmitExtraDeclarations(file);
}


void GeneratorObject::EmitDefinition(FILE *file)
{
    // Emit the extra definitions first since this includes the arrays
    // which must be declared before they are used
    this->EmitExtraDefinitions(file);

    fprintf(file, "    %s _::_%lu\n    (\n", this->GetCompiledTypeName(),
            (unsigned long) numberM);

    this->EmitDefinitionArguments(file);

    fprintf(file, "\n    );\n\n");
}


}; // namespace Xrtti
