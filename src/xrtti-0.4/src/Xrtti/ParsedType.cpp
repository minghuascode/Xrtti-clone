/*****************************************************************************\
 *                                                                           *
 * ParsedType.cpp                                                            *
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


// This helper type holds keeps track of Type attributes
typedef struct Attributes
{
    bool is_const;
    bool is_volatile;
} Attributes;

// This helper type holds keeps track of Type arrays
typedef struct ArrayOrPointerProps
{
    bool is_array;

    // array props
    bool is_unbounded;
    u32 max;

    // pointer props
    bool is_const;
    bool is_volatile;
} ArrayOrPointerProps;



ParsedType::ParsedType()
{
}

ParsedType::~ParsedType()
{
	for (u32 i = 0; i < vArrayOrPointersM.size(); i++) {
		delete vArrayOrPointersM[i];
	}
}

bool ParsedType::Initialize(ParsedContextSet & /* set */, Parser &parser,
                            Type::BaseType baseType, Parser::Element *pElement,
                            string &error)
{
    baseTypeM = baseType;

    // bool isReferenceM;
    if (pElement->GetElementName() == "ReferenceType") {
        // Set reference to true
        isReferenceM = true;
        // Get the type that is referenced
        if (!(pElement = parser.LookupElement
              (pElement->GetAttributeValue("type")))) {
            error = ("No such type [" + pElement->GetAttributeValue("type") +
                     "] for reference type " + 
                     pElement->GetAttributeValue("id"));
            return false;
        }
    }
    else {
        isReferenceM = false;
    }

	isConstM = isVolatileM = false;

    // Now loop through the remaining types, accumulating pointer
    // and array info
    vector<ArrayOrPointerProps> vArrayOrPointerProps;
    while (true) {
        string type = pElement->GetElementName();

        // CvQualifiedType simply adds qualifying attributes to the previous
        // type specifier, if there was one
        if (type == "CvQualifiedType") {
			bool isConst = (pElement->GetAttributeValue("const") == "1");
			bool isVolatile = (pElement->GetAttributeValue("volatile") == "1");
			if (vArrayOrPointerProps.size()) {
				ArrayOrPointerProps &props = vArrayOrPointerProps.back();
				props.is_const = isConst;
				props.is_volatile = isVolatile;
			}
			else {
				// bool isConstM;
				isConstM = isConst;
				// bool isVolatileM;
				isVolatileM = isVolatile;
			}
            // Ignore restrict, it is g++-specific
        }
        // PointerType adds to the pointer level vector, with the last
        // used attributes
        else if (type == "PointerType") {
            ArrayOrPointerProps props = { false, false, 0, false, false };
            vArrayOrPointerProps.push_back(props);
        }
        // ArrayType adds another array dimension, with its max
        else if (type == "ArrayType") {
            ArrayOrPointerProps props = { true, false, 0, false, false };
            string max = pElement->GetAttributeValue("max");
            // Strip off the trailing 'u' if it's there
            if (max.length()) {
                if (max.at(max.length() - 1) == 'u') {
                    max = max.substr(0, max.length() - 1);
                }
                if ((max == "ffffffffffffffff") ||
                    (max == "0xffffffffffffffff")) {
                    // This indicates no practical maximumum
                    props.is_unbounded = true;
                }
                else {
                    props.max = StringUtils::ToU64(max);
                }
            }
            else {
                props.max = 0;
            }

            vArrayOrPointerProps.push_back(props);
        }
        // Skip right over typedefs, to the actual type
        else if (type == "Typedef") {
        }
        // Other types finish the type definition
        else {
            break;
        }
        
        // Next sub type
        if (!(pElement = parser.LookupElement
              (pElement->GetAttributeValue("type")))) {
            error = ("Expected a sub type for type id " +
                     pElement->GetAttributeValue("id"));
            return false;
        }
    }

    // u32 arrayOrPointerCountM;
    u32 arrayOrPointerCount = vArrayOrPointerProps.size();

    // Pointer *pPointersM;
    if (arrayOrPointerCount) {
        vArrayOrPointersM.resize(arrayOrPointerCount);
        for (u32 i = 0; i < arrayOrPointerCount; i++) {
            ArrayOrPointerProps props = vArrayOrPointerProps[i];
            if (props.is_array) {
                vArrayOrPointersM[i] = new ParsedArray
                    (props.is_unbounded, props.max + 1);
            }
            else {
                vArrayOrPointersM[i] = new ParsedPointer
                    (props.is_const, props.is_volatile);
            }
        }
    }

    return true;
}


}; // namespace Xrtti
