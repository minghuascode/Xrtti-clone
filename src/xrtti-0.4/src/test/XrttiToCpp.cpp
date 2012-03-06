/*****************************************************************************\
 *                                                                           *
 * XrttiToCpp.cpp                                                            *
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

#include <map>
#include <stdarg.h>
#include <string>
#include <vector>
#include <private/StringUtils.h>
#include "test/XrttiToCpp.h"
#include <Xrtti/Xrtti.h>

using namespace Xrtti;
using namespace std;

typedef Xrtti::Enumeration X_Enumeration;

enum Access
    {
        Unknown,
        Public,
        Protected,
        Private
    };

class OutputIndentStream
{
public:

    OutputIndentStream(FILE *file)
        : fileM(file), indentM(0), newlineM(true)
    {
    }

	u32 getIndent()
	{
		return indentM;
	}

    void indent()
    {
        indentM++;
    }

    void deindent()
    {
		indentM--;
    }

    inline void Write(const string &string)
    {
        this->WriteIndentIfNecessary();

        fprintf(fileM, "%s", string.c_str());
    }

    inline void WriteF(const char *pFormat, ...)
    {
        this->WriteIndentIfNecessary();

        va_list valist;
        
        va_start(valist, pFormat);

        vfprintf(fileM, pFormat, valist);
        
        va_end(valist);
    }

    inline void WriteLine()
    {
        this->Write("\n");

        newlineM = true;
    }

    inline void WriteLine(const string &string)
    {
        this->Write(string);

        this->WriteLine();
    }

    inline void WriteLineF(const char *pFormat, ...)
    {
        this->WriteIndentIfNecessary();

        va_list valist;
        
        va_start(valist, pFormat);

        vfprintf(fileM, pFormat, valist);

        va_end(valist);

        this->WriteLine();
    }
    
private:

    inline void WriteIndentIfNecessary()
    {
        if (!newlineM) {
            return;
        }

        newlineM = false;
        
        for (u32 i = 0; i < indentM; i++) {
            fprintf(fileM, "    ");
        }
    }

    FILE *fileM;
    u32 indentM;
    bool newlineM;
};


static void SortIntoByName(const Context &context,
                           vector<const Context *> &vContexts);
static void AddToHierarchy(const Context &context);
static void CollectEnumerations(const Context &context);
static bool IsInAnonymousChain(const Structure &structure);
static Access PrintAccess(OutputIndentStream &out, AccessType accessType,
                          Access access);
static void PrintFunctionTypedefs(OutputIndentStream &out, 
                                  const Context &context, bool contextSet);
static void PrintChildrenContexts(OutputIndentStream &out, 
                                  const Context &context, bool contextSet);
static void PrintNamespace(OutputIndentStream &out,
                           const Namespace &namespaceRef, bool contextSet);
static void PrintStructure(OutputIndentStream &out, const Structure &structRef,
                           bool contextSet);
static void PrintContext(OutputIndentStream &out, const Context &context,
                         bool contextSet);
static void PrintField(OutputIndentStream &out, const Field &field,
                       bool contextSet);
static void PrintConstructorSuffix(OutputIndentStream &out, 
                                   const Constructor &c, bool contextSet);
static void PrintMethod(OutputIndentStream &out, const Method &method, 
                        bool contextSet);
static void PrintEnumeration(OutputIndentStream &out,
                             const X_Enumeration &enumeration);
#if 0 // This was only used when defaults were not strings, but actual
      // typed values
static void PrintValue(OutputIndentStream &out, const Type &type, 
                       const void *pValue);
#endif
static void PrintTypePrefix(OutputIndentStream &out, const Type &type,
                            bool contextSet, bool space);
static void PrintTypeSuffix(OutputIndentStream &, const Type &type);
static void PrintConstructorSignatureSuffix(OutputIndentStream &out, 
                                            const ConstructorSignature &sig,
                                            vector<const char *> *pArgNames,
                                            bool contextSet);

class ContextExtra
{
public:

    static ContextExtra &Get(const Context &context)
    {
        string fullName = context.GetFullName();
        map<string, ContextExtra *>::iterator iter = 
            htContextExtrasG.find(fullName);
        if (iter != htContextExtrasG.end()) {
            return *(iter->second);
        }

        ContextExtra *pContextExtra = new ContextExtra();

        htContextExtrasG[fullName] = pContextExtra;

        return *pContextExtra;
    }

    bool HasBeenPrinted()
    {
        return hasBeenPrintedM;
    }

    void SetHasBeenPrinted()
    {
        hasBeenPrintedM = true;
    }

    void AddChild(const Context &context)
    {
        const Context *pContext = &context;

        vChildrenM.push_back(pContext);
    }

    u32 GetChildCount()
    {
        return vChildrenM.size();
    }

    const Context &GetChild(u32 index)
    {
        return *(vChildrenM[index]);
    }

    void AddEnumeration(const X_Enumeration &enumeration)
    {
        const X_Enumeration *pEnumeration = &enumeration;

        // Make sure it's not there already
        if (find(vEnumerationsM.begin(), vEnumerationsM.end(), pEnumeration)
            != vEnumerationsM.end()) {
            return;
        }

        vEnumerationsM.push_back(pEnumeration);
    }

    u32 GetEnumerationCount()
    {
        return vEnumerationsM.size();
    }

    const X_Enumeration &GetEnumeration(u32 index)
    {
        return *(vEnumerationsM[index]);
    }

    void SortChildren()
    {
        u32 count = vChildrenM.size();

        // This will hold the sorted children
        vector<const Context *> vSortedChildren;

        // For each child
        for (u32 i = 0; i < count; i++) {
            const Context *pChild = vChildrenM[i];

            // For each child already in the sorted list, if this one should
            // go before it, insert it there
            u32 where;
            for (where = 0; where < i; where++) {
                if (ShouldComeBefore(pChild, vSortedChildren[where])) {
                    break;
                }
            }

            vSortedChildren.insert(vSortedChildren.begin() + where, pChild);
        }

        // Replace the children with the sorted children
        vChildrenM.clear();
        vChildrenM.swap(vSortedChildren);

        // Now sort the children of this
        for (u32 i = 0; i < count; i++) {
            Get(*vChildrenM[i]).SortChildren();
        }
    }

private:

    ContextExtra()
        : hasBeenPrintedM(false)
    {
    }

    // c1 should come before c2 if:
    // a. Any of c2's bases or parent is c1
    // b. Recursively, any of c2's children's bases or parent is c1
    // c. Recursively, any of (a) or (b) is true for c2 and any of c1's
    //    children
    bool ShouldComeBefore(const Context *pC1, const Context *pC2)
    {
        // If pC2 has as a parent pC1, then return true
        const Context *pParent = pC2->GetContext();
        if (pC1 == pParent) {
            return true;
        }
        
        // If pC2 has as a base pC1, then return true
        if ((pC2->GetType() == Context::Type_Class) ||
            (pC2->GetType() == Context::Type_Class) ||
            (pC2->GetType() == Context::Type_Union)) {
            const Structure *pS2 = (const Structure *) pC2;
            u32 count = pS2->GetBaseCount();

            for (u32 i = 0; i < count; i++) {
                const Structure &baseStruct = pS2->GetBase(i).GetStructure();
                if (pC1 == ((const Context *) &baseStruct)) {
                    return true;
                }
            }
        }

        // Recursively for pC1 and all children of pC2
        {
            ContextExtra &ce = ContextExtra::Get(*pC2);

            u32 count = ce.GetChildCount();
        
            for (u32 i = 0; i < count; i++) {
                const Context &child = ce.GetChild(i);
                if (ShouldComeBefore(pC1, &child)) {
                    return true;
                }
            }
        }

        // Recursively for pC2 and all children of pC1
        {
            ContextExtra &ce = ContextExtra::Get(*pC1);
            
            u32 count = ce.GetChildCount();
            
            for (u32 i = 0; i < count; i++) {
                const Context &child = ce.GetChild(i);
                if (ShouldComeBefore(&child, pC2)) {
                    return true;
                }
            }
        }

        return false;
    }

    vector<const Context *> vChildrenM;
    vector<const X_Enumeration *> vEnumerationsM;
    bool hasBeenPrintedM;

    static map<string, ContextExtra *> htContextExtrasG;
};

map<string, ContextExtra *> ContextExtra::htContextExtrasG;

class FunctionExtra
{
public:
    
    static FunctionExtra &Get(const MethodSignature &sig)
    {
        map<const MethodSignature *, FunctionExtra *>::iterator iter =
            htFunctionExtrasG.find(&sig);
        if (iter != htFunctionExtrasG.end()) {
            return *(iter->second);
        }

        FunctionExtra *pFunctionExtra = new FunctionExtra();

        htFunctionExtrasG[&sig] = pFunctionExtra;

        return *pFunctionExtra;
    }

    u32 GetNumber()
    {
        return numberM;
    }

    bool HasBeenPrinted()
    {
        return hasBeenPrintedM;
    }

    void SetHasBeenPrinted()
    {
        hasBeenPrintedM = true;
    }

private:

    FunctionExtra()
        : numberM(nextNumberG++), hasBeenPrintedM(false)
    {
    }

    u32 numberM;
    bool hasBeenPrintedM;

    static u32 nextNumberG;
    static map<const MethodSignature *, FunctionExtra *> htFunctionExtrasG;

};

u32 FunctionExtra::nextNumberG = 1;
map<const MethodSignature *, FunctionExtra *> FunctionExtra::htFunctionExtrasG;

static void SortIntoByName(const Context &context, 
                           vector<const Context *> &vContexts)
{
    const Context *pContext = &context;

    // Find where the context goes
    u32 count = vContexts.size();

    for (u32 i = 0; i < count; i++) {
        const Context *pExisting = vContexts[i];

        // If pExisting comes after pContext, then insert pContext here
        if (string(pExisting->GetName()) > string(pContext->GetName())) {
            vContexts.insert(vContexts.begin() + i, pContext);
            return;
        }
    }

    // Must go at the end
    vContexts.push_back(pContext);
}

static void AddToHierarchy(const Context &context)
{
    // Add this to the list of children of the parent context
    const Context *pParent = context.GetContext();

    // Ignore the root context
    if (!pParent) {
        return;
    }

    ContextExtra &ceParent = ContextExtra::Get(*pParent);

    ceParent.AddChild(context);
}

static void CollectEnumerations(const Context &context)
{
    ContextExtra &ce = ContextExtra::Get(context);

    // Do the sub-contexts
    u32 count = ce.GetChildCount();

    for (u32 i = 0; i < count; i++) {
        CollectEnumerations(ce.GetChild(i));
    }

    // Now look at each field of this context, if it has fields, and
    // see if there is an enumeration type to collect
    if ((context.GetType() != Context::Type_Class) &&
        (context.GetType() != Context::Type_Struct) &&
        (context.GetType() != Context::Type_Union)) {
        return;
    }

    const Structure &structure = (const Structure &) context;

    count = structure.GetFieldCount();

    for (u32 i = 0; i < count; i++) {
        const Type &type = structure.GetField(i).GetType();

        if (type.GetBaseType() != Type::BaseType_Enumeration) {
            continue;
        }

        const X_Enumeration &enumeration = ((const TypeEnumeration &) type).
            GetEnumeration();

        ContextExtra &ceEnum = ContextExtra::Get(enumeration.GetContext());

        ceEnum.AddEnumeration(enumeration);
    }
}

static bool IsInAnonymousChain(const Structure &structure)
{
    if (structure.IsAnonymous()) {
        return true;
    }

    const Context *pParent = structure.GetContext();

    if (pParent == NULL) {
        return false;
    }

    if ((pParent->GetType() != Context::Type_Class) &&
        (pParent->GetType() != Context::Type_Struct) &&
        (pParent->GetType() != Context::Type_Union)) {
        return false;
    }
    
    const Structure &parentStructure = (const Structure &) *pParent;

    return IsInAnonymousChain(parentStructure);
}

static Access PrintAccess(OutputIndentStream &out, AccessType accessType,
                          Access access)
{
	const char *str = 0;

    switch (accessType) {
    case AccessType_Public:
        if (access != Public) {
			access = Public;
			str = "public:";
        }
        break;
    case AccessType_Protected:
        if (access != Protected) {
			access = Protected;
			str = "protected:";
        }
        break;
    default: // AccessType_Private
        if (access != Private) {
			access = Private;
			str = "private:";
        }
        break;
    }

	if (str) {
		bool hasIndent = out.getIndent();
		hasIndent = false;
		if (hasIndent) {
			out.deindent();
		}
		out.WriteLine(str);
		if (hasIndent) {
			out.indent();
		}
	}

    return access;
}

static void PrintFunctionTypedefs(OutputIndentStream &out,
                                  const Context &context, bool contextSet)
{
    ContextExtra &ce = ContextExtra::Get(context);

    // Do the sub-contexts
    u32 count = ce.GetChildCount();

    for (u32 i = 0; i < count; i++) {
        PrintFunctionTypedefs(out, ce.GetChild(i), contextSet);
    }

    // Non-Structure Contexts cannot have fields, so skip them
    if ((context.GetType() != Context::Type_Class) &&
        (context.GetType() != Context::Type_Struct) &&
        (context.GetType() != Context::Type_Union)) {
        return;
    }

    const Structure &structure = (const Structure &) context;

    count = structure.GetFieldCount();

    for (u32 i = 0; i < count; i++) {
        const Type &type = structure.GetField(i).GetType();

        if (type.GetBaseType() != Type::BaseType_Function) {
            continue;
        }

        const MethodSignature &sig = 
            ((const TypeFunction &) type).GetSignature();

        FunctionExtra &fe = FunctionExtra::Get(sig);

        if (fe.HasBeenPrinted()) {
            continue;
        }

        // Print the typedef of it
        out.Write("typedef ");
        PrintTypePrefix(out, sig.GetReturnType(), contextSet, true);
        out.WriteF("(function_%lu) ", fe.GetNumber());
        PrintConstructorSignatureSuffix(out, sig, 0, contextSet);
        out.WriteLine(";");
    }
}

static void PrintChildrenContexts(OutputIndentStream &out,
                                  const Context &context, bool contextSet)
{
    ContextExtra &ce = ContextExtra::Get(context);

    u32 count = ce.GetChildCount();

    for (u32 i = 0; i < count; i++) {
		const Context &childContext = ce.GetChild(i);
		// Don't print out anonymous children contexts (which start with '<');
		// these will be printed with the fields whose type is the context
		if (childContext.GetName()[0] != '<') {
			PrintContext(out, childContext, contextSet);
		}
    }
}

static void PrintNamespace(OutputIndentStream &out,
                           const Namespace &namespaceRef, bool contextSet)
{
    // If the parent is a namespace, print it first
    const Context *pParent = namespaceRef.GetContext();
    if (pParent) {
        PrintContext(out, *pParent, contextSet);
    }

    ContextExtra &ce = ContextExtra::Get(namespaceRef);

    if (ce.HasBeenPrinted()) {
        return;
    }

    ce.SetHasBeenPrinted();

    // Skip it if it's the root namespace
    if (!namespaceRef.GetContext()) {
        return;
    }

    out.WriteLineF("namespace %s", namespaceRef.GetName());

    out.WriteLine("{");

    out.indent();

    // Print the enumerations
    u32 count = ce.GetEnumerationCount();
    
    for (u32 i = 0; i < count; i++) {
        PrintEnumeration(out, ce.GetEnumeration(i));
    }

	PrintChildrenContexts(out, namespaceRef, contextSet);

    out.deindent();

    out.WriteLine("}");
}

// This is only called on anonymous structs to in-place print out types
static void PrintStructure(OutputIndentStream &out,
                           const Structure &structureRef, bool contextSet)
{
    // If the parent is a namespace, print it first
    const Context *pParent = structureRef.GetContext();
    if (pParent) {
        PrintContext(out, *pParent, contextSet);
    }

    ContextExtra &ce = ContextExtra::Get(structureRef);

	if (ce.HasBeenPrinted()) {
        return;
    }

    ce.SetHasBeenPrinted();

    if (structureRef.IsIncomplete()) {
        out.WriteLineF("struct %s;\n", structureRef.GetName());
        return;
    }

    Access access;

	if (structureRef.GetName()[0] == '<') {
        if (structureRef.GetType() == Context::Type_Class) {
            access = Private;
            out.Write("class");
        }
        else if (structureRef.GetType() == Context::Type_Struct) {
            access = Public;
            out.Write("struct");
        }
        else { // Context::Type_Union
            access = Public;
            out.Write("union");
        }
    }
    else {
        if (structureRef.GetType() == Context::Type_Class) {
            access = Private;
            out.WriteF("class %s", structureRef.GetName());
        }
        else if (structureRef.GetType() == Context::Type_Struct) {
            access = Public;
            out.WriteF("typedef struct %s", structureRef.GetName());
        }
        else { // Context::Type_Union
            access = Public;
            out.WriteF("typedef union %s", structureRef.GetName());
        }
    }

    // bases
    u32 count = structureRef.GetBaseCount();

    if (count) {
        out.Write(" : ");
        for (u32 i = 0; i < count; i++) {
			const Base &base = structureRef.GetBase(i);
			const Structure &baseStructure = base.GetStructure();
			switch (base.GetAccessType()) {
			case AccessType_Public:
				out.Write("public ");
				break;
			case AccessType_Protected:
				out.Write("protected ");
				break;
			default:
				out.Write("private ");
				break;
			}
			if (base.IsVirtual()) {
				out.Write("virtual ");
			}
            out.WriteF("%s", baseStructure.GetName());
            if (i < (count - 1)) {
                out.Write(", ");
            }
        }
    }
    
	out.WriteLine();

    out.WriteLine("{");

    out.indent();

    // Print the enumerations
    count = ce.GetEnumerationCount();
    
    for (u32 i = 0; i < count; i++) {
        const X_Enumeration &enumeration = ce.GetEnumeration(i);
		out.deindent();
        access = PrintAccess(out, enumeration.GetAccessType(), access);
		out.indent();
        PrintEnumeration(out, enumeration);
    }

    out.deindent();

    count = ce.GetChildCount();

    for (u32 i = 0; i < count; i++) {
        const Context &child = ce.GetChild(i);

        if ((child.GetType() == Context::Type_Class) ||
            (child.GetType() == Context::Type_Struct) ||
            (child.GetType() == Context::Type_Union)) {
            const Structure &structure = * (const Structure *) & child;

            // If it's anonymous, don't print it out; it will be printed
            // out with the field that it defines
            if (structure.GetName()[0] == '<') {
                continue;
            }
            
            access = PrintAccess(out, structure.GetAccessType(), access);
        } 
        
        out.indent();
    
        PrintContext(out, child, contextSet);

        out.deindent();
    }

    out.indent();

    if (structureRef.GetType() != Context::Type_Union) {
        const Struct &structRef = (const Struct &) structureRef;

        // Now print the constructors and destructor of the struct - but only
        // if this is not anonymous
        if (!IsInAnonymousChain(structureRef)) {
            count = structRef.GetConstructorCount();

            for (u32 i = 0; i < count; i++) {
                const Constructor &constructor = structRef.GetConstructor(i);
				out.deindent();
                access = PrintAccess(out, constructor.GetAccessType(), access);
				out.indent();
                out.WriteF("%s", structureRef.GetName());
                PrintConstructorSuffix(out, constructor, contextSet);
            }

            if (structRef.HasDestructor()) {
                const Destructor &destructor = structRef.GetDestructor();
				out.deindent();
                access = PrintAccess(out, destructor.GetAccessType(), access);
				out.indent();
                if (destructor.IsVirtual()) {
                    out.WriteF("virtual ");
                }
                out.WriteF("~%s()", structureRef.GetName());
                if (destructor.IsPureVirtual()) {
                    out.WriteF(" = 0");
                }
                out.WriteLineF(";");
            }
        }

        // Now print the methods of the struct
        count = structRef.GetMethodCount();

        for (u32 i = 0; i < count; i++) {
            const Method &method = structRef.GetMethod(i);
			out.deindent();
            access = PrintAccess(out, method.GetAccessType(), access);
			out.indent();
            PrintMethod(out, method, contextSet);
        }
    }
    
    // Now print the fields of the structure
    count = structureRef.GetFieldCount();

    for (u32 i = 0; i < count; i++) {
        const Field &field = structureRef.GetField(i);
		out.deindent();
        access = PrintAccess(out, field.GetAccessType(), access);
		out.indent();
        PrintField(out, field, contextSet);
    }

	// Finally, print friends
	count = structureRef.GetFriendCount();

	for (u32 i = 0; i < count; i++) {
		const Structure &friendStructure = structureRef.GetFriend(i);
		out.WriteLineF("friend class ::%s;", friendStructure.GetFullName());
	}

    out.deindent();

	if (structureRef.GetName()[0] == '<') {
        out.Write("}");
    }
    else {
        if (structureRef.GetType() == Context::Type_Class) {
            out.WriteLine("};");
        }
        else {
            out.WriteLineF("} %s;", structureRef.GetName());
        }
    }
}

static void PrintContext(OutputIndentStream &out, const Context &context,
                         bool contextSet)
{
    switch (context.GetType()) {
    case Context::Type_Class:
		PrintStructure(out, (const Class &) context, contextSet);
        break;
    case Context::Type_Namespace:
        PrintNamespace(out, (const Namespace &) context, contextSet);
        break;
    case Context::Type_Struct:
		PrintStructure(out, (const Struct &) context, contextSet);
		break;
    case Context::Type_Union:
		PrintStructure(out, (const Union &) context, contextSet);
        break;
    }
}

static void PrintField(OutputIndentStream &out, const Field &field,
                       bool contextSet)
{
    if (field.IsStatic()) {
        out.Write("static ");
    }

    // Print the type prefix
    PrintTypePrefix(out, field.GetType(), contextSet, true);

    if (*(field.GetName())) {
        out.WriteF("%s", field.GetName());
    }
    
    PrintTypeSuffix(out, field.GetType());

    u32 bitfieldCount = field.GetBitfieldBitCount();
    if (bitfieldCount) {
        out.WriteF(":%lu", bitfieldCount);
    }

    out.WriteLine(";");
}

static void PrintConstructorSuffix(OutputIndentStream &out, 
                                   const Constructor &constructor,
                                   bool contextSet)
{
    vector<const char *> vArgumentNames;
    
    u32 count = constructor.GetSignature().GetArgumentCount();

    vArgumentNames.resize(count);
    for (u32 i = 0; i < count; i++) {
        vArgumentNames[i] = constructor.GetArgumentName(i);
    }
    
    PrintConstructorSignatureSuffix(out, constructor.GetSignature(), 
                                    &vArgumentNames, contextSet);
    
    count = constructor.GetSignature().GetThrowCount();

    if (count > 0) {
        out.Write(" throw(");
        for (u32 i = 0; i < count; i++) {
            PrintTypePrefix(out, constructor.GetSignature().GetThrow(i),
                            contextSet, false);
            if (i < (count - 1)) {
                out.Write(", ");
            }
        }
        out.Write(")");
    }

    out.WriteLine(";");
}

static void PrintMethod(OutputIndentStream &out, const Method &method,
                        bool contextSet)
{
    if (method.IsStatic()) {
        out.Write("static ");
    }

    if (method.IsVirtual()) {
        out.Write("virtual ");
    }

    vector<const char *> vArgumentNames;
    
    u32 count = method.GetSignature().GetArgumentCount();

    vArgumentNames.resize(count);
    for (u32 i = 0; i < count; i++) {
        vArgumentNames[i] = method.GetArgumentName(i);
    }

    // Print the return type
    PrintTypePrefix(out, method.GetSignature().GetReturnType(),
					contextSet, true);

    out.WriteF("%s", method.GetName());

    PrintConstructorSignatureSuffix(out, method.GetSignature(), 
                                    &vArgumentNames, contextSet);

    count = method.GetSignature().GetThrowCount();

    if (count > 0) {
        out.Write(" throw(");
        for (u32 i = 0; i < count; i++) {
            PrintTypePrefix(out, method.GetSignature().GetThrow(i),
                            contextSet, false);
            if (i < (count - 1)) {
                out.Write(", ");
            }
        }
        out.Write(")");
    }

    if (method.IsConst()) {
        out.Write(" const");
    }

    if (method.IsPureVirtual()) {
        out.Write(" = 0");
    }

    out.WriteLine(";");
}

static void PrintEnumeration(OutputIndentStream &out,
                             const X_Enumeration &enumeration)
{
    out.WriteLineF("enum %s {", enumeration.GetName());
    out.indent();
    u32 count = enumeration.GetValueCount();
    for (u32 i = 0; i < count; i++) {
        const EnumerationValue &value = enumeration.GetValue(i);
        out.WriteF("%s = %ld", value.GetName(), value.GetValue());
        if (i < (count - 1)) {
            out.Write(",");
        }
        out.WriteLine();
    }
    out.deindent();
    out.WriteLine("};");
}

#if 0
static void PrintValue(OutputIndentStream &out, const Type &type, 
                       const void *pValue)
{
    if (type.GetArrayOrPointerCount()) {
        switch (type.GetBaseType()) {
        case Type::BaseType_Char:
        case Type::BaseType_Unsigned_Char:
            out.WriteF("\"%s\"", * (const wchar_t **) pValue);
            break;
        case Type::BaseType_WChar:
            out.WriteF("\"%s\"", * (const wchar_t **) pValue);
            break;
        default:
            out.WriteF("%p", * (const char **) pValue);
            break;
        }
        return;
    }

    switch (type.GetBaseType()) {
    case Type::BaseType_Bool:
        out.Write(* (bool *) pValue ? "true" : "false");
        break;
    case Type::BaseType_Char:
    case Type::BaseType_Unsigned_Char:
        out.WriteF("'%c'", * (char *) pValue);
        break;
    case Type::BaseType_WChar:
    case Type::BaseType_Short:
        out.WriteF("%d", * (short *) pValue);
        break;
    case Type::BaseType_Unsigned_Short:
        out.WriteF("%u", * (unsigned short *) pValue);
        break;
    case Type::BaseType_Int:
        out.WriteF("%ld", * (int *) pValue);
        break;
    case Type::BaseType_Unsigned_Int:
        out.WriteF("%lu", * (unsigned int *) pValue);
        break;
    case Type::BaseType_Long:
        out.WriteF("%ld", * (long *) pValue);
        break;
    case Type::BaseType_Unsigned_Long:
        out.WriteF("%lu", * (unsigned long *) pValue);
        break;
    case Type::BaseType_Long_Long:
        out.WriteF("%lld", * (long long *) pValue);
        break;
    case Type::BaseType_Unsigned_Long_Long:
        out.WriteF("%llu", * (unsigned long long *) pValue);
        break;
    case Type::BaseType_Float:
        out.WriteF("%f", * (float *) pValue);
        break;
    case Type::BaseType_Double:
        out.WriteF("%g", * (double *) pValue);
        break;
    case Type::BaseType_Long_Double:
        out.WriteF("%lg", * (long double *) pValue);
    case Type::BaseType_Enumeration:
        out.WriteF("%d", * (int *) pValue);
        break;
    case Type::BaseType_Function:
        {
            FunctionExtra &fe = FunctionExtra::Get
                (((const TypeFunction &) type).GetSignature());
            
            out.WriteF("function_%lu", fe.GetNumber());
        }
        break;
        // I don't think these makes sense ...
    case Type::BaseType_Void:
    default: // BaseType_Structure: 
        out.Write("XXX-UNIMPLEMENTED-XXX");
        break;
    }
}
#endif

static void PrintTypePrefix(OutputIndentStream &out, const Type &type,
                            bool contextSet, bool space)
{
    string baseType;

    switch (type.GetBaseType()) {
    case Type::BaseType_Void:
        baseType = "void";
        break;
    case Type::BaseType_Bool:
        baseType = "bool";
        break;
    case Type::BaseType_Char:
        baseType = "char";
        break;
    case Type::BaseType_Unsigned_Char:
        baseType = "unsigned char";
        break;
    case Type::BaseType_WChar:
        baseType = "wchar_t";
        break;
    case Type::BaseType_Short:
        baseType = "short";
        break;
    case Type::BaseType_Unsigned_Short:
        baseType = "unsigned short";
        break;
    case Type::BaseType_Int:
        baseType = "int";
        break;
    case Type::BaseType_Unsigned_Int:
        baseType = "unsigned int";
        break;
    case Type::BaseType_Long:
        baseType = "long";
        break;
    case Type::BaseType_Unsigned_Long:
        baseType = "unsigned long";
        break;
    case Type::BaseType_Long_Long:
        baseType = "long long";
        break;
    case Type::BaseType_Unsigned_Long_Long:
        baseType = "unsigned long long";
        break;
    case Type::BaseType_Float:
        baseType = "float";
        break;
    case Type::BaseType_Double:
        baseType = "double";
        break;
    case Type::BaseType_Long_Double:
        baseType = "long double";
        break;
    case Type::BaseType_Enumeration:
        baseType = ((const TypeEnumeration &) type).GetEnumeration().GetName();
        break;
    case Type::BaseType_Function:
        {
            FunctionExtra &fe = FunctionExtra::Get
                (((const TypeFunction &) type).GetSignature());
            
            baseType = StringUtils::ToString
                (StringUtils::Format(L"function_%lu", fe.GetNumber()));
        }
        break;
    default: { // BaseType_Structure: 
        const Structure &structure = 
            ((const TypeStructure &) type).GetStructure();

		if (structure.GetName()[0] == '<') {
            // Write the type out on the spot
            PrintStructure(out, (const Struct &) structure, contextSet);
        }
        else {
			baseType = structure.GetName();
        }
    }
        break;
    }

    string typestr;

    if (type.IsReference()) {
        typestr = " &";
    }

    u32 count = type.GetArrayOrPointerCount();
    if (count) for (u32 i = 0; i < count; i++) {
        string p;
        // The first pointer ArrayOrPointer is special in that its const
        // or volatile indicators come before the base type
        const ArrayOrPointer &arrayOrPointer = type.GetArrayOrPointer(i);
        if (arrayOrPointer.GetType() == ArrayOrPointer::Type_Pointer) {
            const Pointer &pointer = (const Pointer &) arrayOrPointer;
            if (pointer.IsConst()) {
                if (pointer.IsVolatile()) {
                    p = "const volatile ";
                }
                else {
                    p = "const ";
                }
            }
            else if (pointer.IsVolatile()) {
                p = "volatile ";
            }

            if (i == (count - 1)) {
                typestr = p + baseType + " *" + typestr;
            }
            else {
                typestr = p + "*" + typestr;
            }
        }
    }
    else if (type.IsConst()) {
        if (type.IsVolatile()) {
            typestr = "const volatile " + baseType + typestr;
        }
        else {
            typestr = "const " + baseType + typestr;
        }
    }
    else if (type.IsVolatile()) {
        typestr = "volatile " + baseType + typestr;
    }
    else {
        typestr = baseType + typestr;
    }

    char e = typestr.c_str()[typestr.length() - 1];
    out.WriteF("%s%s", typestr.c_str(),
               (space && (e != '&') && (e != '*')) ? " " : "");
}

static void PrintTypeSuffix(OutputIndentStream &out, const Type &type)
{
    string typestr;

    u32 count = type.GetArrayOrPointerCount();
    for (u32 i = 0; i < count; i++) {
        const ArrayOrPointer &arrayOrPointer = type.GetArrayOrPointer(i);
        
        if (arrayOrPointer.GetType() == ArrayOrPointer::Type_Array) {
            const Array &array = (const Array &) arrayOrPointer;
            if (array.IsUnbounded()) {
                typestr += "[]";
            }
            else {
                typestr += StringUtils::ToString
                    (StringUtils::Format(L"[%lu]", array.GetElementCount()));
            }
        }
    }

    out.Write(typestr);
}

static void PrintConstructorSignatureSuffix(OutputIndentStream &out, 
                                            const ConstructorSignature &sig,
                                            vector<const char *> *vArgNames,
                                            bool contextSet)
{
    out.Write("(");

    u32 count = sig.GetArgumentCount();

    for (u32 i = 0; i < count; i++) {
        const Argument &arg = sig.GetArgument(i);
        PrintTypePrefix(out, arg.GetType(), contextSet, true);
        if (vArgNames) {
            out.WriteF("%s", (*vArgNames)[i]);
        }
        if (arg.HasDefault()) {
			const Type &argType = arg.GetType();
			const char *quote = "";
			if (argType.GetBaseType() == Type::BaseType_Char) {
				quote = ((argType.GetArrayOrPointerCount() == 0) ? "'" :
						 (argType.GetArrayOrPointerCount() == 1) ? "\"" : "");
			}

            out.WriteF(" = %s%s%s", quote, arg.GetDefault(), quote);
        }
        if (i < (count - 1)) {
            out.Write(", ");
        }
    }

    out.Write(")");
}

void XrttiToCpp(FILE *file)
{
    u32 count = GetContextCount();

    // First create a name-sorted list of contexts
    vector<const Context *> vNameSortedContexts;

    for (u32 i = 0; i < count; i++) {
        const Context *pContext = GetContext(i);
        SortIntoByName(*pContext, vNameSortedContexts);
    }

    // Now create a hierarchy of contexts
    for (u32 i = 0; i < count; i++) {
        AddToHierarchy(*(vNameSortedContexts[i]));
    }

    // Now sort the hierarchy - starting with the root namespace, sort the
    // children of each context successively
    const Context *pRoot = LookupContext("::");
    if (!pRoot) {
        // If there is no root, then no contexts have been registered and
        // there is nothing to do
        return;
    }
    ContextExtra &rootExtra = ContextExtra::Get(*pRoot);
    rootExtra.SortChildren();

    // Collect the child enumerations of each context
    CollectEnumerations(*pRoot);
    
    // Use an output indent stream
    OutputIndentStream out(file);
    
    // First print out any function typedefs
    PrintFunctionTypedefs(out, *pRoot, false);

    // Now print the hierarchy, starting with children of the root node
    PrintChildrenContexts(out, *pRoot, false);
}


void XrttiToCpp(const ContextSet &set, FILE *file)
{
    u32 count = set.GetContextCount();

    // First create a name-sorted list of contexts
    vector<const Context *> vNameSortedContexts;

    for (u32 i = 0; i < count; i++) {
        const Context *pContext = set.GetContext(i);
        SortIntoByName(*pContext, vNameSortedContexts);
    }

    // Now create a hierarchy of contexts
    for (u32 i = 0; i < count; i++) {
        AddToHierarchy(*(vNameSortedContexts[i]));
    }

    // Now sort the hierarchy - starting with the root namespace, sort the
    // children of each context successively
    const Context *pRoot = set.LookupContext("::");
    if (!pRoot) {
        // If there is no root, then no contexts have been registered and
        // there is nothing to do
        return;
    }
    ContextExtra &rootExtra = ContextExtra::Get(*pRoot);
    rootExtra.SortChildren();

    // Collect the child enumerations of each context
    CollectEnumerations(*pRoot);
    
    // Use an output indent stream
    OutputIndentStream out(file);
    
    // First print out any function typedefs
    PrintFunctionTypedefs(out, *pRoot, true);

    // Now print the hierarchy, starting with children of the root node
    PrintChildrenContexts(out, *pRoot, true);
}
