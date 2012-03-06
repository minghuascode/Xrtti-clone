/*****************************************************************************\
 *                                                                           *
 * GeneratorTypeEnumeration.cpp                                              *
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


#include <private/StringUtils.h>
#include <private/Generator.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


GeneratorTypeEnumeration::
GeneratorTypeEnumeration(Generator &generator,
                         const TypeEnumeration &typeEnumeration)
    : GeneratorType(generator, (const Type &) typeEnumeration),
      typeEnumerationM(typeEnumeration)
{
    pEnumerationM = generator.GetGeneratorEnumeration
        (&(typeEnumeration.GetEnumeration()));
}


bool GeneratorTypeEnumeration::CanEmitTypedef()
{
    const Context &context = typeEnumerationM.GetEnumeration().GetContext();
    
    if (context.GetType() == Context::Type_Namespace) {
        return true;
    }

    return !((const Structure &) context).IsAnonymous();
}


string GeneratorTypeEnumeration::GetTypedefTypeName()
{
    const Enumeration &enumeration = typeEnumerationM.GetEnumeration();

    return StringUtils::ToString
        (StringUtils::Format(L"%s::%s", enumeration.GetContext().GetFullName(),
                        enumeration.GetName()));
}


void GeneratorTypeEnumeration::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorType::EmitDefinitionArguments(file);
    fprintf(file, ",\n");
    
    fprintf(file, "        &_::_%lu", 
            (unsigned long) pEnumerationM->GetNumber());
}


}; // namespace Xrtti
