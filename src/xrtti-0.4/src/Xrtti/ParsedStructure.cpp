/*****************************************************************************\
 *                                                                           *
 * ParsedStructure.cpp                                                       *
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


ParsedStructure::ParsedStructure()
    : isIncompleteM(false), isAnonymousM(false), pDestructorM(0)
{
}


ParsedStructure::~ParsedStructure()
{
	delete pDestructorM;
}

  
bool ParsedStructure::Initialize(ParsedContextSet &set, Parser &parser,
                                 const Parser::Element *pElement, string &error)
{
    // ParsedContext
    if (!superM.Initialize(set, parser, pElement, error)) {
        return false;
    }

    // Get a pointer to this context - this is done because this context
    // actually may not be a ParsedStructure; we cannot use our "this" pointer
    // as a pointer to this context, because we may just be imbedded in
    // another object (ParsedStruct or ParsedUnion)
    Context *pThisContext = set.GetContext
        (parser, pElement->GetAttributeValue("id"), error);

    // isAnonymousM must be done out-of-order: it must be done early, so
    // that it can be initialized before any subclasses end up being
    // initialized (subclasses are initialized when loaded below in this
    // initializer, and isAnonymousM has to be initialized before then)
    // bool isAnonymousM;
    string name = this->GetName();
    if (name[0] == '<') { // <anonymous-xxx>
        isAnonymousM = true;
    }
    else {
        const Context *pContext = this->GetContext();

        if (pContext) {
            switch (pContext->GetType()) {
            case Type_Class:
            case Type_Struct:
            case Type_Union:
                isAnonymousM = ((const Structure *) pContext)->IsAnonymous();
                break;
            default: // Type_Namespace
                isAnonymousM = false;
                break;
            }
        }
        else {
            isAnonymousM = false;
        }
    }

    // Friends
    string befriending = pElement->GetAttributeValue("befriending");
    vector<string> vBefriendings;
    split(befriending.c_str(), vBefriendings);

    u32 count = vBefriendings.size();
    for (u32 i = 0; i < count; i++) {
        Context *pContext = set.GetContext
            (parser, string(vBefriendings[i]), error);
        if (pContext == NULL) {
            return false;
        }
        switch (pContext->GetType()) {
        case Context::Type_Class:
            ((ParsedClass *) pContext)->
                AddFriend((Structure *) pThisContext);
            break;
        case Context::Type_Struct:
            ((ParsedStruct *) pContext)->
                AddFriend((Structure *) pThisContext);
            break;
        default:
            string typeNumber = 
                StringUtils::ToString(StringUtils::Format(L"%lu", pContext->GetType()));
            string indexNumber = StringUtils::ToString(StringUtils::Format(L"%lu", i));
            error = ("Unexpected context type " + typeNumber + " for friend " +
                     indexNumber + " of structure " + 
                     pElement->GetAttributeValue("id"));
            return false;
        }
    }

    // bool isIncompleteM
    isIncompleteM = (pElement->GetAttributeValue("incomplete") == "1");
    if (isIncompleteM) {
        // just in case gccxml tries to be funny and give contents for
        // an incomplete structure, ignore anything more
        accessTypeM = AccessType_Public;
        hasStructureNameM = false;
        return true;
    }

    // bool hasStructureNameM
    hasStructureNameM = (pElement->GetAttributeValue("artificial") == "1");

    // AccessType accessTypeM;
    accessTypeM = get_access_type(pElement);
        
    u32 baseCount = 0;
    count = pElement->GetSubElementCount();
    for (u32 i = 0; i < count; i++) {
        Parser::Element *pSubElement = pElement->GetSubElement(i);
        if (pSubElement->GetElementName() == "Base") {
            baseCount++;
        }
    }

    // vector<ParsedBase> vBasesM;
    if (baseCount) {
        vBasesM.resize(baseCount);

        u32 which = 0;
        for (u32 i = 0; i < count; i++) {
            Parser::Element *pSubElement = pElement->GetSubElement(i);
            if (pSubElement->GetElementName() == "Base") {
                if (!vBasesM[which++].Initialize
                    (set, parser, pSubElement->GetAttributeValue("type"), 
                     get_access_type(pSubElement),
                     pSubElement->GetAttributeValue("virtual") == "1",
                     error)) {
                    return false;
                }
            }
        }
    }

    // Get the members elements
    vector<Parser::Element *> vMemberElements;

    if (!get_member_elements(parser, pElement, vMemberElements, error)) {
        return false;;
    }

    // u32 fieldCountM
    // u32 constructorCountM
    u32 fieldCount = 0, constructorCount = 0;
    count = vMemberElements.size();
    for (u32 i = 0; i < count; i++) {
        Parser::Element *pMemberElement = vMemberElements[i];

        string memberElementName = pMemberElement->GetElementName();

        // If this member is a Field, count it as a field
        if (memberElementName == "Field") {
            fieldCount++;
        }
        // If this member is a Variable, count add it as a static field
        else if (memberElementName == "Variable") {
            // Skip it if it's got no name, which gccxml does for no obvious
            // reason sometimes
            if (pMemberElement->GetAttributeValue("name") == "") {
                continue;
            }
            fieldCount++;
        }
        // Else if it's a Constructor, then count it as a constructor
        else if (memberElementName == "Constructor") {
            constructorCount++;
        }
        // Else ignore it
    }

    // vector<Field> vFieldsM;
    if (fieldCount) {
        vFieldsM.resize(fieldCount);
    }
        
    // Constructor *pConstructorsM;
    if (constructorCount) {
        vConstructorsM.resize(constructorCount);
    }

    // Go over all members, collecting fields, constructors, and the
    // destructor
    u32 fieldIndex = 0, constructorIndex = 0;

    // Destructor *pDestructorM;
    for (u32 i = 0; i < count; i++) {
        Parser::Element *pMemberElement = vMemberElements[i];

        string memberElementName = pMemberElement->GetElementName();

        // If this member is a Field, then initialize it
        if (memberElementName == "Field") {
            if (!vFieldsM[fieldIndex++].Initialize
                (set, parser, pThisContext, false, pMemberElement, error)) {
                return false;;
            }
        }
        // If this member is a Variable, then initialize it as a static
        else if (memberElementName == "Variable") {
            // Skip it if it's got no name, which gccxml does for no obvious
            // reason sometimes
            if (pMemberElement->GetAttributeValue("name") == "") {
                continue;
            }
            if (!vFieldsM[fieldIndex++].Initialize
                (set, parser, pThisContext, true, pMemberElement, error)) {
                return false;;
            }
        }
        // Else if it's a Constructor, then initialize it
        else if (memberElementName == "Constructor") {
            if (!vConstructorsM[constructorIndex++].Initialize
                (set, parser, pThisContext, pMemberElement, error)) {
                return false;;
            }
        }
        // Else if it's a Destructor, then initialize it
        else if (memberElementName == "Destructor") {
            if (pDestructorM) {
                error = ("Duplicate destructor in structure id " +
                         pElement->GetAttributeValue("id"));
                return false;;
            }
            pDestructorM = new ParsedDestructor();
            if (!pDestructorM->Initialize
                (set, parser, pThisContext, pMemberElement, error)) {
                return false;;
            }
        }
        // Otherwise, ignore it, we don't care about it
    }
        
    return true;
}


void ParsedStructure::MergeContents(ParsedContextSet &to, 
                                    ParsedContextSet &from)
{
    superM.MergeContents(to, from);

    u32 count = vBasesM.size();
    for (u32 i = 0; i < count; i++) {
        vBasesM[i].MergeContents(to, from);
    }

    count = vFieldsM.size();
    for (u32 i = 0; i < count; i++) {
        vFieldsM[i].MergeContents(to, from);
    }

    count = vConstructorsM.size();
    for (u32 i = 0; i < count; i++) {
        vConstructorsM[i].MergeContents(to, from);
    }

    if (pDestructorM) {
        pDestructorM->MergeContents(to, from);
    }
}


void ParsedStructure::ReplaceContext(Context *pOld, Context *pNew)
{
    u32 count = vFriendsM.size();
    for (u32 i = 0; i < count; i++) {
        if (vFriendsM[i] == pOld) {
            vFriendsM[i] = (Structure *) pNew;
        }
    }

    count = vFieldsM.size();
    for (u32 i = 0; i < count; i++) {
        vFieldsM[i].ReplaceContext(pOld, pNew);
    }

    count = vConstructorsM.size();
    for (u32 i = 0; i < count; i++) {
        vConstructorsM[i].ReplaceContext(pOld, pNew);
    }
}


const Base &ParsedStructure::GetBase(u32 index) const
{
    return vBasesM[index];
}


const Structure &ParsedStructure::GetFriend(u32 index) const
{
    return *(vFriendsM[index]);
}


const Field &ParsedStructure::GetField(u32 index) const
{
    return vFieldsM[index];
}


const Constructor &ParsedStructure::GetConstructor(u32 index) const
{
    return vConstructorsM[index];
}


const Destructor &ParsedStructure::GetDestructor() const
{
    return *pDestructorM;
}


}; // namespace Xrtti
