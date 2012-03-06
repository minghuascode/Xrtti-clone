/*****************************************************************************\
 *                                                                           *
 * GeneratorType.cpp                                                         *
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
#include <private/Generator.h>


using namespace std;

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


GeneratorType::GeneratorType(Generator &generator, const Type &type)
    : typedefEmittedM(false), typeM(type)
{
    generator.RegisterGeneratorType(&type, this);

    u32 count = type.GetArrayOrPointerCount();
    if (count) {
        vArrayOrPointersM.resize(count);
        for (u32 i = 0; i < count; i++) {
            const ArrayOrPointer &arrayOrPointer = type.GetArrayOrPointer(i);
            if (arrayOrPointer.GetType() == ArrayOrPointer::Type_Array) {
                vArrayOrPointersM[i] = new GeneratorArray
                    ((const Array &) arrayOrPointer);
            }
            else {
                vArrayOrPointersM[i] = new GeneratorPointer
                    ((const Pointer &) arrayOrPointer);
            }
        }
    }
}


GeneratorType::~GeneratorType()
{
	for (u32 i = 0; i < vArrayOrPointersM.size(); i++) {
		delete vArrayOrPointersM[i];
	}
}


// normal:      typedef <type> <typename>;
// function:    typedef <return_type> (<type> <typename>)(<argument types>);
void GeneratorType::EmitTypedef(FILE *file)
{
    // If typedef has already been emitted, then don't emit it
    if (typedefEmittedM) {
        return;
    }

    typedefEmittedM = true;

    this->EmitPrerequisiteTypedefs(file);
    
    fprintf(file, "    typedef ");

    this->EmitTypedefContents(file);

    fprintf(file, ";\n\n");
}

void GeneratorType::EmitTypedefContents(FILE *file)
{
    // typedef type_name
    fprintf(file, "%s %s", this->GetTypedefTypeName().c_str(),
            this->GetTypedefTypeDefinition().c_str());
}

string GeneratorType::GetTypedefTypeName()
{
    // We know that this is a basic type, not a function, enumeration, or
    // structure
    switch (typeM.GetBaseType()) {
    case Type::BaseType_Void:
        return "void";
    case Type::BaseType_Bool:
        return "bool";
    case Type::BaseType_Char:
        return "char";
    case Type::BaseType_Unsigned_Char:
        return "unsigned char";
    case Type::BaseType_WChar:
        return "wchar_t";
    case Type::BaseType_Short:
        return "short";
    case Type::BaseType_Unsigned_Short:
        return "unsigned short";
    case Type::BaseType_Int:
        return "int";
    case Type::BaseType_Unsigned_Int:
        return "unsigned int";
    case Type::BaseType_Long:
        return "long";
    case Type::BaseType_Unsigned_Long:
        return "unsigned long";
    case Type::BaseType_Long_Long:
        return "long long";
    case Type::BaseType_Unsigned_Long_Long:
        return "unsigned long long";
    case Type::BaseType_Float:
        return "float";
    case Type::BaseType_Double:
        return "double";
    default: // Type::BaseType_Long_Double:
        return "long double";
    }
}


void GeneratorType::EmitExtraDeclarations(FILE *file)
{
    u32 count = typeM.GetArrayOrPointerCount();
    for (u32 i = 0; i < count; i++) {
        vArrayOrPointersM[i]->EmitDeclaration(file);
    }
}


void GeneratorType::EmitExtraDefinitions(FILE *file)
{
    u32 count = typeM.GetArrayOrPointerCount();
    if (count) {
        for (u32 i = 0; i < count; i++) {
            vArrayOrPointersM[i]->EmitDefinition(file);
        }
        fprintf(file, "    static Xrtti::ArrayOrPointer *"
                "_%lu_array_or_pointers[] =\n    {\n", 
                (unsigned long) this->GetNumber());
        for (u32 i = 0; i < count; i++) {
            fprintf(file, "        &_::_%lu%s\n", 
                    (unsigned long) vArrayOrPointersM[i]->GetNumber(),
                    (i < (count - 1)) ? "," : "");
        }
        fprintf(file, "    };\n\n");
    }
}


void GeneratorType::EmitDefinitionArguments(FILE *file)
{
    fprintf(file, "        Xrtti::%s,\n", this->GetBaseTypeName());
    
    Generator::EmitBooleanArgument(file, typeM.IsConst(), true);

    Generator::EmitBooleanArgument(file, typeM.IsVolatile(), true);

    Generator::EmitBooleanArgument(file, typeM.IsReference(), true);

    Generator::EmitArrayArgument
        (file, typeM.GetArrayOrPointerCount(), "array_or_pointers", 
         this->GetNumber(), false);
}


string GeneratorType::GetTypedefTypeDefinition()
{
    // type_definition
    string tdef = StringUtils::ToString
        (StringUtils::Format(L"_%lu_type", (unsigned long) this->GetNumber()));

    // Don't include reference information; this is because we're just
    // typedef'ing enough for return type and argument type purposes, and
    // reference information is not included in that

    u32 count = typeM.GetArrayOrPointerCount();
    for (u32 i = 0; i < count; i++) {
        tdef = "(" + tdef + ")";
        const ArrayOrPointer &arrayOrPointer = typeM.GetArrayOrPointer(i);
        if (arrayOrPointer.GetType() == ArrayOrPointer::Type_Array) {
            const Array &array = (const Array &) arrayOrPointer;
            if (array.IsUnbounded()) {
                tdef = tdef + string("[]");
            }
            else {
                tdef = tdef + 
                    StringUtils::ToString
                    (StringUtils::Format
					 (L"[%lu]", (unsigned long) array.GetElementCount()));
            }
        }
        else {
            const Pointer &pointer = (const Pointer &) arrayOrPointer;
            if (pointer.IsVolatile()) {
                if (pointer.IsConst()) {
                    tdef = "const volatile *" + tdef;
                }
                else {
                    tdef = "volatile *" + tdef;
                }
            }
            else if (pointer.IsConst()) {
                tdef = "const *" + tdef;
            }
            else {
                tdef = " *" + tdef;
            }
        }
    }

    return tdef;
}

const char *GeneratorType::GetBaseTypeName()
{
    switch (typeM.GetBaseType()) {
    case Type::BaseType_Void:
        return "Type::BaseType_Void";
    case Type::BaseType_Bool:
        return "Type::BaseType_Bool";
    case Type::BaseType_Char:
        return "Type::BaseType_Char";
    case Type::BaseType_Unsigned_Char:
        return "Type::BaseType_Unsigned_Char";
    case Type::BaseType_WChar:
        return "Type::BaseType_WChar";
    case Type::BaseType_Short:
        return "Type::BaseType_Short";
    case Type::BaseType_Unsigned_Short:
        return "Type::BaseType_Unsigned_Short";
    case Type::BaseType_Int:
        return "Type::BaseType_Int";
    case Type::BaseType_Unsigned_Int:
        return "Type::BaseType_Unsigned_Int";
    case Type::BaseType_Long:
        return "Type::BaseType_Long";
    case Type::BaseType_Unsigned_Long:
        return "Type::BaseType_Unsigned_Long";
    case Type::BaseType_Long_Long:
        return "Type::BaseType_Long_Long";
    case Type::BaseType_Unsigned_Long_Long:
        return "Type::BaseType_Unsigned_Long_Long";
    case Type::BaseType_Float:
        return "Type::BaseType_Float";
    case Type::BaseType_Double:
        return "Type::BaseType_Double";
    case Type::BaseType_Long_Double:
        return "Type::BaseType_Long_Double";
    case Type::BaseType_Enumeration:
        return "Type::BaseType_Enumeration";
    case Type::BaseType_Function:
        return "Type::BaseType_Function";
    default: // BaseType_Structure
        return "Type::BaseType_Structure";
    }
}

string GeneratorType::GetCppTypeName()
{
    switch (typeM.GetBaseType()) {
    case Type::BaseType_Void:
        return "void";
    case Type::BaseType_Bool:
        return "bool";
    case Type::BaseType_Char:
        return "char";
    case Type::BaseType_Unsigned_Char:
        return "unsigned char";
    case Type::BaseType_WChar:
        return "wchar_t";
    case Type::BaseType_Short:
        return "short";
    case Type::BaseType_Unsigned_Short:
        return "unsigned short";
    case Type::BaseType_Int:
        return "int";
    case Type::BaseType_Unsigned_Int:
        return "unsigned int";
    case Type::BaseType_Long:
        return "long";
    case Type::BaseType_Unsigned_Long:
        return "unsigned long";
    case Type::BaseType_Long_Long:
        return "long long";
    case Type::BaseType_Unsigned_Long_Long:
        return "unsigned long long";
    case Type::BaseType_Float:
        return "float";
    case Type::BaseType_Double:
        return "double";
    case Type::BaseType_Long_Double:
        return "long double";
    case Type::BaseType_Enumeration:
        {
            const Enumeration &enumeration = 
                ((const TypeEnumeration &) typeM).GetEnumeration();
            return (string(enumeration.GetContext().GetFullName()) + 
                    "::" + enumeration.GetName());
        }
    case Type::BaseType_Function:
        return "todo";
    default: // BaseType_Structure
        return ((const TypeStructure &) typeM).GetStructure().GetFullName();
    }
}


}; // namespace Xrtti
