/*****************************************************************************\
 *                                                                           *
 * CompiledContextSet.cpp                                                    *
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
 * These are private data structure definitions shared between               *
 * xrttigen-generated code and the xrtti library.                            *
 *                                                                           *
 * THIS HEADER FILE IS NOT MEANT TO BE INSPECTED OR USED BY ANY END-USER OF  *
 * THE XRTTI LIBRARY.  IGNORE THE CONTENTS OF THIS FILE.                     *
 *                                                                           *
\*****************************************************************************/

#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <Xrtti/XrttiPrivate.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


static map<string, Context *> htNonAnonymousContextsByNameG;
static map<string, Structure *> htStructuresByTypeinfoG;
static vector<Context *> vContextsG;


/* static */
u32 CompiledContextSet::GetContextCount()
{
    return vContextsG.size();
}

    
/* static */
const Context *CompiledContextSet::GetContext(u32 index)
{
    return vContextsG[index];
}

    
/* static */
const Context *CompiledContextSet::LookupContext(const char *pFullName)
{
    map<string, Context *>::iterator iter =
        htNonAnonymousContextsByNameG.find(pFullName);
    
    if (iter == htNonAnonymousContextsByNameG.end()) {
        return 0;
    }
    
    return iter->second;
}


/* static */
const Structure *
CompiledContextSet::LookupStructure(const std::type_info &typeinfo)
{
    map<string, Structure *>::iterator iter =
        htStructuresByTypeinfoG.find(typeinfo.name());
    if (iter == htStructuresByTypeinfoG.end()) {
        return 0;
    }

    return iter->second;
}


/* static */
void CompiledContextSet::RegisterContext(Context *pContext)
{
    Context *pExisting = 0;

    if ((pContext->GetType() == Context::Type_Namespace) ||
        !((Structure *) pContext)->IsAnonymous()) {
        map<string, Context *>::iterator iter =
            htNonAnonymousContextsByNameG.find(pContext->GetFullName());
        if (iter != htNonAnonymousContextsByNameG.end()) {
            pExisting = iter->second;
            switch (pContext->GetType()) {
            case Context::Type_Class:
                if (*((Class *) pContext) == *pExisting) {
                    return;
                }
                break;
            case Context::Type_Namespace:
                if (*((Namespace *) pContext) == *pExisting) {
                    return;
                }
                break;
            case Context::Type_Struct:
                if (*((Struct *) pContext) == *pExisting) {
                    return;
                }
                break;
            default: // Context::Type_Union
                if (*((Union *) pContext) == *pExisting) {
                    return;
                }
                break;
            }
            // This is bad - mismatch
            fprintf(stderr, "Mismatched Contexts for %s\n", 
                    pContext->GetFullName());
            exit(-1);
        }
        
        htNonAnonymousContextsByNameG[pContext->GetFullName()] = pContext;
    }
        
    if (pContext->GetType() != Context::Type_Namespace) {
        Structure *pStructure = (Structure *) pContext;
        const std::type_info *pTypeInfo = pStructure->GetTypeInfo();
        if (pTypeInfo) {
            htStructuresByTypeinfoG[pTypeInfo->name()] = pStructure;
        }
    }

    vContextsG.push_back(pContext);
}

}; // namespace Xrtti
