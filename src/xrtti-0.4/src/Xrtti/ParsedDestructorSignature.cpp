/*****************************************************************************\
 *                                                                           *
 * ParsedDestructorSignature.cpp                                             *
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

#include <string>
#include <vector>
#include <private/Parsed.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


ParsedDestructorSignature::ParsedDestructorSignature()
{
}

ParsedDestructorSignature::~ParsedDestructorSignature()
{
}

bool ParsedDestructorSignature::Initialize(ParsedContextSet &set,
                                           Parser &parser,
                                           Parser::Element *pElement,
                                           string &error)
{
    string throws = pElement->GetAttributeValue("throw");
	
    // u32 throwCountM;
    vector<string> vThrows;
    split(throws.c_str(), vThrows);
    u32 count = vThrows.size();

    for (u32 i = 0; i < count; i++) {
        string _throw = vThrows[i];
                
        // If it's a context, then a type has to be used instead.  The thing
        // is, gccxml may not generate a Type for this and instead just point
        // to a context.  In this case, we have to try to find the type.
        
        // It's either a Type or a Context.  Try to get it as a type.
        Type *pType = set.GetType(parser, _throw, error);
        
        if (!pType) {
            // Not a type ... try to get it as a structure
            Context *pContext = set.GetContext(parser, _throw, error);
            
            if (pContext) {
                // Must convert this context into a type
                pType = set.GetType(pContext, error);
                
                if (!pType) {
                    error = ("No such type while initializing destructor "
                             "signature id " + 
                             pElement->GetAttributeValue("id") + ": " +
                             error);
                    return false;
                }
            }
            else {
                // Not a context either
                error = ("No such context while initializing destructor "
                         "signature id " + 
                         pElement->GetAttributeValue("id") + ": " + error);
                return false;
            }
        }
        
        vThrowsM.push_back(pType);
    }

    return true;
}


void ParsedDestructorSignature::MergeContents(ParsedContextSet &to,
                                              ParsedContextSet &from)
{
    u32 count = vThrowsM.size();

    for (u32 i = 0; i < count; i++) {
        vThrowsM[i] = to.MergeType(vThrowsM[i], from);
    }
}


}; // namespace Xrtti
