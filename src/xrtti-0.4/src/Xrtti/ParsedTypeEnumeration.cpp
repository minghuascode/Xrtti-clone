/*****************************************************************************\
 *                                                                           *
 * ParsedTypeEnumeration.cpp                                                 *
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


bool ParsedTypeEnumeration::Initialize(ParsedContextSet &set, Parser &parser,
                                       Parser::Element *pElementBase,
                                       Parser::Element *pElement,
                                       string &error)
{
    if (!superM.Initialize(set, parser, BaseType_Enumeration, 
                           pElementBase, error)) {
        return false;
    }

    if (!(pEnumerationM = set.GetEnumeration
          (parser, pElement->GetAttributeValue("id"), error))) {
        return false;
    }

    return true;
}


void ParsedTypeEnumeration::MergeContents(ParsedContextSet &to,
                                          ParsedContextSet &from)
{
    pEnumerationM = to.MergeEnumeration(pEnumerationM, from);
}


}; // namespace Xrtti
