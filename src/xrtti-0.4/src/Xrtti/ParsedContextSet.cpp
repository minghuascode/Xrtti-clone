/*****************************************************************************\
 *                                                                           *
 * ParsedContextSet.cpp                                                      *
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

#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <private/StringUtils.h>
#include <private/Parsed.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


// **************************************************************************
// static helper functions
// **************************************************************************

static string get_full_name_of(Parser &parser, const string &elementId)
{
    Parser::Element *pElement = parser.LookupElement(elementId);
    
    if (pElement == NULL) {
        return "";
    }

    string name = pElement->GetAttributeValue("name");

    string parentName = get_full_name_of
        (parser, pElement->GetAttributeValue("context"));

    if ((parentName == "") || (parentName == "::")) {
        return name;
    }

    return parentName + "::" + name;
}


// **************************************************************************
// ParsedContextSet implementation
// **************************************************************************

ParsedContextSet::ParsedContextSet()
{
}


ParsedContextSet::~ParsedContextSet()
{
    u32 count = vContextsM.size();
    for (u32 i = 0; i < count; i++) {
        delete vContextsM[i];
    }

    count = vTypesM.size();
    for (u32 i = 0; i < count; i++) {
        delete vTypesM[i];
    }

    count = vEnumerationsM.size();
    for (u32 i = 0; i < count; i++) {
        delete vEnumerationsM[i];
    }
}


bool ParsedContextSet::AddHeader(const char *path, u32 includeCount,
                                 const char **pIncludes, u32 definitionCount,
                                 const char **pDefinitions, const char *tmp)
{
    // This keeps track of whatever error might have occurred
    string error;
        
    // Reset the current error string because we're starting a new add
    errorM.clear();

    // If this is the first header to be added, then just add it
    if (vContextsM.size() == 0) {
        bool ret = this->AddHeaderToEmptySet(path, includeCount, pIncludes,
                                             definitionCount, pDefinitions,
                                             tmp, error);
        if (!ret) {
            errorM = error;
        }

        return ret;
    }
    // Otherwise, it has to be added to a new ParsedContextSet and then
    // merged into this one
    else {
        ParsedContextSet contextSet;
        
        // Add it
        if (!contextSet.AddHeaderToEmptySet
            (path, includeCount, pIncludes, definitionCount, pDefinitions,
             tmp, error)) {
            errorM = error;
            return false;
        }
        
        // Check to see if it can be merged, so that this set is not altered
        // in the case that the merge would not succeed
        if (!this->CanMerge(contextSet, error)) {
            errorM = error;
            return false;
        }
        
        // Merge the new stuff in
        this->Merge(contextSet);

        return true;
    }
}


u32 ParsedContextSet::GetContextCount() const
{
    return vContextsM.size();
}

    
const Context *ParsedContextSet::GetContext(u32 index) const
{
    return vContextsM[index];
}

    
const Context *ParsedContextSet::LookupContext(const char *pFullName) const
{
    map<string, Context *>::const_iterator iter = 
        htNonAnonymousContextsByNameM.find(pFullName);
    if (iter != htNonAnonymousContextsByNameM.end()) {
        return iter->second;
    }

    return 0;
}


Context *ParsedContextSet::GetContext(Parser &parser, const string &id, 
                                      string &error)
{
    delete new ParsedStruct();

    // Look it up first
    map<string, Context *>::iterator iter = htContextsByIdM.find(id);
    if (iter != htContextsByIdM.end()) {
        // If it's already found, use it
        return iter->second;
    }

    // Not found, so it must be created
    Context *pContext;

    // Look up the parser element corresponding to this id
    Parser::Element *pElement = parser.LookupElement(id);

    // If not found, there was a gccxml error
    if (pElement == NULL) {
        error = "Bad context id: " + id;
        return 0;
    }

    // Create a new one and put it in the vector of contexts, to ensure that
    // it will be deleted in any case, and then initialize it
    string name = pElement->GetElementName();

    // If it's not a Namespace, it's one of the Structure types, and we use
    // GetStructure() to get it, so that it's handled properly
    if (name == "Namespace") {
        ParsedNamespace *pNamespace;
        pContext = pNamespace = new ParsedNamespace();
        // Add it to the hashtable by id, where it needs to be when
        // initialized
        htContextsByIdM[id] = pContext;
        if (!pNamespace->Initialize(*this, parser, pElement, error)) {
            delete pNamespace;
            return 0;
        }
    }
    else if (name == "Class") {
        ParsedClass *pClass;
        pContext = pClass = new ParsedClass();
        // Add it to the hashtable by id, where it needs to be when
        // initialized
        htContextsByIdM[id] = pContext;
        if (!pClass->Initialize(*this, parser, pElement, error)) {
            delete pClass;
            return 0;
        }
    }
    else if (name == "Struct") {
        ParsedStruct *pStruct;
        pContext = pStruct = new ParsedStruct();
        // Add it to the hashtable by id, where it needs to be when
        // initialized
        htContextsByIdM[id] = pContext;
        if (!pStruct->Initialize(*this, parser, pElement, error)) {
            delete pStruct;
            return 0;
        }
    }
    else if (name == "Union") {
        ParsedUnion *pUnion;
        pContext = pUnion = new ParsedUnion();
        // Add it to the hashtable by id, where it needs to be when
        // initialized
        htContextsByIdM[id] = pContext;
        if (!pUnion->Initialize(*this, parser, pElement, error)) {
            delete pUnion;
            return 0;
        }
    }
    else {
        error = ("Expected Class, Struct, or Union for id " + id + 
                 "; got " + name);
        return 0;
    }

    // Put it into the hashtable by name, if it's not anonymous
    if ((pContext->GetType() == Context::Type_Namespace) ||
        !((const Structure *) pContext)->IsAnonymous()) {
        name = pContext->GetFullName();
        if (htNonAnonymousContextsByNameM.find(name) != 
			htNonAnonymousContextsByNameM.end()) {
            delete pContext;
            // Duplicate
            error = "Duplicate name " + name + " for id " + id;
            return 0;
        }
        
        // Add it to the hashtable by name
        htNonAnonymousContextsByNameM[name] = pContext;
    }

    // Put it into the vector of contexts
    vContextsM.push_back(pContext);

    // Return it
    return pContext;
}


Type *ParsedContextSet::GetType(Parser &parser, const string &id, string &error)
{
    map<string, Type *>::iterator iter = htTypesByIdM.find(id);
    if (iter != htTypesByIdM.end()) {
        // If it's already found, use it
        return iter->second;
    }

    // Not found, so it must be created
    Type *pType;

    // Get the Parser element for the type's id.  pElementBase will
    // always refer to the "head" of the type "list"
    Parser::Element *pElementBase = parser.LookupElement(id);
    if (!pElementBase) {
        error = "No such element: " + id;
    }

    // pElement is the next "node" in the type "list"
    Parser::Element *pElement = pElementBase;

    // Now iterate through the type "list" until the type of the type
    // can be determined; once it is determined, create it, add it to the
    // vector of all types, and initialize it
    while (true) {
        string type = pElement->GetElementName();
            
        if (type == "FundamentalType") {
            Type::BaseType baseType;

            string name = pElement->GetAttributeValue("name");
                
            if (name == "void") {
                baseType = Type::BaseType_Void;
            }
            else if (name == "bool") {
                baseType = Type::BaseType_Bool;
            }
            else if ((name == "char") ||
                     (name == "signed char")) {
                baseType = Type::BaseType_Char;
            }
            else if (name == "unsigned char") {
                baseType = Type::BaseType_Unsigned_Char;
            }
            else if (name == "wchar_t") {
                baseType = Type::BaseType_WChar;
            }
            else if ((name == "short int") ||
                     (name == "short signed int")) {
                baseType = Type::BaseType_Short;
            }
            else if (name == "short unsigned int") {
                baseType = Type::BaseType_Unsigned_Short;
            }
            else if ((name == "int") ||
                     (name == "signed int")) {
                baseType = Type::BaseType_Int;
            }
            else if (name == "unsigned int") {
                baseType = Type::BaseType_Unsigned_Int;
            }
            else if ((name == "long int") ||
                     (name == "long signed int")) {
                baseType = Type::BaseType_Long;
            }
            else if (name == "long unsigned int") {
                baseType = Type::BaseType_Unsigned_Long;
            }
            else if ((name == "long long int") ||
                     (name == "long long signed int")) {
                baseType = Type::BaseType_Long_Long;
            }
            else if (name == "long long unsigned int") {
                baseType = Type::BaseType_Unsigned_Long_Long;
            }
            else if (name == "float") {
                baseType = Type::BaseType_Float;
            }
            else if (name == "double") {
                baseType = Type::BaseType_Double;
            }
            else if (name == "long double") {
                baseType = Type::BaseType_Long_Double;
            }
            else {
                error = "Unexpected fundamental type: " + name;
                return 0;
            }

            ParsedType *pParsedType;
            pType = pParsedType = new ParsedType();
            // And put into the hashtable
            htTypesByIdM[id] = pType;
            if (!pParsedType->Initialize
                (*this, parser, baseType, pElementBase, error)) {
                delete pType;
                return 0;
            }
            break;
        }
        else if (type == "Enumeration") {
            ParsedTypeEnumeration *pTypeEnumeration;
            pType = pTypeEnumeration = new ParsedTypeEnumeration();
            // And put into the hashtable
            htTypesByIdM[id] = pType;
            if (!pTypeEnumeration->Initialize
                (*this, parser, pElementBase, pElement, error)) {
                delete pType;
                return 0;
            }
            break;
        }
        else if (type == "FunctionType") {
            ParsedTypeFunction *pTypeFunction;
            pType = pTypeFunction = new ParsedTypeFunction();
            // And put into the hashtable
            htTypesByIdM[id] = pType;
            if (!pTypeFunction->Initialize
                (*this, parser, pElementBase, pElement, error)) {
                delete pType;
                return 0;
            }
            break;
        }
        // Class types finish the type definition
        else if ((type == "Class") || (type == "Struct") || 
                 (type == "Union")) {
            ParsedTypeStructure *pTypeStructure;
            // And put into the hashtable
            pType = pTypeStructure = new ParsedTypeStructure();
            htTypesByIdM[id] = pType;
            if (!pTypeStructure->Initialize
                (*this, parser, pElementBase, pElement, error)) {
                delete pType;
                return 0;
            }
            break;
        }
        // These types just decorate the true type, and we then look at
        // the next in the chain of types
        else if ((type == "CvQualifiedType") ||
                 (type == "ReferenceType") ||
                 (type == "PointerType") ||
                 (type == "ArrayType") ||
                 (type == "Typedef")) {
            // Next in chain
            if (!(pElement = parser.LookupElement
                  (pElement->GetAttributeValue("type")))) {
                error = ("Unknown element type " + type + " from id " +
                         pElement->GetAttributeValue("id") + 
                         " while initializing type id " + id);
                return 0;
            }
        }
        else {
            error = ("Unexpected type " + type + " referenced by id " + 
                     pElement->GetAttributeValue("id"));
            return 0;
        }
    }

    // Put it into the vector
    vTypesM.push_back(pType);

    // Return it
    return pType;
}


Type *ParsedContextSet::GetType(Context *pContext, string &error)
{
    // Look it up first
    string hashCode = pContext->GetFullName();

    map<string, Type *>::iterator iter = 
        htExtraTypesByNameM.find(hashCode);
    if (iter != htExtraTypesByNameM.end()) {
        // If it's already found, use it
        return iter->second;
    }

    // Not found, so it must be created
    Type *pType;
    ParsedTypeStructure *pTypeStructure;

    pType = pTypeStructure = new ParsedTypeStructure();

    // And put into the extra hashtable
    htExtraTypesByNameM[hashCode] = pType;

    // Initialize it
    if (!pTypeStructure->Initialize(pContext, error)) {
        delete pType;
        return 0;
    }

    // Put it into the vector
    vTypesM.push_back(pType);

    return pType;
}


Enumeration *ParsedContextSet::GetEnumeration(Parser &parser,
                                              const string &id, 
                                              string &error)
{
    if (htEnumerationsByIdM.count(id)) {
        // If it's already found, use it
        return htEnumerationsByIdM[id];
    }

    // Not found, so it must be created
    Enumeration *pEnumeration;

    // Get the Parser element for the type's id.
    Parser::Element *pElement = parser.LookupElement(id);
    if (!pElement) {
        error = "No such element: " + id;
        return 0;
    }

    ParsedEnumeration *pParsedEnumeration;

    pEnumeration = pParsedEnumeration = new ParsedEnumeration();

    // And put into the hashtable
    htEnumerationsByIdM[id] = pEnumeration;
        
    // Initialize it
    if (!pParsedEnumeration->Initialize(*this, parser, pElement, error)) {
        delete pEnumeration;
        return 0;
    }

    // Put it into the vector
    vEnumerationsM.push_back(pEnumeration);

    // Return it
    return pEnumeration;
}


bool ParsedContextSet::AddHeaderToEmptySet(const char *file, u32 includeCount,
                                           const char **pIncludes,
                                           u32 definitionCount,
                                           const char **pDefinitions,
                                           const char *tmpfile,
                                           string &error)
{
    // Compose the gccxml command
    string gccxml = "gccxml";

    // -I arguments
    for (u32 i = 0; i < includeCount; i++) {
        gccxml += " -I";
        gccxml += pIncludes[i];
    }

    // -D arguments
    for (u32 i = 0; i < definitionCount; i++) {
        gccxml += " -D\"";
        gccxml += pDefinitions[i];
        gccxml += "\"";
    }

    // -fxml=tmpfile
    gccxml += " -fxml=\"";
    gccxml += tmpfile;
    gccxml += "\"";

    // incfile
    gccxml += " \"";
    gccxml += file;
    gccxml += "\"";
        
    // Execute gccxml
    if (system(gccxml.c_str())) {
        error = "Failed to execute gccxml command: " + gccxml;
        return false;
    }
            
    // Open the resulting file
    int infile;
    if ((infile = open(tmpfile, O_RDONLY)) == -1) {
        error = "Failed to open gccxml output file " + string(tmpfile);
        return false;
    }
            
    // Parse the resulting gccxml file
    Parser parser;
    parser.Parse(infile);

    // Close the file
    if (close(infile)) {
        error = "Failed to close gccxml output file " + string(tmpfile);
        return false;
    }

    // Delete the file
    // (void) unlink(tmpfile);
    
    // Look at every element parsed
	map<string, Parser::Element *>::iterator iter = parser.GetElementsBegin();

    // For each Class/Struct/Union element, determine if it is to be
    // included in the output
	for ( ; iter != parser.GetElementsEnd(); iter++) {
        Parser::Element *pElement = iter->second;
        string elementName = pElement->GetElementName();

        // Skip it if it's not a Class, Struct, or Union
        if ((elementName != "Class") && (elementName != "Struct") &&
            (elementName != "Union")) {
            continue;
        }

        // Check the id to make sure it's OK
        string id = pElement->GetAttributeValue("id");
        if (id == "") {
            string lineNumber = 
                StringUtils::ToString
                (StringUtils::Format(L"%lu", pElement->GetLineNumber()));
            error = (elementName + " element at line " + lineNumber +
                     "is missing an id attribute");
            return false;
        }

        // Skip it if it's a gccxml-generated pseudo class.  We detect
        // these classes because they are defined in the File "<internal>"
        Parser::Element *pFileElement = parser.LookupElement
            (pElement->GetAttributeValue("file"));
        if ((pFileElement != NULL) &&
            (pFileElement->GetAttributeValue("name") == "<internal>")) {
            continue;
        }
            
        // Also skip these very weird gccxml-generated classes
        string fullName = get_full_name_of(parser, id);
        if (StringUtils::StartsWith(fullName, "__") &&
            (fullName.find("_type_info_pseudo", 0) != string::npos)) {
            continue;
        }

        // OK, it's a Class/Struct/Union, we'll include it

        // Get it, which populates the hashtables and vectors with it and
        // everything it references
        if (!this->GetContext(parser, id, /* returns */ error)) {
            // Failed to get it, return with error
            return false;
        }
    }

    return true;
}


bool ParsedContextSet::CanMerge(ParsedContextSet &set, string & /* error */)
{
    // For each context in the new set, check to make sure that if it exists
    // in this set, it is exactly equal.
    u32 count = set.vContextsM.size();
    for (u32 i = 0; i < count; i++) {
        const Context *pContext = set.vContextsM[i];
        // Find a corresponding context from this set, but only if it's
        // non-anonymous and not incomplete; anonymous and incomplete contexts
        // can always be merged
        if (pContext->GetType() == Context::Type_Namespace) {
            continue;
        }

        const Structure *pStructure = (const Structure *) pContext;

        if (pStructure->IsIncomplete()) {
            continue;
        }

        if (pStructure->IsAnonymous()) {
            continue;
        }

        // Look up non-anonymous contexts in the hashtable
        const Context *pContextThis = 
            this->LookupContext(pContext->GetFullName());
        
        // If we have it ...
        if (pContextThis) {
            // If what we have is incomplete, then we can merge it by
            // replacing what we have
            if (((Structure *) pContextThis)->IsIncomplete()) {
                continue;
            }

            // if it's not equal, then we can't merge it
            if (!Equals(*pContextThis, *pContext)) {
                return false;
            }
        }
    }

    return true;
}


void ParsedContextSet::Merge(ParsedContextSet &from)
{
    // For each context in the from set, if it's in this set, skip it.
    // Otherwise:
    //   - Move it into this set
    //   - For each context/type/enumeration pointed to by the context,
    //     if it's in this set, replace the pointer to the one in this
    //     set; otherwise, move it into this set, and recurse
    // Copy the vector of contexts to merge since the source vector will
    // change during the merging (should fix Vector__Enumeration to work
    // properly in this case and just use that)
    vector<Context *> vContexts(from.vContextsM);

    u32 count = vContexts.size();
    for (u32 i = 0; i < count; i++) {
        this->MergeContext(vContexts[i], from);
    }
}

Context *ParsedContextSet::MergeContext(Context *pContext, 
                                        ParsedContextSet &from)
{
    // If we already have this context:
    // - If what we have is incomplete, then we can merge in the new one
    // - Otherwise, return what we have, since it must match the new one
    //   (or the new one is incomplete, and we continue to use what we have)
    Context *pFound = 0;

    if ((pContext->GetType() == Context::Type_Namespace) ||
        !((const Structure *) pContext)->IsAnonymous()) {
        if (htNonAnonymousContextsByNameM.count(pContext->GetFullName())) {
            pFound = htNonAnonymousContextsByNameM[pContext->GetFullName()];
        }
        else {
            pFound = 0;
        }
    }
    else {
        u32 count = vContextsM.size();
        for (u32 i = 0; i < count; i++) {
            Context *pThisContext = vContextsM[i];
            if (*pThisContext == *pContext) {
                pFound = pThisContext; 
            }
        }
    }

    // If it was found
    if (pFound) {
        // If it's the same as what we're looking for, this means it's already
        // been merged in and we just return it
        if (pFound == pContext) {
            return pFound;
        }

        // ... then check to see if what we have is not incomplete
        if ((pFound->GetType() == Context::Type_Namespace) ||
            !((Structure *) pFound)->IsIncomplete()) {
            // ... and if it is not incomplete, just return it, because it
            // must match what they have (or what they had was incomplete)
            return pFound;
        }

        // We remove what we had from our hashtable
        if ((pFound->GetType() == Context::Type_Namespace) ||
            !((const Structure *) pFound)->IsAnonymous()) {
            htNonAnonymousContextsByNameM.erase(pFound->GetFullName());
        }

        // And also remove what we had from our vector
        u32 count = vContextsM.size();
        for (u32 i = 0; i < count; i++) {
            if (vContextsM[i] == pFound) {
                vContextsM.erase(vContextsM.begin() + i);
                break;
            }
        }

        // And delete it because we don't need it any more
        delete pFound;

        // And finally, replace any reference to the old one with the new one
        this->ReplaceContext(pFound, pContext);
        
        // And proceed to merge it in the rest of the way
    }

    // Remove it from from
    u32 count = from.vContextsM.size();
    for (u32 i = 0; i < count; i++) {
        if (from.vContextsM[i] == pContext) {
            from.vContextsM.erase(from.vContextsM.begin() + i);
            break;
        }
    }
    
    // We get this context now
    vContextsM.push_back(pContext);

    if ((pContext->GetType() == Context::Type_Namespace) ||
        !((const Structure *) pContext)->IsAnonymous()) {
        htNonAnonymousContextsByNameM[pContext->GetFullName()] = pContext;
    }
    
    // Now, it has to be "merged in", which means that everything it
    // referenced must be either pulled in, or replaced with our
    // pre-existing version
    switch (pContext->GetType()) {
    case Context::Type_Class:
        ((ParsedClass *) pContext)->MergeContents(*this, from);
        break;
    case Context::Type_Namespace:
        ((ParsedNamespace *) pContext)->MergeContents(*this, from);
        break;
    case Context::Type_Struct:
        ((ParsedStruct *) pContext)->MergeContents(*this, from);
        break;
    default: // Context::Type_Union
        ((ParsedUnion *) pContext)->MergeContents(*this, from);
        break;
    }

    return pContext;
}


Type *ParsedContextSet::MergeType(Type *pType, ParsedContextSet &from)
{
    // Must do a linear search for the type since there is no way to hash.
    // A hashtable could be added later if this turns out to be too slow.
    u32 count = vTypesM.size();
    
    for (u32 i = 0; i < count; i++) {
        Type *pThisType = vTypesM[i];

        if (Equals(*pThisType, *pType)) {
            return pThisType;
        }
    }

    // Remove it from from
    count = from.vTypesM.size();
    for (u32 i = 0; i < count; i++) {
        if (from.vTypesM[i] == pType) {
            from.vTypesM.erase(from.vTypesM.begin() + i);
            break;
        }
    }
    
    // We get this context now
    vTypesM.push_back(pType);
    
    // Now, it has to be "merged in", which means that everything it
    // referenced must be either pulled in, or replaced with our
    // pre-existing version
    switch (pType->GetBaseType()) {
    case Type::BaseType_Enumeration:
        ((ParsedTypeEnumeration *) pType)->MergeContents(*this, from);
        break;
    case Type::BaseType_Function:
        ((ParsedTypeFunction *) pType)->MergeContents(*this, from);
        break;
    case Type::BaseType_Structure:
        ((ParsedTypeStructure *) pType)->MergeContents(*this, from);
        break;
    default: // fundamental types
        break;
    }

    return pType;
}


Enumeration *ParsedContextSet::MergeEnumeration(Enumeration *pEnumeration, 
                                                ParsedContextSet &from)
{
    // Just do a linear search for it.  A hashtable of enumerations by
    // full name could be added later if this is too slow.
    u32 count = vEnumerationsM.size();
    
    for (u32 i = 0; i < count; i++) {
        Enumeration *pThisEnumeration = vEnumerationsM[i];

        if (*pThisEnumeration == *pEnumeration) {
            return pThisEnumeration;
        }
    }

    // Remove it from from
    count = from.vEnumerationsM.size();
    for (u32 i = 0; i < count; i++) {
        if (from.vEnumerationsM[i] == pEnumeration) {
            from.vEnumerationsM.erase(from.vEnumerationsM.begin() + i);
            break;
        }
    }
    
    // We get this context now
    vEnumerationsM.push_back(pEnumeration);

    // Now merge its contents too
    ((ParsedEnumeration *) pEnumeration)->MergeContents(*this, from);

    return pEnumeration;
}


void ParsedContextSet::ReplaceContext(Context *pOld, Context *pNew)
{
    u32 count = vContextsM.size();

    for (u32 i = 0; i < count; i++) {
        Context *pThisContext = vContextsM[i];
        switch (pThisContext->GetType()) {
        case Context::Type_Class:
            ((ParsedClass *) pThisContext)->ReplaceContext(pOld, pNew);
            break;
        case Context::Type_Struct:
            ((ParsedStruct *) pThisContext)->ReplaceContext(pOld, pNew);
            break;
        case Context::Type_Union:
            ((ParsedUnion *) pThisContext)->ReplaceContext(pOld, pNew);
            break;
        default:
            break;
        }
    }
}


}; // namespace Xrtti
