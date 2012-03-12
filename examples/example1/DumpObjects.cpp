/*****************************************************************************\
 *                                                                           *
 * DumpObjects.cpp                                                           *
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

// This is an example of using the Xrtti API.  The purpose of this code is to
// provide a method for dumping some basic information about all objects that
// a program allocates.
//
// Most uses of Xrtti would be from library code, or from object files meant
// to be compiled into many different executables.  This is because Xrtti is
// most useful as a means for writing generic algorithms that can process any
// object, given an exteded runtime type description of the object.
//
// There would be little point in using Xrtti in a program where you knew all
// of the types of all of the objects (although there are still times when
// this is useful - when you want to write generic class processing code that
// doesn't need to be updated as you add, remove, and change your classes) -
// Xrtti is best used to write algorithms that can process *any* classes,
// given their extended runtime type definitions.  So you would normally write
// code that uses Xrtti into a library or an object file meant to be linked
// into many different applications.

// For printf
#include <stdio.h>
// For strcmp
#include <string.h>
// This header file defines all of the Xrtti API
#include <Xrtti/Xrtti.h>

// Use the Xrtti namespace so we don't have to prepend Xrtti when referencing
// the Xrtti API
using namespace Xrtti;

typedef u32 U32;

// Dump a single object - the object MUST have a vtable!  In C++, Only objects
// with vtables can have their types dynamically detected at runtime.
void DumpObject(void *pObject, const std::type_info &typeinfo)
{
    // Get the extended runtime type information for this object.  This
    // assumes that 1) The Xrtti tables were generated for this class
    // using the -r option, which enables mappings from C++ rtti to Xrtti,
    // and that 2) C++ rtti was turned on for this program.
    const Structure *pStructure = LookupStructure(typeinfo);

    // If NULL is returned, then no Xrtti tables were generated for this
    // class
    if (pStructure == NULL) {
        return;
    }
    
    printf("\nAt %p: ", pObject);

    // It could be a class or struct
    if (pStructure->GetType() == Context::Type_Class) {
        printf("class ");
    }
    else {
        printf("struct ");
    }

    // Print the full class name
    printf("%s:\n", pStructure->GetFullName());

    // We could print out lots of detail about this class, such as the base
    // class types, complete set of methods and their types, constructors and
    // their types, all fields and their types, etc.  In fact the entire
    // contents of the class, including the value of every member, could be
    // printed out using the Xrtti API.  But for the purposes of this simple
    // example, we'll just print the fundamentally-typed fields one by one.
    U32 fieldCount = pStructure->GetFieldCount();
    for (U32 i = 0; i < fieldCount; i++) {
        const Field &field = pStructure->GetField(i);

        printf("    %s: ", field.GetName());

        // First off, skip pointer and array types, for the purposes of
        // this simple example.
        if (field.GetType().GetArrayOrPointerCount()) {
            printf("array or pointer\n");
            continue;
        }

        // Also, skip void, function, and structure types
        if (field.GetType().GetBaseType() == Type::BaseType_Void) {
            printf("void\n");
            continue;
        }
        if (field.GetType().GetBaseType() == Type::BaseType_Function) {
            printf("function\n");
            continue;
        }
        if (field.GetType().GetBaseType() == Type::BaseType_Structure) {
            printf("structure\n");
            continue;
        }

        // Get the value of the field
        void *pValue = field.Get(pObject);

        // Print out the field's type
        switch (field.GetType().GetBaseType()) {
        case Type::BaseType_Bool:
            printf("bool: %s\n", * (bool *) pValue);
            break;
        case Type::BaseType_Char:
            printf("char: %c\n", * (char *) pValue);
            break;
        case Type::BaseType_Unsigned_Char:
            printf("unsigned char: %uc\n", * (unsigned char *) pValue);
            break;
        case Type::BaseType_WChar:
            printf("wchar_t: %lc\n", * (wchar_t *) pValue);
            break;
        case Type::BaseType_Short:
            printf("short: %s\n", * (short *) pValue);
            break;
        case Type::BaseType_Unsigned_Short:
            printf("unsigned short: %us\n", * (unsigned short *) pValue);
            break;
        case Type::BaseType_Int:
            printf("int: %d\n", * (int *) pValue);
            break;
        case Type::BaseType_Unsigned_Int:
            printf("unsigned int: %ud\n", * (unsigned int *) pValue);
            break;
        case Type::BaseType_Long:
            printf("long: %ld\n", * (long *) pValue);
            break;
        case Type::BaseType_Unsigned_Long:
            printf("unsigned long: %uld\n", * (unsigned long *) pValue);
            break;
        case Type::BaseType_Long_Long:
            printf("long long: %ll\n", * (long long *) pValue);
            break;
        case Type::BaseType_Unsigned_Long_Long:
            printf("unsignd long long: %ull\n", 
                   * (unsigned long long *) pValue);
            break;
        case Type::BaseType_Float:
            printf("float: %f\n", * (float *) pValue);
            break;
        case Type::BaseType_Double:
            printf("double: %g\n", * (double *) pValue);
            break;
        default: // Type::BaseType_Long_Double:
            printf("long double: %lg\n", * (long double *) pValue);
            break;
        }
    }

    // If this structure is a union, then it cannot have any methods so
    // we're done with it
    if (pStructure->GetType() == Context::Type_Union) {
        return;
    }

    // This Xrtti::Structure is actually an Xrtti::Struct
    const Struct *pStruct = (const Struct *) pStructure;

    U32 methodCount = pStruct->GetMethodCount();
    for (U32 i = 0; i < methodCount; i++) {
        const Method &method = pStruct->GetMethod(i);
        // If the method is not named "Dumped", then we are not interested
        // in it
        if (strcmp(method.GetName(), "Dumped")) {
            continue;
        }

        // If the method has a non-void return type, then skip it
        if (method.GetSignature().GetReturnType().GetBaseType() != 
            Type::BaseType_Void) {
            continue;
        }

        // If the method has a "pointer count", which is basically the
        // number of asterisks in the type definition (i.e. void * would
        // have pointer count 1, void ** would have pointer count 2, etc),
        // then skip it
        if (method.GetSignature().GetReturnType().
            GetArrayOrPointerCount() > 0) {
            continue;
        }

        // If the method has arguments, skip it
        if (method.GetSignature().GetArgumentCount() > 0) {
            continue;
        }

        // OK, we now know that the signature of this method is:
        // void Dumped()
        // So, invoke it!
        method.Invoke(pObject, NULL, NULL);
    }
}
