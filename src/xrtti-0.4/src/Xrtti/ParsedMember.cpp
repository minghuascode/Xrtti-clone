/*****************************************************************************\
 *                                                                           *
 * ParsedMember.cpp                                                          *
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

#include "private/Parsed.h"

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif

ParsedMember::ParsedMember()
{
}


ParsedMember::~ParsedMember()
{
}


void ParsedMember::Initialize(ParsedContextSet & /* set */, Context *pContext,
                              bool isStatic, Parser::Element *pElement)
{
    // AccessType accessTypeM;
    accessTypeM = get_access_type(pElement);
    
    // Context *pContextM;
    pContextM = pContext;
    
    // const char *pNameM;
    nameM = get_name(pElement, false);
    
    // bool isStaticM;
    isStaticM = isStatic;
}


void ParsedMember::MergeContents(ParsedContextSet &to, ParsedContextSet &from)
{
    pContextM = to.MergeContext(pContextM, from);
}


}; // namespace Xrtti
