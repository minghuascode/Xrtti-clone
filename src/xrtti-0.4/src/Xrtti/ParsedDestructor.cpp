/*****************************************************************************\
 *                                                                           *
 * ParsedDestructor.cpp                                                      *
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


bool ParsedDestructor::Initialize(ParsedContextSet &set, Parser &parser,
                                  Context *pContext,
                                  Parser::Element *pElement, string &error)
{
    superM.Initialize(set, pContext, false, pElement);

    // bool isVirtualM
    isVirtualM = (pElement->GetAttributeValue("virtual") == "1");
    
    // bool isPureVirtualM
    isPureVirtualM = (pElement->GetAttributeValue("pure_virtual") == "1");
    
    // DestructorSignature signatureM;
    if (!signatureM.Initialize(set, parser, pElement, error)) {
        return false;
    }

    return true;
}


void ParsedDestructor::MergeContents(ParsedContextSet &to,
                                     ParsedContextSet &from)
{
    superM.MergeContents(to, from);

    signatureM.MergeContents(to, from);
}


}; // namespace Xrtti
