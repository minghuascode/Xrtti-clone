/*****************************************************************************\
 *                                                                           *
 * TestParsed.cpp                                                            *
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

#include <stdio.h>
#include <Xrtti/Xrtti.h>
#include "private/Configuration.h"
#include "test/XrttiToCpp.h"


using namespace Xrtti;

int main(int argc, char **argv)
{
    // Read the configuration from the arguments (re-use xrttigen Configuration)
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
        if (!pContextSet->AddHeader(pInput, includeCount, 
                                    (const char **) pIncludes,
                                    definitionCount, 
                                    (const char **) pDefinitions, 
                                    "gccxml.out")) {
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

    // Now output the results
    XrttiToCpp(*pContextSet, stdout);

    return 0;
}
