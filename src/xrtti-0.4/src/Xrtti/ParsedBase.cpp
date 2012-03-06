/*****************************************************************************\
 *                                                                           *
 * ParsedBase.cpp                                                            *
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


bool ParsedBase::Initialize(ParsedContextSet &set, Parser &parser,
                            const string &id, AccessType accessType,
                            bool isVirtual, string &error)
{
    // AccessType accessTypeM;
    accessTypeM = accessType;

    // bool isVirtualM;
    isVirtualM = isVirtual;

    // Structure *pStructureM;
    Context *pContext = set.GetContext(parser, id, error);

    if (!pContext) {
        error = ("Failed to get context while initializing base id " + 
                 id + ": " + error);
        return false;
    }

    if ((pContext->GetType() != Context::Type_Namespace) &&
        ((Structure *) pContext)->IsIncomplete()) {
        error = "Incomplete context while initializing base id " + id;
        return false;
    }

    if ((pContext->GetType() != Context::Type_Class) &&
        (pContext->GetType() != Context::Type_Struct)) {
        error = "Base class of id " + id + " is not a class";
        return false;
    }

    pStructureM = (Structure *) pContext;

    return true;
}


void ParsedBase::MergeContents(ParsedContextSet &to, ParsedContextSet &from)
{
    pStructureM = (Structure *) to.MergeContext(pStructureM, from);
}


}; // namespace Xrtti
