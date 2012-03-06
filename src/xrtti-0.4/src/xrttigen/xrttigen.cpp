/*****************************************************************************\
 *                                                                           *
 * xrttigen.cpp                                                              *
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

#include <vector>
#include <string>
#include <Xrtti/Xrtti.h>
#include <private/Configuration.h>
#include <private/Generator.h>

using namespace Xrtti;
using namespace std;


int main(int argc, char **argv)
{
    // Read the configuration from the arguments
    Configuration config(argc, argv);

    // Get the include list and definition list
    u32 includeCount = config.GetIncludeCount();
    const char **pIncludes = new const char * [includeCount];
    for (u32 i = 0; i < includeCount; i++) {
        pIncludes[i] = config.GetInclude(i).c_str();
    }

    u32 definitionCount = config.GetDefinitionCount();
    const char **pDefinitions = new const char * [definitionCount];
    for (u32 i = 0; i < definitionCount; i++) {
        pDefinitions[i] = config.GetDefinition(i).c_str();
    }

    // Create a ContextSet to read in all of the contexts from the
    // header file(s)
    ContextSet *pContextSet = CreateContextSet();

    bool success = true;
    u32 inputCount = config.GetInputCount();
    for (u32 i = 0; i < inputCount; i++) {
        const char *pInput = config.GetInput(i).c_str();
        string tmpFile = config.GetTempFile();
        if (!pContextSet->AddHeader(pInput, includeCount, 
                                    (const char **) pIncludes,
                                    definitionCount, 
                                    (const char **) pDefinitions, 
                                    tmpFile.c_str())) {
            fprintf(stderr, "Failed to process header file: %s\n", pInput);
            fprintf(stderr, pContextSet->GetLastError());
            fprintf(stderr, "\n");
            success = false;
            break;
        }
    }

    delete [] pDefinitions;
    delete [] pIncludes;

    if (!success) {
        delete pContextSet;
        return -1;
    }

    {
        Generator generator(config, *pContextSet);

        if (!generator.Generate()) {
            delete pContextSet;
            return -1;
        }
    }

    delete pContextSet;

    return 0;
}
