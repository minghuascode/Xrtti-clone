/*****************************************************************************\
 *                                                                           *
 * ParsedConstructorSignature.cpp                                            *
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


ParsedConstructorSignature::ParsedConstructorSignature()
{
}

ParsedConstructorSignature::~ParsedConstructorSignature()
{
}

bool ParsedConstructorSignature::Initialize(ParsedContextSet &set,
                                            Parser &parser,
                                            Parser::Element *pElement,
                                            string &error)
{
    if (!superM.Initialize(set, parser, pElement, error)) {
        return false;
    }

    u32 count = pElement->GetSubElementCount();

    // u32 argumentCountM;
    u32 argumentCount = 0;
    for (u32 i = 0; i < count; i++) {
        Parser::Element *pSubElement = pElement->GetSubElement(i);
            
        if (pSubElement->GetElementName() == "Argument") {
            argumentCount++;
        }
    }

    // Argument *pArgumentsM;
    // bool hasEllipsisM;
    hasEllipsisM = false;
    if (argumentCount) {
        vArgumentsM.resize(argumentCount);

        u32 argumentIndex = 0;

        for (u32 i = 0; i < count; i++) {
            Parser::Element *pSubElement = pElement->GetSubElement(i);
            if (pSubElement->GetElementName() == "Argument") {
                if (!vArgumentsM[argumentIndex++].Initialize
                    (set, parser, pSubElement, error)) {
                    return false;
                }
            }
            else if (pSubElement->GetElementName() == "Ellipsis") {
                hasEllipsisM = true;
            }
        }
    }

    return true;
}


void ParsedConstructorSignature::MergeContents(ParsedContextSet &to,
                                               ParsedContextSet &from)
{
    superM.MergeContents(to, from);

    u32 count = vArgumentsM.size();
    for (u32 i = 0; i < count; i++) {
        vArgumentsM[i].MergeContents(to, from);
    }
}


void ParsedConstructorSignature::ReplaceContext(Context *pOld, Context *pNew)
{
    u32 count = vArgumentsM.size();
    for (u32 i = 0; i < count; i++) {
        vArgumentsM[i].ReplaceContext(pOld, pNew);
    }
}


}; // namespace Xrtti
