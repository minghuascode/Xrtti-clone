/*****************************************************************************\
 *                                                                           *
 * GeneratorTypeFunction.cpp                                              *
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


GeneratorTypeFunction::
GeneratorTypeFunction(Generator &generator, const TypeFunction &typeFunction)
    : GeneratorType(generator, (const Type &) typeFunction),
      typeFunctionM(typeFunction), pMethodSignatureM(0)
{
    pMethodSignatureM = new GeneratorMethodSignature
        (generator, typeFunction.GetSignature());
}


GeneratorTypeFunction::~GeneratorTypeFunction()
{
	delete pMethodSignatureM;
}


bool GeneratorTypeFunction::CanEmitTypedef()
{
    return pMethodSignatureM->CanEmitArguments();
}


void GeneratorTypeFunction::EmitTypedefContents(FILE *file)
{
    fprintf(file, "_%lu_type (%s)", 
            (unsigned long) pMethodSignatureM->GetReturnTypeNumber(),
            this->GetTypedefTypeDefinition().c_str());
    fprintf(file, "(");
    pMethodSignatureM->EmitArgumentTypeList(file);
    fprintf(file, ")");
}


void GeneratorTypeFunction::EmitPrerequisiteTypedefs(FILE *file)
{
    pMethodSignatureM->EmitTypedefs(file);
}


string GeneratorTypeFunction::GetTypedefTypeName()
{
    return "";
}


void GeneratorTypeFunction::EmitExtraDeclarations(FILE *file)
{
    this->GeneratorType::EmitExtraDeclarations(file);

    pMethodSignatureM->EmitDeclaration(file);
}


void GeneratorTypeFunction::EmitExtraDefinitions(FILE *file)
{
    this->GeneratorType::EmitExtraDefinitions(file);

    pMethodSignatureM->EmitDefinition(file);
}


void GeneratorTypeFunction::EmitDefinitionArguments(FILE *file)
{
    this->GeneratorType::EmitDefinitionArguments(file);
    fprintf(file, ",\n");
    
    fprintf(file, "        &_::_%lu", 
            (unsigned long) pMethodSignatureM->GetNumber());
}


}; // namespace Xrtti
