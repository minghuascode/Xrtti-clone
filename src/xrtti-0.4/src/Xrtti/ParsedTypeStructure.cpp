/*****************************************************************************\
 *                                                                           *
 * ParsedTypeStructure.cpp                                                   *
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

#include <private/Parsed.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


bool ParsedTypeStructure::Initialize(ParsedContextSet &set, Parser &parser,
                                     Parser::Element *pElementBase,
                                     Parser::Element *pElement, string &error)
{
    if (!superM.Initialize(set, parser, BaseType_Structure,
                           pElementBase, error)) {
        return false;
    }

    // Structure *pStructureM
    Context *pContext;
    if (!(pContext = set.GetContext
          (parser, pElement->GetAttributeValue("id"), error))) {
        return false;
    }

    if ((pContext->GetType() != Context::Type_Class) &&
        (pContext->GetType() != Context::Type_Struct) &&
        (pContext->GetType() != Context::Type_Union)) {
        error = ("Expected structure while processing structure type " + 
                 pElementBase->GetAttributeValue("id"));
        return false;
    }

    pStructureM = (Structure *) pContext;

    return true;
}


bool ParsedTypeStructure::Initialize(Context *pContext, string &error)
{
    superM.Initialize();

    if ((pContext->GetType() != Context::Type_Class) &&
        (pContext->GetType() != Context::Type_Struct) &&
        (pContext->GetType() != Context::Type_Union)) {
        error = ("Expected structure while processing structure type for "
                 "context " + string(pContext->GetFullName()));
        return false;
    }
    
    pStructureM = (Structure *) pContext;

    return true;
}


void ParsedTypeStructure::MergeContents(ParsedContextSet &to,
                                        ParsedContextSet &from)
{
    pStructureM = (Structure *) to.MergeContext(pStructureM, from);
}


void ParsedTypeStructure::ReplaceContext(Context *pOld, Context *pNew)
{
    if (pStructureM == pOld) {
        pStructureM = (Structure *) pNew;
    }
}


}; // namespace Xrtti
