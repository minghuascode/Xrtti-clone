/*****************************************************************************\
 *                                                                           *
 * ParsedEnumeration.cpp                                                     *
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

#include <private/StringUtils.h>
#include <private/Parsed.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


ParsedEnumeration::ParsedEnumeration()
{
}

ParsedEnumeration::~ParsedEnumeration()
{
}

bool ParsedEnumeration::Initialize(ParsedContextSet &set, Parser &parser,
                                   Parser::Element *pElement, string &error)
{
    // AccessType accessTypeM;
    accessTypeM = get_access_type(pElement);

    // Context *pContextM;
    if (!(pContextM = set.GetContext
          (parser, pElement->GetAttributeValue("context"), error))) {
        return false;
    }

    // const char *pNameM;
    nameM = pElement->GetAttributeValue("name");

    u32 valueCount = 0;

    u32 count = pElement->GetSubElementCount();

    for (u32 i = 0; i < count; i++) {
        Parser::Element *pSubElement = pElement->GetSubElement(i);
            
        if (pSubElement->GetElementName() == "EnumValue") {
            valueCount++;
        }
    }

    // vector<ParsedEnumerationValue> *valuesM;
    if (valueCount) {
        valuesM.resize(valueCount);

        count = 0;

        for (u32 i = 0; i < valueCount; i++) {
            Parser::Element *pSubElement = pElement->GetSubElement(i);
            
            if (pSubElement->GetElementName() != "EnumValue") {
                continue;
            }

            string name = pSubElement->GetAttributeValue("name");
            if (name == "") {
                string number = StringUtils::ToString(StringUtils::Format(L"%lu", i));
                error = ("Enumeration value number " + number + 
                         " from Enumeration id " + 
                         pElement->GetAttributeValue("id") + " has no name");
                return false;
            }

            string value = pSubElement->GetAttributeValue("init");
            if (value == "") {
                string number = StringUtils::ToString(StringUtils::Format(L"%lu", i));
                error = ("Enumeration value number " + number + 
                         " from Enumeration id " + 
                         pElement->GetAttributeValue("id") + " has no value");
                return false;
            }
            
            valuesM[count++].Initialize(set, name, (s32) StringUtils::ToS64(value));
        }
    }

    return true;
}


void ParsedEnumeration::MergeContents(ParsedContextSet &to,
                                      ParsedContextSet &from)
{
    pContextM = to.MergeContext(pContextM, from);
}


}; // namespace Xrtti
