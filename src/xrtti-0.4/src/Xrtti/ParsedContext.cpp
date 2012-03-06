/*****************************************************************************\
 *                                                                           *
 * ParsedContext.cpp                                                         *
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


ParsedContext::ParsedContext()
{
}

bool ParsedContext::Initialize(ParsedContextSet &set, Parser &parser, 
                               const Parser::Element *pElement, string &error)
{
    // nameM
    nameM = get_name(pElement, true);

    // pContextM
    if (nameM == "::") {
        pContextM = 0;
    }
    else {
        if (!(pContextM = set.GetContext
              (parser, pElement->GetAttributeValue("context"), error))) {
            error = ("Failed to initialize Context id " + 
                     pElement->GetAttributeValue("id") + ": " + error);
            return false;
        }
    }
    
    // pFullNameM
    if (pContextM && pContextM->GetContext()) {
        fullNameM = pContextM->GetFullName();
        fullNameM += "::";
        fullNameM += nameM;
    }
    else {
        fullNameM = nameM;
    }

    return true;
}

ParsedContext::~ParsedContext()
{
}


void ParsedContext::MergeContents(ParsedContextSet &to, ParsedContextSet &from)
{
    // pContextM
    if (pContextM) {
        pContextM = to.MergeContext(pContextM, from);
    }
}


}; // namespace Xrtti
