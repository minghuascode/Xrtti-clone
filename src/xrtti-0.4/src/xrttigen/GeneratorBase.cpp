/*****************************************************************************\
 *                                                                           *
 * GeneratorBase.cpp                                                         *
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


GeneratorBase::GeneratorBase(Generator &generator, const Base &base,
                             GeneratorStructure *pSubstructure)
    : baseM(base), pSubstructureM(pSubstructure)
{
    pStructureM = (GeneratorStructure *) 
        generator.GetGeneratorContext(&(base.GetStructure()));
}


void GeneratorBase::EmitXrttiAccess(FILE *file)
{
    if (!this->CanEmitCast()) {
        return;
    }

    fprintf(file, "    static void *_%lu_cast(void *pSubclass)\n    {\n",
            (unsigned long) this->GetNumber());
    
    fprintf(file, "        return (%s *) (%s *) pSubclass;\n",
            Generator::GetTypeName(&(baseM.GetStructure())).c_str(),
            pSubstructureM->GetTypeName().c_str());

    fprintf(file, "    };\n\n");
}


void GeneratorBase::EmitDefinitionArguments(FILE *file)
{
    Generator::EmitAccessTypeArgument(file, baseM.GetAccessType(), true);

    Generator::EmitBooleanArgument(file, baseM.IsVirtual(), true);

    fprintf(file, "        &_::_%lu,\n", 
            (unsigned long) pStructureM->GetNumber());

    if (this->CanEmitCast()) {
        fprintf(file, "        &XrttiAccess::_%lu_cast", 
                (unsigned long) this->GetNumber());
    }
    else {
        Generator::EmitU32Argument(file, 0, false);
    }
}


bool GeneratorBase::CanEmitCast()
{
    return (Generator::IsAccessible(&(baseM.GetStructure())) &&
            pSubstructureM->IsAccessible());
}


}; // namespace Xrtti
