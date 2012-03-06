/*****************************************************************************\
 *                                                                           *
 * ParsedMethod.cpp                                                          *
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


ParsedMethod::ParsedMethod()
{
}

ParsedMethod::~ParsedMethod()
{
}

bool ParsedMethod::Initialize(ParsedContextSet &set, Parser &parser,
                              Context *pContext, Parser::Element *pElement,
                              bool isOperator, string &error)
{
    superM.Initialize(set, pContext, 
                      (pElement->GetAttributeValue("static") == "1"), 
                      pElement);

    // bool isOperatorMethodM, isConstM, isVirtualM, isPureVirtualM;
    isOperatorMethodM = isOperator;
    isConstM = (pElement->GetAttributeValue("const") == "1");
    isVirtualM = (pElement->GetAttributeValue("virtual") == "1");
    isPureVirtualM = (pElement->GetAttributeValue("pure_virtual") == "1");

    // MethodSignature signatureM;
    if (!signatureM.Initialize(set, parser, pElement, error)) {
        return false;
    }

    // vector<string> vArgumentsNameM
    u32 count = signatureM.GetArgumentCount();
    for (u32 i = 0; i < count; i++) {
        Parser::Element *pSubElement = pElement->GetSubElement(i);
        if (pSubElement->GetElementName() == "Argument") {
            vArgumentNamesM.push_back(pSubElement->GetAttributeValue("name"));
        }
    }
        
    return true;
}


void ParsedMethod::MergeContents(ParsedContextSet &to, ParsedContextSet &from)
{
    superM.MergeContents(to, from);

    signatureM.MergeContents(to, from);
}


void ParsedMethod::ReplaceContext(Context *pOld, Context *pNew)
{
    signatureM.ReplaceContext(pOld, pNew);
}


}; // namespace Xrtti
