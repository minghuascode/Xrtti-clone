/*****************************************************************************\
 *                                                                           *
 * Generator.cpp                                                             *
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
#include <string.h>
#include <private/Generator.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


u32 GeneratorObject::nextNumberG = 1;


Generator::Generator(Configuration &config, const ContextSet &contextSet)
    : configM(config)
{
    // Build up the vector of starting point GeneratorContexts
    u32 count = contextSet.GetContextCount();
    for (u32 i = 0; i < count; i++) {
        const Context *pContext = contextSet.GetContext(i);
        if (config.ShouldInclude(pContext->GetFullName())) {
            (void) this->GetGeneratorContext(pContext);
        }
    }
}

Generator::~Generator()
{
    {
        map<const Context *, GeneratorContext *>::iterator iter =
            htGeneratorContextsM.begin();

        while (iter != htGeneratorContextsM.end()) {
            delete iter->second;
            iter++;
        }
    }

    {
        map<const Type *, GeneratorType *>::iterator iter =
            htGeneratorTypesM.begin();

        while (iter != htGeneratorTypesM.end()) {
            delete iter->second;
            iter++;
        }
    }

    {
        map<const Enumeration *, GeneratorEnumeration *>::iterator iter =
            htGeneratorEnumerationsM.begin();

        while (iter != htGeneratorEnumerationsM.end()) {
            delete iter->second;
            iter++;
        }
    }
}


bool Generator::Generate()
{
    string outFile = configM.GetOutputFile();
    FILE *file;
    bool createdFile;

    if (outFile == "-") {
        file = stdout;
        createdFile = false;
    }
    else {
        if ((file = fopen(outFile.c_str(), "w")) == NULL) {
            return false;
        }
        createdFile = true;
    }

    // Emit header
    this->EmitHeader(file);

    // Emit XrttiAccess
    this->EmitXrttiAccess(file);

    // Emit XrttiACcess static definitions
    this->EmitXrttiAccessStaticDefinitions(file);

    // Emit declarations
    this->EmitDeclarations(file);

    // Emit definitions
    this->EmitDefinitions(file);

    // Emit register
    this->EmitRegister(file);

    if (createdFile) {
        fclose(file);
    }

    return true;
}


GeneratorContext *Generator::GetGeneratorContext(const Context *pContext)
{
    map<const Context *, GeneratorContext *>::iterator iter = 
        htGeneratorContextsM.find(pContext);
    if (iter != htGeneratorContextsM.end()) {
        return iter->second;
    }
    else {
        // The constructors automatically add the context to the hashtable
        switch (pContext->GetType()) {
        case Context::Type_Class:
            return new GeneratorClass(*this, (const Class &) *pContext);
        case Context::Type_Namespace:
            return new GeneratorNamespace(*this, (const Namespace &) *pContext);
        case Context::Type_Struct:
            return new GeneratorStruct(*this, (const Struct &) *pContext);
        default: // Context::Type_Union:
            return new GeneratorUnion(*this, (const Union &) *pContext);
        }
    }
}


GeneratorType *Generator::GetGeneratorType(const Type *pType)
{
    map<const Type *, GeneratorType *>::iterator iter =
        htGeneratorTypesM.find(pType);
    if (iter != htGeneratorTypesM.end()) {
        return iter->second;
    }
    else {
        // The constructors automatically add the type to the hashtable
        switch (pType->GetBaseType()) {
        case Type::BaseType_Enumeration:
            return new GeneratorTypeEnumeration
                (*this, (const TypeEnumeration &) *pType);
        case Type::BaseType_Function:
            return new GeneratorTypeFunction
                (*this, (const TypeFunction &) *pType);
        case Type::BaseType_Structure:
            return new GeneratorTypeStructure
                (*this, (const TypeStructure &) *pType);
        default: // basic types
            return new GeneratorType(*this, *pType);
        }
    }
}


GeneratorEnumeration *Generator::
GetGeneratorEnumeration(const Enumeration *pEnumeration)
{
    map<const Enumeration *, GeneratorEnumeration *>::iterator iter =
        htGeneratorEnumerationsM.find(pEnumeration);
    if (iter != htGeneratorEnumerationsM.end()) {
        return iter->second;
    }
    else {
        // The constructor automatically adds the enumeration to the hashtable
        return new GeneratorEnumeration(*this, *pEnumeration);
    }
}


void Generator::RegisterGeneratorContext(const Context *pContext,
                                         GeneratorContext *pGeneratorContext)
{
    htGeneratorContextsM[pContext] = pGeneratorContext;
}


void Generator::RegisterGeneratorType(const Type *pType,
                                      GeneratorType *pGeneratorType)
{
    htGeneratorTypesM[pType] = pGeneratorType;
}


void Generator::
RegisterGeneratorEnumeration(const Enumeration *pEnumeration,
                             GeneratorEnumeration *pGeneratorEnumeration)
{
    htGeneratorEnumerationsM[pEnumeration] = pGeneratorEnumeration;
}

/* static */
void Generator::EmitBooleanArgument(FILE *file, bool val, bool comma)
{
    fprintf(file, "        %s%s", val ? "true" : "false", comma ? ",\n" : "");
}


/* static */
void Generator::EmitStringArgument(FILE *file, const char *str, bool comma)
{
    fprintf(file, "        \"%s\"%s", str, comma ? ",\n" : "");
}


/* static */
void Generator::EmitU32Argument(FILE *file, u32 number, bool comma)
{
    fprintf(file, "        %luUL%s", (unsigned long) number, 
            comma ? ",\n" : "");
}


/* static */
void Generator::EmitAccessTypeArgument(FILE *file, AccessType accessType, 
                                       bool comma)
{
    const char *str;

    switch (accessType) {
    case AccessType_Public:
        str = "Xrtti::AccessType_Public";
        break;
    case AccessType_Protected:
        str = "Xrtti::AccessType_Protected";
        break;
    default:
        str = "Xrtti::AccessType_Private";
        break;
    }

    fprintf(file, "        %s%s", str, comma ? ",\n" : "");
}


/* static */
void Generator::EmitArrayArgument(FILE *file, u32 count,
                                  const char *name, u32 number, bool comma)
{
    Generator::EmitU32Argument(file, count, true);

    if (count) {
        fprintf(file, "        _%lu_%s%s", (unsigned long) number, name, 
                comma ? ",\n" : "");
    }
    else {
        Generator::EmitU32Argument(file, 0, comma);
    }
}


/* static */
bool Generator::IsAccessible(const Context *pContext)
{
    while (true) {
        // Namespaces are always accessible
        if (pContext->GetType() == Context::Type_Namespace) {
            return true;
        }

        const Structure *pStructure = (const Structure *) pContext;

        // If it's anonymous or incomplete, then no it is not
        if (pStructure->IsAnonymous() || pStructure->IsIncomplete()) {
            return false;
        }

        // If it's not public, then it's probably not accessible
        if (pStructure->GetAccessType() != AccessType_Public) {
            // OK the only saving grace would be if the parent class has
            // XrttiAccess
            pContext = pStructure->GetContext();
            if (pContext->GetType() == Context::Type_Namespace) {
                return false;
            }
            return IsXrttiAccessFriend((const Structure *) pContext);
        }

        // Check its context; we know there must be one because every
        // context terminates at the :: namespace
        pContext = pStructure->GetContext();
    }
}


/* static */
bool Generator::IsAccessible(const Member *pMember)
{
    // First off, the containing context must be accessible
    const Context &context = pMember->GetContext();
    if (!IsAccessible(&context)) {
        return false;
    }

    // Next up, if the containing context is a friend of XrttiAccess then
    // everything in it is accessible, so this member is
    if ((context.GetType() != Context::Type_Namespace) &&
        IsXrttiAccessFriend((const Structure *) &context)) {
        return true;
    }

    // Finally, it's only accessible if it's public
    return (pMember->GetAccessType() == AccessType_Public);
}


/* static */
bool Generator::IsPod(const Context *pContext)
{
    if (pContext->GetType() == Context::Type_Namespace) {
        return true;
    }

    const Structure *pStructure = (const Structure *) pContext;

    u32 count = pStructure->GetFieldCount();
    for (u32 i = 0; i < count; i++) {
        const Field &field = pStructure->GetField(i);

        // Ignore static fields; they aren't really in the structure
        if (field.IsStatic()) {
            continue;
        }

        // A POD class cannot have:
        // - protected or private fields
        if ((field.GetAccessType() == AccessType_Protected) ||
            (field.GetAccessType() == AccessType_Private)) {
            return false;
        }
        
        // A POD class cannot have:
        // - non-POD members
        const Xrtti::Type &type = field.GetType();

        // If it's a structure type
        if (type.GetBaseType() == Xrtti::Type::BaseType_Structure) {
            const Xrtti::TypeStructure &typeStructure =
                (const Xrtti::TypeStructure &) type;
            bool isPointer = false;
            u32 count2 = typeStructure.GetArrayOrPointerCount();
            for (u32 j = 0; j < count2; j++) {
                if (typeStructure.GetArrayOrPointer(j).GetType() == 
                    ArrayOrPointer::Type_Pointer) {
                    isPointer = true;
                    break;
                }
            }
            
            // If it's not a pointer, and thus is embedded, then the
            // embedded structure must be POD
            if (!isPointer && 
                !Generator::IsPod(&typeStructure.GetStructure())) {
                return false;
            }
        }
        
        // A POD class cannot have:
        // - pointers to member methods
        // Not sure how to detect this
        
        // A POD class cannot have:
        // - references
        if (type.IsReference()) {
            return false;
        }
    }
    
    // A POD class cannot have:
    // - constructors
    if (pStructure->GetConstructorCount()) {
        return false;
    }
    
    // A POD class cannot have:
    // - base classes
    if (pStructure->GetBaseCount()) {
        return false;
    }

    if (pStructure->GetType() == Context::Type_Union) {
        return true;
    }

    const Struct *pStruct = (const Struct *) pStructure;

    // A POD class cannot have:
    // - virtual methods
    // - overloaded assignment operator
    count = pStruct->GetMethodCount();
    for (u32 i = 0; i < count; i++) {
        const Method &method = pStruct->GetMethod(i);

        if (method.IsVirtual()) {
            return false;
        }

        if (method.IsOperatorMethod() && !strcmp(method.GetName(), "=")) {
            return false;
        }
    }

    return true;
}


/* static */
string Generator::GetTypeName(const Context *pContext)
{
    switch (pContext->GetType()) {
    case Context::Type_Class:
        if (((const Structure *) pContext)->HasStructureName()) {
            return (string("class ") + pContext->GetFullName());
        }
        else {
            return pContext->GetFullName();
        }
    case Context::Type_Namespace:
        return pContext->GetFullName();
    case Context::Type_Struct:
        if (((const Structure *) pContext)->HasStructureName()) {
            return (string("struct ") + pContext->GetFullName());
        }
        else {
            return pContext->GetFullName();
        }
    default: // Context::Type_Union
        if (((const Structure *) pContext)->HasStructureName()) {
            return (string("union ") + pContext->GetFullName());
        }
        else {
            return pContext->GetFullName();
        }
    }
}


/* static */
bool Generator::IsXrttiAccessFriend(const Structure *pStructure)
{
    u32 count = pStructure->GetFriendCount();
    for (u32 i = 0; i < count; i++) {
        if (!strcmp(pStructure->GetFriend(i).GetFullName(), "XrttiAccess")) {
            return true;
        }
    }

    return false;
}


void Generator::EmitHeader(FILE *file)
{
    // Emit the disclaimer
    fprintf(file, "/****************************************************"
            "*************************\\\n"
            " *                                                         "
            "                  *\n"
            " *    AUTOGENERATED FILE - DO NOT EDIT - "
            "AUTOGENERATED FILE - DO NOT EDIT    *\n"
            " *                                                         "
            "                  *\n"
            " *      This file was generated by the "
            "xrttigen program.                     *\n"
            " *      Do not edit this file directly.  Edit the source "
            "file(s) instead     *\n"
            " *      and re-generate this file.                         "
            "                  *\n"
            " *                                                          "
            "                 *\n"
            " *    AUTOGENERATED FILE - DO NOT EDIT - "
            "AUTOGENERATED FILE - DO NOT EDIT    *\n"
            " *                                                          "
            "                 *\n"
            "\\************************************************************"
            "*****************/\n\n");

    // Emit the "includes"
    fprintf(file, "#include <stddef.h>\n");
    fprintf(file, "#include <typeinfo>\n");
    fprintf(file, "#include <Xrtti/XrttiPrivate.h>\n");
    u32 count = configM.GetHeaderCount();
    // If there was no -h argument at all, then include the input file names
    if (count == 0) {
        count = configM.GetInputCount();
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "#include \"%s\"\n", configM.GetInput(i).c_str());
        }
    }
    else for (u32 i = 0; i < count; i++) {
        string header = configM.GetHeader(i);
        if (header.at(0) == '<') {
            fprintf(file, "#include %s\n", header.c_str());
        }
        else {
            fprintf(file, "#include \"%s\"\n", header.c_str());
        }
    }

    fprintf(file, "\n");
}


void Generator::EmitXrttiAccess(FILE *file)
{
    fprintf(file, "class XrttiAccess\n{\npublic:\n\n");

    this->EmitTypedefs(file);

    fprintf(file, "\n");

    map<const Context *, GeneratorContext *>::iterator iter = 
        htGeneratorContextsM.begin();

    while (iter != htGeneratorContextsM.end()) {
        (iter++)->second->EmitXrttiAccess(file);
    }

    fprintf(file, "};\n\n");
}


void Generator::EmitXrttiAccessStaticDefinitions(FILE *file)
{
    map<const Context *, GeneratorContext *>::iterator iter =
        htGeneratorContextsM.begin();

    while (iter != htGeneratorContextsM.end()) {
        (iter++)->second->EmitXrttiAccessStaticDefinitions(file);
    }

    fprintf(file, "\n");
}


void Generator::EmitTypedefs(FILE *file)
{
    map<const Context *, GeneratorContext *>::iterator iter = 
        htGeneratorContextsM.begin();

    while (iter != htGeneratorContextsM.end()) {
        (iter++)->second->EmitTypedefs(file);
    }
}


void Generator::EmitDeclarations(FILE *file)
{
    fprintf(file, "namespace\n{\n\nstruct _\n{\n");

    map<const Context *, GeneratorContext *>::iterator iter =
        htGeneratorContextsM.begin();

    while (iter != htGeneratorContextsM.end()) {
        (iter++)->second->EmitDeclaration(file);
    }

    map<const Type *, GeneratorType *>::iterator iter2 =
        htGeneratorTypesM.begin();

    while (iter2 != htGeneratorTypesM.end()) {
        (iter2++)->second->EmitDeclaration(file);
    }

    map<const Enumeration *, GeneratorEnumeration *>::iterator iter3 =
        htGeneratorEnumerationsM.begin();

    while (iter3 != htGeneratorEnumerationsM.end()) {
        (iter3++)->second->EmitDeclaration(file);
    }

    fprintf(file, "};\n\n}\n\n");
}


void Generator::EmitDefinitions(FILE *file)
{
    fprintf(file, "namespace\n{\n");

    map<const Context *, GeneratorContext *>::iterator iter =
        htGeneratorContextsM.begin();

    while (iter != htGeneratorContextsM.end()) {
        (iter++)->second->EmitDefinition(file);
    }

    map<const Type *, GeneratorType *>::iterator iter2 =
        htGeneratorTypesM.begin();

    while (iter2 != htGeneratorTypesM.end()) {
        (iter2++)->second->EmitDefinition(file);
    }

    map<const Enumeration *, GeneratorEnumeration *>::iterator iter3 =
        htGeneratorEnumerationsM.begin();

    while (iter3 != htGeneratorEnumerationsM.end()) {
        (iter3++)->second->EmitDefinition(file);
    }

    fprintf(file, "}\n\n");
}


void Generator::EmitRegister(FILE *file)
{
    fprintf(file, "namespace\n{\n");

    if (htGeneratorContextsM.size()) {
        fprintf(file, "    static Xrtti::Context *contexts[] =\n    {\n");

        map<const Context *, GeneratorContext *>::iterator iter =
            htGeneratorContextsM.begin();

        while (iter != htGeneratorContextsM.end()) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) (iter++)->second->GetNumber(),
                    (iter == htGeneratorContextsM.end()) ? "" : ",");
        }

        fprintf(file, "    };\n\n");
    }

    fprintf(file, "    static Xrtti::CompiledRegister registration\n    (\n"
            "        %lu,\n        %s\n    );\n",
            (unsigned long) htGeneratorContextsM.size(),
            htGeneratorContextsM.size() ? "contexts" : "0");

    fprintf(file, "}\n");
}


}; // namespace Xrtti
