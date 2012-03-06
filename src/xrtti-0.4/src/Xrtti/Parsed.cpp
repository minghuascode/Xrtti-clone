/*****************************************************************************\
 *                                                                           *
 * Parsed.cpp                                                                 *
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
#include <private/StringUtils.h>

using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif

/* static */
void split(const string &wst, vector<string> &v)
{
    const char *p, *p2;
    p = p2 = wst.c_str();
    while (true) {
        // Skip whitespace to new member
        while (*p2 && (*p2 == ' ')) {
            p2++;
        }
        if (!*p2) {
            break;
        }
        // This is the start of the member
        p = p2;
        // Skip to whitespace
        while (*p2 && (*p2 != ' ')) {
            p2++;
        }
        // Now add the id
        v.push_back(string(p, p2 - p));
    }
}


/* static */
string get_name(const Parser::Element *pElement, bool anonymous)
{
    string name = pElement->GetAttributeValue("name");
    const char *u_name;
	static u32 anonymous_number = 1;

    // gccxml apparently has decided to forego names on some contexts now.
    // The name can be recovered from the "demangled" field though.
    if (name == "") {
        string demangled = pElement->GetAttributeValue("demangled");
        if (demangled == "") {
			if (anonymous) {
				return StringUtils::ToString
					(StringUtils::Format(L"<anonymous-%lu>", 
										 anonymous_number++));
			}
			else {
				return "";
			}
        }
        else {
            // Find the last colon in demangled, and use the rest as name
            const char *demangled_u = demangled.c_str();
            const char *end = &(demangled_u[demangled.length() - 1]);
            while ((end > demangled_u) && (*end != ':')) {
                end--;
            }
            if (*end == ':') {
                end++;
            }
            u_name = end;
        }
    }
    else {
        u_name = name.c_str();
    }

    // If it's a gccxml anonymous name, use <anonymous>
    if (anonymous && (u_name[0] == '.')) {
		return StringUtils::ToString
			(StringUtils::Format(L"<anonymous-%lu>", anonymous_number++));
    }
    
    return string(u_name);
}


/* static */
AccessType get_access_type(const Parser::Element *pElement)
{
    string access = pElement->GetAttributeValue("access");

    if (access == "private") {
        return AccessType_Private;
    }
    else if (access == "protected") {
        return AccessType_Protected;
    }
    else {
        return AccessType_Public;
    }
}


/* static */
bool get_member_elements(Parser &parser,const Parser::Element *pElement, 
                         vector<Parser::Element *> &vMembers,
                         string &error)
{
    string memberIds = pElement->GetAttributeValue("members");

    vector<string> vMemberIds;

    split(memberIds, vMemberIds);

    u32 membersCount = vMemberIds.size();

    for (u32 i = 0; i < membersCount; i++) {
        string memberId = vMemberIds[i];
        Parser::Element *pMemberElement = parser.LookupElement(memberId);

        if (!pMemberElement) {
            error = "No such member: " + memberId;
            return false;
        }

        // Skip artifical members
        if (pMemberElement->GetAttributeValue("artificial") == "1") {
            continue;
        }

        vMembers.push_back(pMemberElement);
    }
    
    return true;
}


ContextSet *CreateContextSet()
{
    return new ParsedContextSet();
}


}; // namespace Xrtti
