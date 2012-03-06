/*****************************************************************************\
 *                                                                           *
 * TestClasses.cpp                                                           *
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
 * This test stresses the xrttigen program using many complex classes.  It   *
 * doesn't verify that the classes are handled properly, just that they are  *
 * handled at all and nothing crashes.                                       *
 *                                                                           *
\*****************************************************************************/

#include <Xrtti/Xrtti.h>
#include <test/TestClasses.h>

using namespace Xrtti;

int main(int /* argc */, char ** /* argv */)
{
    u32 count = GetContextCount();

    for (u32 i = 0; i < count; i++) {
        const Context *pContext = GetContext(i);

        printf("Registered %s: %s\n", 
               (pContext->GetType() == Context::Type_Class) ? "class" : 
               (pContext->GetType() == Context::Type_Namespace) ? "namespace" : 
               (pContext->GetType() == Context::Type_Struct) ? "struct" : 
               "union", pContext->GetFullName());
                      
        if ((pContext->GetType() == Context::Type_Class) ||
            (pContext->GetType() == Context::Type_Struct) ||
            (pContext->GetType() == Context::Type_Union)) {
            const std::type_info *pTypeInfo = 
                ((Structure *) pContext)->GetTypeInfo();
            if (pTypeInfo != NULL) {
                printf("    type_info name: %s\n", pTypeInfo->name());
            }
        }
    }

    const Structure *pStructure = LookupStructure(typeid(BasicTypes));

    if (pStructure != NULL) {
        printf("Found Structure: %s\n", pStructure->GetFullName());
    }

    return 0;
}
