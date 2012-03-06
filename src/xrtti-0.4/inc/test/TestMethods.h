/*****************************************************************************\
 *                                                                           *
 * TestMethods.h                                                             *
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


#ifndef TEST_METHODS_H
#define TEST_METHODS_H

#include <inttypes.h>
#include <stdio.h>

typedef int16_t               s16;      /**< 16-bit signed */
typedef int32_t               s32;      /**< 32-bit signed */
typedef uint32_t              u32;      /**< 32-bit unsigned */


class XrttiAccess;

// Defines a bunch of methods that we can test invocation with
class TestMethods
{
public:

    typedef struct Inner
    {
        u32 a;
        float b;
    } Inner;

    // Default constructor
    TestMethods()
        : counterM(1)
    {
        innerM.a = 1;
        innerM.b = 2.0;
        printf("TestMethods constructor\n");
    }

    // Default destructor
    ~TestMethods()
    {
        printf("TestMethods destructor\n");
    }

    void Identify()
    {
        printf("TestMethods Identify\n");
    }

    s16 Count()
    {
        return counterM++;
    }

    s32 Sum(s32 arg1, s32 arg2) const
    {
        return arg1 + arg2;
    }

    s32 *Allocate(u32 arg) const
    {
        s32 *pRet = new s32[arg];

        for (u32 i = 0; i < arg; i++) {
            pRet[i] = i;
        }

        return pRet;
    }

    const Inner &GetInnerRef() const
    {
        return innerM;
    }

    const Inner GetInnerCopy() const
    {
        return innerM;
    }

    void TestParams(int a, float &b, char *c, Inner d, Inner &e, Inner *f)
    {
        printf("TestMethods TestParams a: %d, b: %f, c: %s, d.a: %d, "
               "d.b: %f, e.a: %d, e.b: %f, f->a: %d, f->b: %f\n",
               a, b, c, d.a, d.b, e.a, e.b, f->a, f->b);
    }
    
private:

    friend class ::XrttiAccess;

    s16 counterM;

    Inner innerM;

    Inner *pInnerM;
};

#endif // TEST_METHODS_H
