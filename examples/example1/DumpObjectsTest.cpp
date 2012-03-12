/*****************************************************************************\
 *                                                                           *
 * DumpObjectsTest.cpp                                                       *
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


// This is an example of using the DumpObjects example.  It defines a
// couple of classes, allocates some instances of them, and then calls
// DumpObjects().  Then it cleans up.

#include <typeinfo>
#include "DumpObjectsTest.h"

// External reference to the DumpObject method defined in DumpObject.cpp
extern void DumpObject(void *pObject, const std::type_info &typeinfo);

int main()
{
    // Create some objects
    SimpleClass **pSimpleClasses = new SimpleClass *[5];

    for (int i = 0; i < 5; i++) {
        pSimpleClasses[i] = new SimpleClass(i);
    }

    SimpleClassWithDumpedMethod **pSimpleClassesWithDumpedMethod =
        new SimpleClassWithDumpedMethod *[8];

    for (int i = 0; i < 8; i++) {
        pSimpleClassesWithDumpedMethod[i] = 
            new SimpleClassWithDumpedMethod(i, i + 100);
    }

    // Now dump them.  The DumpObject call doesn't know anything about the
    // specific types of any object, but can stil dump these objects because
    // it uses Xrtti to reflect on the type.
    for (int i = 0; i < 5; i++) {
        DumpObject(pSimpleClasses[i], typeid(*pSimpleClasses[i]));
    }
    
    for (int i = 0; i < 8; i++) {
        DumpObject(pSimpleClassesWithDumpedMethod[i], 
                   typeid(*pSimpleClassesWithDumpedMethod[i]));
    }

    // Now delete everything
    for (int i = 0; i < 8; i++) {
        delete pSimpleClassesWithDumpedMethod[i];
    }
    
    delete[] pSimpleClassesWithDumpedMethod;

    for (int i = 0; i < 5; i++) {
        delete pSimpleClasses[i];
    }
    
    delete[] pSimpleClasses;

    return 0;
}
