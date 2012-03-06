/*****************************************************************************\
 *                                                                           *
 * ParsedField.cpp                                                           *
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


bool ParsedField::Initialize(ParsedContextSet &set, Parser &parser,
                             Context *pContext, bool isStatic, 
                             Parser::Element *pElement, string &error)
{
    superM.Initialize(set, pContext, isStatic, pElement);

    // U32 bitCountM
    string bits = pElement->GetAttributeValue("bits");
    if (bits == "") {
        bitCountM = 0;
    }
    else {
        bitCountM = StringUtils::ToU64(bits);
    }
    
    // Type *pTypeM;
    string type = pElement->GetAttributeValue("type");
    if (type == "") {
        error = "Field " + pElement->GetAttributeValue("id") + " has no type";
        return false;
    }
    
    if (!(pTypeM = set.GetType(parser, type, error))) {
        return false;
    }
    
    return true;
}


const Type &ParsedField::GetType() const
{
    return *pTypeM;
}


void ParsedField::MergeContents(ParsedContextSet &to, ParsedContextSet &from)
{
    superM.MergeContents(to, from);

    pTypeM = to.MergeType(pTypeM, from);
}


void ParsedField::ReplaceContext(Context *pOld, Context *pNew)
{
    switch (pTypeM->GetBaseType()) {
    case Type::BaseType_Function:
        ((ParsedTypeFunction *) (Type *) pTypeM)->
            ReplaceContext(pOld, pNew);
        break;
    case Type::BaseType_Structure:
        ((ParsedTypeStructure *) (Type *) pTypeM)->
            ReplaceContext(pOld, pNew);
        break;
    default:
        break;
    }
}


}; // namespace Xrtti
