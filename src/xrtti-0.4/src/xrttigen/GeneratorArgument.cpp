/*****************************************************************************\
 *                                                                           *
 * GeneratorArgument.cpp                                                     *
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


GeneratorArgument::GeneratorArgument(Generator &generator, const Argument &arg)
    : argumentM(arg)
{
    pTypeM = generator.GetGeneratorType(&arg.GetType());
}

bool GeneratorArgument::IsPointer()
{
    // Since arguments can never be arrays, we consider any array or pointer
    // count to be a pointer count
    return argumentM.GetType().GetArrayOrPointerCount();
}

bool GeneratorArgument::TypeIsConstNonPointer()
{
    return (argumentM.GetType().IsConst() && 
            !argumentM.GetType().GetArrayOrPointerCount());
}


u32 GeneratorArgument::GetTypeNumber()
{
    return pTypeM->GetNumber();
}


bool GeneratorArgument::CanEmitType()
{
    return pTypeM->CanEmitTypedef();
}


void GeneratorArgument::EmitTypedefs(FILE *file)
{
    pTypeM->EmitTypedef(file);
}


void GeneratorArgument::EmitExtraDefinitions(FILE * /* file */)
{
    if (argumentM.HasDefault()) {
#if 0
        fprintf(file, "    static XrttiAccess::_%lu_type _%lu_default = "
                "%s;\n\n", (unsigned long) pTypeM->GetNumber(), 
                (unsigned long) this->GetNumber(), 
                (const char *) argumentM.GetDefault());
#endif
    }
}


void GeneratorArgument::EmitDefinitionArguments(FILE *file)
{
    fprintf(file, "        &_::_%lu,\n", (unsigned long) pTypeM->GetNumber());

    Generator::EmitBooleanArgument(file, argumentM.HasDefault(), true);

    if (argumentM.HasDefault()) {
        const char *pDefault = argumentM.GetDefault();
        if (pDefault[0] == '"') {
            fprintf(file, "        %s", pDefault);
        }
        else {
            fprintf(file, "        \"%s\"", pDefault);
        }
    }
    else {
        fprintf(file, "        0");
    }
}


}; // namespace Xrtti
