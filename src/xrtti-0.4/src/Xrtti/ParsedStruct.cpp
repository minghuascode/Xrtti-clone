/*****************************************************************************\
 *                                                                           *
 * ParsedStruct.cpp                                                          *
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


ParsedStruct::ParsedStruct()
{
}

ParsedStruct::~ParsedStruct()
{
}

bool ParsedStruct::Initialize(ParsedContextSet &set, Parser &parser,
                              const Parser::Element *pElement, string &error)
{
    if (!superM.Initialize(set, parser, pElement, error)) {
        return false;
    }

    // Get a pointer to this context - this is done because this context
    // actually may not be a ParsedStructure; we cannot use our "this" pointer
    // as a pointer to this context, because we may just be imbedded in
    // another object (ParsedStruct or ParsedUnion)
    Context *pThisContext = set.GetContext
        (parser, pElement->GetAttributeValue("id"), error);

    // Get the members elements
    vector<Parser::Element *> vMemberElements;

    if (!get_member_elements(parser, pElement, vMemberElements, error)) {
        return false;
    }

    u32 methodCount = 0;
    u32 count = vMemberElements.size();
    for (u32 i = 0; i < count; i++) {
        Parser::Element *pMemberElement = vMemberElements[i];

        string memberElementName = pMemberElement->GetElementName();

        if ((memberElementName == "Method") ||
            (memberElementName == "OperatorMethod")) {
            methodCount++;
        }
        // Else ignore it
    }

    // vector<Method> vMethodsM;
    if (methodCount) {
        vMethodsM.resize(methodCount);

        // Go over all members, collecting methods
        u32 methodIndex = 0;

        for (u32 i = 0; i < count; i++) {
            Parser::Element *pMemberElement = vMemberElements[i];
                
            string memberElementName = pMemberElement->GetElementName();
                
            // If this member is a Method, then initialize it
            bool isOperatorMethod;
            if (memberElementName == "Method") {
                isOperatorMethod = false;
            }
            else if (memberElementName == "OperatorMethod") {
                isOperatorMethod = true;
            }
            // Otherwise, ignore it, we don't care about it
            else {
                continue;
            }

            if (!vMethodsM[methodIndex++].Initialize
                (set, parser, pThisContext, pMemberElement,
                 isOperatorMethod, error)) {
                return false;
            }
        }
    }

    // bool isAbstractM
    isAbstractM = false;
    for (u32 i = 0; i < methodCount; i++) {
        if (vMethodsM[i].IsPureVirtual()) {
            isAbstractM = true;
            break;
        }
    }

    return true;
}


void ParsedStruct::MergeContents(ParsedContextSet &to, ParsedContextSet &from)
{
    superM.MergeContents(to, from);

    u32 count = vMethodsM.size();
    for (u32 i = 0; i < count; i++) {
        vMethodsM[i].MergeContents(to, from);
    }
}


void ParsedStruct::ReplaceContext(Context *pOld, Context *pNew)
{
    superM.ReplaceContext(pOld, pNew);

    u32 count = vMethodsM.size();
    for (u32 i = 0; i < count; i++) {
        vMethodsM[i].ReplaceContext(pOld, pNew);
    }
}


const Method &ParsedStruct::GetMethod(u32 index) const
{
    return vMethodsM[index];
}


}; // namespace Xrtti
