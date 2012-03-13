/*****************************************************************************\
 *                                                                           *
 * DumpObjectsTest.h                                                         *
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


// This is an example of using the DumpObjects example.  This header file
// defines some classes to use in the test program.

// For printf
#include <stdio.h>

class XrttiAccess; // So that we can give our classes XrttiAccess friendship

struct ss1_s {
    int a, b;
};

class SimpleClass
{
public:

    SimpleClass(int value)
        : intM(value), floatM((float) value)
    {
    }

    // This class must have a vtable to be properly supported by C++ rtti
    virtual ~SimpleClass() { }

private:

    friend class ::XrttiAccess; // Required if we want these private members
                                // to be accessible via Xrtti

    int intM;
    float floatM;
    struct ss1_s aa;
    
    typedef struct ss2_s {
        int c, d;
    } ss2_t;
    ss2_t bb;
};

class SimpleClassWithDumpedMethod : public SimpleClass
{
public:

    SimpleClassWithDumpedMethod(int value1, long value2)
        : SimpleClass(value1), longM(value1), doubleM((double) value2)
    {
    }

private:

    friend class ::XrttiAccess; // Required if we want these private members
                                // to be accessible via Xrtti

    // This method will be called by the DumpObjects method; even though
    // it's a private method, the Xrtti API can invoke it
    void Dumped()
    {
        printf("Dumped called for %p\n", this);
    }

    long longM;
    double doubleM;
};
