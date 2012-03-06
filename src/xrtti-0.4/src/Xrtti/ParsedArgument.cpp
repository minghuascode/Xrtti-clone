/*****************************************************************************\
 *                                                                           *
 * ParsedArgument.cpp                                                         *
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


ParsedArgument::ParsedArgument()
{
}


ParsedArgument::~ParsedArgument()
{
}


bool ParsedArgument::Initialize(ParsedContextSet &set, Parser &parser,
                                Parser::Element *pElement, string &error)
{
    // Type *pTypeM;
    if (!(pTypeM = set.GetType
          (parser, pElement->GetAttributeValue("type"), error))) {
        return false;
    }
    
    // bool hasDefaultM;
    hasDefaultM = pElement->HasAttribute("default");
    
    // Value defaultM;
    if (hasDefaultM) {
        // Cop-out: defaultM is just the string itself
        defaultM = pElement->GetAttributeValue("default");

#if 0
        // TEMPORARY - fix for an issue, possibly a bug in gccxml, where the
        // default value is sometimes bogus
        if ((defaultM.find('<', 0) < defaultM.find('"', 0)) ||
            StringUtils::EndsWith(defaultM, "()")) {
            // In the first case, this is a template instantiation.  Because
            // we cannot tell validly-specified templates from
            // invalidly-specified templates here, just ignore any default
            // which is a template instantiation.
            // In the second case, this is a method call, which might be OK
            // sometimes, but in the cases I am having problems with at the
            // moment, is never correct.
            hasDefaultM = false;
        }
#endif
    }
    
    return true;
}


void ParsedArgument::MergeContents(ParsedContextSet &to,
                                   ParsedContextSet &from)
{
    pTypeM = to.MergeType(pTypeM, from);
}


void ParsedArgument::ReplaceContext(Context *pOld, Context *pNew)
{
    switch (pTypeM->GetBaseType()) {
    case Type::BaseType_Function:
        ((ParsedTypeFunction *) pTypeM)->ReplaceContext(pOld, pNew);
        break;
    case Type::BaseType_Structure:
        ((ParsedTypeStructure *) pTypeM)->ReplaceContext(pOld, pNew);
        break;
    default:
        break;
    }
}


}; // namespace Xrtti
