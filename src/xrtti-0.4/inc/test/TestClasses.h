/*****************************************************************************\
 *                                                                           *
 * TestClasses.h                                                             *
 *                                                                           *
 * ------------------------------------------------------------------------- *
 * Copyright (C) 2007-2008 Bryan Ischo <bryan@ischo.com>                     *
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

#ifndef TEST_CLASSES_H
#define TEST_CLASSES_H

#include <stdarg.h>
#include <stdlib.h>
#include <string>
#include "test/Util.h"

using namespace std;
using namespace Xrtti;


class XrttiAccess;

// Convenience for setting a value to a random number
template<typename T>
inline void Rand(T &t)
{
    t = Random(0xFFFFFFFF);
}

// Convenience for setting a value to a random real number
template<typename T>
inline void RandReal(T &t)
{
    t = ((T) Random(0xFFFFFFFF)) / 
        ((T) Random(0xFFFFFFFF));

    if (Random(1)) {
        t = -t;
    }
}

// Convenience for setting a value to a random number, with a maximum
template<typename T>
inline void Rand(T &t, T max)
{
    t = Random(max);
}

// Convenience for setting an inline max-terminated array to random real 
// numbers
template<typename T>
inline void RandRealArray(T *pArray, u32 maxCount)
{
    for (u32 i = 0; i < maxCount; i++) {
        RandReal<T>(pArray[i]);
    }
}

// Convenience for setting a non-inline max-terminated array to random real 
// numbers
template<typename T>
inline void CreateRandRealArray(T *&pArray, u32 maxCount)
{
    pArray = new T[maxCount];

    for (u32 i = 0; i < maxCount; i++) {
        RandReal<T>(pArray[i]);
    }
}

// Convenience for setting an inline count-terminated array to random numbers
template<typename T, typename C>
inline void RandArray(T *pArray, C &count, u32 maxCount)
{
    count = Random(0xFFFFFFFF) % (maxCount + 1);
    
    for (u32 i = 0; i < (u32) count; i++) {
        Rand<T>(pArray[i]);
    }
}

// Convenience for setting an inline count-terminated array to random real 
// numbers
template<typename T, typename C>
inline void RandRealArray(T *pArray, C &count, u32 maxCount)
{
    count = Random(0xFFFFFFFF) % (maxCount + 1);
    
    for (u32 i = 0; i < (u32) count; i++) {
        RandReal<T>(pArray[i]);
    }
}

// Convenience for setting a non-inline count-terminated array to random 
// numbers
template<typename T, typename C>
inline void CreateRandArray(T *&pArray, C &count, u32 maxCount)
{
    count = Random(0xFFFFFFFF) % (maxCount + 1);

    pArray = new T[count];
    
    for (u32 i = 0; i < (u32) count; i++) {
        Rand<T>(pArray[i]);
    }
}

// Convenience for setting a non-inline count-terminated array to random real 
// numbers
template<typename T, typename C>
inline void CreateRandRealArray(T *&pArray, C &count, u32 maxCount)
{
    count = Random(0xFFFFFFFF) % (maxCount + 1);

    pArray = new T[count];

    for (u32 i = 0; i < (u32) count; i++) {
        RandReal<T>(pArray[i]);
    }
}

// Convenience for setting an inline zero-terminated array to random numbers
template<typename T>
inline void RandArray(T *pArray, u32 maxCount)
{
    u32 count = Random(0xFFFFFFFF) % (maxCount + 1);
    
    for (u32 i = 0; i < count; i++) {
        Rand<T>(pArray[i]);
    }

    if (count < maxCount) {
        pArray[count] = 0;
    }
}

// Convenience for setting a non-inline zero-terminated array to random numbers
template<typename T>
inline void CreateRandArray(T *&pArray, u32 maxCount)
{
    u32 count = Random(0xFFFFFFFF) % maxCount;
    
    pArray = new T[count + 1];
    
    for (u32 i = 0; i < (u32) count; i++) {
        Rand<T>(pArray[i]);
    }

    pArray[count] = 0;
}

inline void LogPrefix(FILE *file, const string &prefix, 
                      const char *format, ...)
{
    fprintf(file, prefix.c_str());

    va_list valist;

    va_start(valist, format);

    vfprintf(file, format, valist);

    va_end(valist);
}

class TestClass
{
public:

    static TestClass *Create(bool fixedId, u32 id);

    inline TestClass()
        : isLastM(false)
    {
    }
    
    inline TestClass(bool isLast)
        : isLastM(isLast)
    {
    }
    
    virtual ~TestClass() { }

    void SetIsLast(bool isLast)
    {
        isLastM = isLast;
    }

    bool IsLast()
    {
        return isLastM;
    }

    virtual void Randomize() { }

    virtual const char *GetIdentity() const { return "TestClass"; }

private:

    friend class ::XrttiAccess;

    bool isLastM;
};

// The purpose of these is to exercise every permutation of serialization

class BasicTypes : public TestClass
{
public:

    // The actual value of fRandomize does not matter
    virtual void Randomize()
    {
        Rand<s8>(s8v);
        Rand<u8>(u8v);
        Rand<s16>(s16v);
        Rand<u16>(u16v);
        Rand<s32>(s32v);
        Rand<u32>(u32v);
        Rand<s64>(s64v);
        Rand<u64>(u64v);
        RandReal<float>(flv);
        RandReal<double>(dlv);
    }

    virtual const char *GetIdentity() const
    {
        return "BasicTypes";
    }

private:

    friend class ::XrttiAccess;
    
    s8 s8v;

    u8 u8v;

    s16 s16v;

    u16 u16v;

    s32 s32v;

    u32 u32v;

    s64 s64v;

    u64 u64v;

    float flv;

    double dlv;
};

class BasicTypeArrays : public TestClass
{
public:
    
    // The actual value of fRandomize does not matter
    virtual void Randomize()
    {
        RandArray<s8>(s8v, countof(s8v));
        RandArray<u8>(u8v, countof(u8v));
        RandArray<s16>(s16v, countof(s16v));
        RandArray<u16>(u16v, countof(u16v));
        RandArray<s32>(s32v, countof(s32v));
        RandArray<u32>(u32v, countof(u32v));
        RandArray<s64>(s64v, countof(s64v));
        RandArray<u64>(u64v, countof(u64v));
        RandRealArray<float>(flv, countof(flv));
        RandRealArray<double>(dlv, countof(dlv));
    }

    virtual const char *GetIdentity() const
    {
        return "BasicTypeArrays";
    }

private:

    friend class ::XrttiAccess;

    s8 s8v[8];

    u8 u8v[8];

    s16 s16v[16];

    u16 u16v[16];

    s32 s32v[32];

    u32 u32v[32];

    s64 s64v[64];

    u64 u64v[64];

    float flv[10];

    double dlv[10];
};

class BasicTypeArraysWithCount : public TestClass
{
public:

    inline BasicTypeArraysWithCount()
        : s8v_CountM(0), u8v_CountM(0), s16v_CountM(0), u16v_CountM(0),
          s32v_CountM(0), u32v_CountM(0), s64v_CountM(0), u64v_CountM(0),
          flv_CountM(0), dlv_CountM(0)
    {
    }

    virtual void Randomize()
    {
        RandArray<s8, s8>(s8v, s8v_CountM, countof(s8v));
        RandArray<u8, u8>(u8v, u8v_CountM, countof(u8v));
        RandArray<s16, s16>(s16v, s16v_CountM, countof(s16v));
        RandArray<u16, u16>(u16v, u16v_CountM, countof(u16v));
        RandArray<s32, s32>(s32v, s32v_CountM, countof(s32v));
        RandArray<u32, u32>(u32v, u32v_CountM, countof(u32v));
        RandArray<s64, s32>(s64v, s64v_CountM, countof(s64v));
        RandArray<u64, u32>(u64v, u64v_CountM, countof(u64v));
        RandRealArray<float, u32>(flv, flv_CountM, countof(flv));
        RandRealArray<double, u32>(dlv, dlv_CountM, countof(dlv));
    }

    virtual const char *GetIdentity() const
    {
        return "BasicTypeArraysWithCount";
    }

    void bleh(int /* a */, char * /* c = "hello" */)
    {
    }

private:

    friend class ::XrttiAccess;

    s8 s8v_CountM;
    s8 s8v[8];

    u8 u8v_CountM; 
    u8 u8v[8];

    s16 s16v_CountM;
    s16 s16v[16];

    u16 u16v_CountM;
    u16 u16v[16];

    s32 s32v_CountM;
    s32 s32v[32];

    u32 u32v_CountM;
    u32 u32v[32];

    s32 s64v_CountM;
    s64 s64v[64];

    u32 u64v_CountM;
    u64 u64v[64];

    u32 flv_CountM;
    float flv[10];

    u32 dlv_CountM;
    double dlv[10];
};

class BasicTypePointersWithCount : public TestClass
{
public:

    inline BasicTypePointersWithCount()
        : ps8_CountM(0), ps8(NULL), pu8_CountM(0), pu8(NULL), 
          ps16_CountM(0), ps16(NULL), pu16_CountM(0), pu16(NULL), 
          ps32_CountM(0), ps32(NULL), pu32_CountM(0), pu32(NULL), 
          ps64_CountM(0), ps64(NULL), pu64_CountM(0), pu64(NULL), 
          pFl_CountM(0), pFl(NULL), pDl_CountM(0), pDl(NULL)
    {
    }

    virtual ~BasicTypePointersWithCount()
    {
        delete [] ps8;
        delete [] pu8;
        delete [] ps16;
        delete [] pu16;
        delete [] ps32;
        delete [] pu32;
        delete [] ps64;
        delete [] pu64;
        delete [] pFl;
        delete [] pDl;
    }

    virtual void Randomize()
    {
        CreateRandArray<s8, s8>(ps8, ps8_CountM, 100);
        CreateRandArray<u8, u8>(pu8, pu8_CountM, 200);
        CreateRandArray<s16, s16>(ps16, ps16_CountM, 50);
        CreateRandArray<u16, u16>(pu16, pu16_CountM, 75);
        CreateRandArray<s32, s32>(ps32, ps32_CountM, 123);
        CreateRandArray<u32, u32>(pu32, pu32_CountM, 321);
        CreateRandArray<s64, s32>(ps64, ps64_CountM, 1);
        CreateRandArray<u64, u32>(pu64, pu64_CountM, 6);
        CreateRandRealArray<float, u32>(pFl, pFl_CountM, 9);
        CreateRandRealArray<double, u32>(pDl, pDl_CountM, 333);
    }

    virtual const char *GetIdentity() const
    {
        return "BasicTypePointersWithCount";
    }

private:

    friend class ::XrttiAccess;

    s8 ps8_CountM;
    s8 *ps8;

    u8 pu8_CountM; 
    u8 *pu8;

    s16 ps16_CountM;
    s16 *ps16;

    u16 pu16_CountM;
    u16 *pu16;

    s32 ps32_CountM;
    s32 *ps32;

    u32 pu32_CountM;
    u32 *pu32;

    s32 ps64_CountM;
    s64 *ps64;

    u32 pu64_CountM;
    u64 *pu64;

    u32 pFl_CountM;
    float *pFl;

    u32 pDl_CountM;
    double *pDl;
};

class BasicTypePointersWithMax : public TestClass
{
public:

    inline BasicTypePointersWithMax()
        : ps8(NULL), pu8(NULL), ps16(NULL), pu16(NULL), ps32(NULL), pu32(NULL),
          ps64(NULL), pu64(NULL), pFl(NULL), pDl(NULL)
    {
    }

    virtual ~BasicTypePointersWithMax()
    {
        delete [] ps8;
        delete [] pu8;
        delete [] ps16;
        delete [] pu16;
        delete [] ps32;
        delete [] pu32;
        delete [] ps64;
        delete [] pu64;
        delete [] pFl;
        delete [] pDl;
    }

    virtual void Randomize()
    {
        CreateRandArray<s8>(ps8, ps8_Max);
        CreateRandArray<u8>(pu8, pu8_Max);
        CreateRandArray<s16>(ps16, ps16_Max);
        CreateRandArray<u16>(pu16, pu16_Max);
        CreateRandArray<s32>(ps32, ps32_Max);
        CreateRandArray<u32>(pu32, pu32_Max);
        CreateRandArray<s64>(ps64, ps64_Max);
        CreateRandArray<u64>(pu64, pu64_Max);
        CreateRandRealArray<float>(pFl, pFl_Max);
        CreateRandRealArray<double>(pDl, pDl_Max);
    }

    virtual const char *GetIdentity() const
    {
        return "BasicTypePointersWithMax";
    }

private:

    friend class ::XrttiAccess;

    static const int ps8_Max = 8;
    s8 *ps8;

    static const int pu8_Max = 8;
    u8 *pu8;

    static const int ps16_Max = 16;
    s16 *ps16;

    static const int pu16_Max = 16;
    u16 *pu16;

    static const int ps32_Max = 32;
    s32 *ps32;

    static const int pu32_Max = 32;
    u32 *pu32;

    static const int ps64_Max = 64;
    s64 *ps64;

    static const int pu64_Max = 64;
    u64 *pu64;

    static const int pFl_Max = 32;
    float *pFl;

    static const int pDl_Max = 64;
    double *pDl;
};

class BasicTypePointersWithCountAndMax : public TestClass
{
public:

    inline BasicTypePointersWithCountAndMax()
        : ps8_CountM(0), ps8(NULL), pu8_CountM(0), pu8(NULL), 
          ps16_CountM(0), ps16(NULL), pu16_CountM(0), pu16(NULL), 
          ps32_CountM(0), ps32(NULL), pu32_CountM(0), pu32(NULL), 
          ps64_CountM(0), ps64(NULL), pu64_CountM(0), pu64(NULL), 
          pFl_CountM(0), pFl(NULL), pDl_CountM(0), pDl(NULL)
    {
    }

    virtual ~BasicTypePointersWithCountAndMax()
    {
        delete [] ps8;
        delete [] pu8;
        delete [] ps16;
        delete [] pu16;
        delete [] ps32;
        delete [] pu32;
        delete [] ps64;
        delete [] pu64;
        delete [] pFl;
        delete [] pDl;
    }

    virtual void Randomize()
    {
        CreateRandArray<s8, s8>(ps8, ps8_CountM, ps8_Max);
        CreateRandArray<u8, u8>(pu8, pu8_CountM, pu8_Max);
        CreateRandArray<s16, s16>(ps16, ps16_CountM, ps16_Max);
        CreateRandArray<u16, u16>(pu16, pu16_CountM, pu16_Max);
        CreateRandArray<s32, s32>(ps32, ps32_CountM, ps32_Max);
        CreateRandArray<u32, u32>(pu32, pu32_CountM, pu32_Max);
        CreateRandArray<s64, s32>(ps64, ps64_CountM, ps64_Max);
        CreateRandArray<u64, u32>(pu64, pu64_CountM, pu64_Max);
        CreateRandRealArray<float, u32>(pFl, pFl_CountM, pFl_Max);
        CreateRandRealArray<double, u32>(pDl, pDl_CountM, pDl_Max);
    }

    virtual const char *GetIdentity() const
    {
        return "BasicTypePointersWithCountAndMax";
    }

private:

    friend class ::XrttiAccess;

    static const int ps8_Max = 8;
    s8 ps8_CountM;
    s8 *ps8;

    static const int pu8_Max = 8;
    u8 pu8_CountM; 
    u8 *pu8;

    static const int ps16_Max = 16;
    s16 ps16_CountM;
    s16 *ps16;

    static const int pu16_Max = 16;
    u16 pu16_CountM;
    u16 *pu16;

    static const int ps32_Max = 32;
    s32 ps32_CountM;
    s32 *ps32;

    static const int pu32_Max = 32;
    u32 pu32_CountM;
    u32 *pu32;

    static const int ps64_Max = 64;
    s32 ps64_CountM;
    s64 *ps64;

    static const int pu64_Max = 64;
    u32 pu64_CountM;
    u64 *pu64;

    static const int pFl_Max = 32;
    u32 pFl_CountM;
    float *pFl;

    static const int pDl_Max = 64;
    u32 pDl_CountM;
    double *pDl;
};

class Structs : public TestClass
{
public:

    virtual void Randomize()
    {
        basicTypes.Randomize();
        basicTypeArrays.Randomize();
        basicTypeArraysWithCount.Randomize();
        basicTypePointersWithMax.Randomize();
        basicTypePointersWithCountAndMax.Randomize();
    }

    virtual const char *GetIdentity() const
    {
        return "Structs";
    }

private:

    friend class ::XrttiAccess;

    BasicTypes basicTypes;

    BasicTypeArrays basicTypeArrays;

    BasicTypeArraysWithCount basicTypeArraysWithCount;

    BasicTypePointersWithMax basicTypePointersWithMax;

    BasicTypePointersWithCountAndMax basicTypePointersWithCountAndMax;
};

class StructArrays : public TestClass
{
public:

    virtual void Randomize()
    {
        for (u32 i = 0; i < countof(basicTypesArray); i++) {
            basicTypesArray[i].Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArraysArray); i++) {
            basicTypeArraysArray[i].Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCountArray); 
             i++) {
            basicTypeArraysWithCountArray[i].Randomize();
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMaxArray); 
             i++) {
            basicTypePointersWithMaxArray[i].Randomize();
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMaxArray); i++) {
            basicTypePointersWithCountAndMaxArray[i].Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructArrays";
    }

private:

    friend class ::XrttiAccess;
    
    BasicTypes basicTypesArray[1];

    BasicTypeArrays basicTypeArraysArray[2];

    BasicTypeArraysWithCount basicTypeArraysWithCountArray[4];

    BasicTypePointersWithMax basicTypePointersWithMaxArray[2];

    BasicTypePointersWithCountAndMax basicTypePointersWithCountAndMaxArray[1];
};

class StructPointers : public TestClass
{
public:

    inline StructPointers()
        : pBasicTypes(NULL), pBasicTypeArrays(NULL), 
          pBasicTypeArraysWithCount(NULL), pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax(NULL)
    {
    }

    virtual ~StructPointers()
    {
        delete pBasicTypes;
        delete pBasicTypeArrays;
        delete pBasicTypeArraysWithCount;
        delete pBasicTypePointersWithMax;
        delete pBasicTypePointersWithCountAndMax;
    }
    
    virtual void Randomize()
    {
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypes = new BasicTypes();
            pBasicTypes->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArrays = new BasicTypeArrays();
            pBasicTypeArrays->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArraysWithCount = new BasicTypeArraysWithCount();
            pBasicTypeArraysWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithMax = new BasicTypePointersWithMax();
            pBasicTypePointersWithMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithCountAndMax =
                new BasicTypePointersWithCountAndMax();
            pBasicTypePointersWithCountAndMax->Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructPointers";
    }

private:

    friend class ::XrttiAccess;
    
    BasicTypes *pBasicTypes;

    BasicTypeArrays *pBasicTypeArrays;

    BasicTypeArraysWithCount *pBasicTypeArraysWithCount;

    BasicTypePointersWithMax *pBasicTypePointersWithMax;

    BasicTypePointersWithCountAndMax *pBasicTypePointersWithCountAndMax;
};

class StructArraysWithCount : public TestClass
{
public:

    inline StructArraysWithCount()
        : basicTypesArray_CountM(0),
          basicTypeArraysArray_CountM(0),
          basicTypeArraysWithCountArray_CountM(0),
          basicTypePointersWithMaxArray_CountM(0),
          basicTypePointersWithCountAndMaxArray_CountM(0)
    {
    }

    virtual void Randomize()
    {
        Rand<s8>(basicTypesArray_CountM, countof(basicTypesArray));
        for (s8 i = 0; i < basicTypesArray_CountM; i++) {
            basicTypesArray[i].Randomize();
        }

        Rand<u8>(basicTypeArraysArray_CountM, 
                 countof(basicTypeArraysArray));
        for (u8 i = 0; i < basicTypeArraysArray_CountM; i++) {
            basicTypeArraysArray[i].Randomize();
        }

        Rand<u16>(basicTypeArraysWithCountArray_CountM,
                  countof(basicTypeArraysWithCountArray));
        for (u16 i = 0; i < basicTypeArraysWithCountArray_CountM; i++) {
            basicTypeArraysWithCountArray[i].Randomize();
        }

        Rand<u32>(basicTypePointersWithMaxArray_CountM,
                  countof(basicTypePointersWithMaxArray));
        for (u32 i = 0; i < basicTypePointersWithMaxArray_CountM; i++) {
            basicTypePointersWithMaxArray[i].Randomize();
        }

        Rand<u32>(basicTypePointersWithCountAndMaxArray_CountM,
                  countof(basicTypePointersWithCountAndMaxArray));
        for (u32 i = 0;
             i < basicTypePointersWithCountAndMaxArray_CountM; i++) {
            basicTypePointersWithCountAndMaxArray[i].Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructArraysWithCount";
    }

private:

    friend class ::XrttiAccess;

    s8 basicTypesArray_CountM;
    BasicTypes basicTypesArray[4];

    u8 basicTypeArraysArray_CountM;
    BasicTypeArrays basicTypeArraysArray[6];

    u16 basicTypeArraysWithCountArray_CountM;
    BasicTypeArraysWithCount basicTypeArraysWithCountArray[10];

    u32 basicTypePointersWithMaxArray_CountM;
    BasicTypePointersWithMax basicTypePointersWithMaxArray[6];

    u32 basicTypePointersWithCountAndMaxArray_CountM;
    BasicTypePointersWithCountAndMax basicTypePointersWithCountAndMaxArray[4];
};

class StructPointersWithCountAndMax : public TestClass
{
public:

    inline StructPointersWithCountAndMax()
        : pBasicTypesArray_CountM(0),
          pBasicTypesArray(NULL),
          pBasicTypeArraysArray_CountM(0),
          pBasicTypeArraysArray(NULL),
          pBasicTypeArraysWithCountArray_CountM(0),
          pBasicTypeArraysWithCountArray(NULL),
          pBasicTypePointersWithMaxArray_CountM(0),
          pBasicTypePointersWithMaxArray(NULL),
          pBasicTypePointersWithCountAndMaxArray_CountM(0),
          pBasicTypePointersWithCountAndMaxArray(NULL)
    {
    }

    virtual ~StructPointersWithCountAndMax()
    {
        delete [] pBasicTypesArray;
        delete [] pBasicTypeArraysArray;
        delete [] pBasicTypeArraysWithCountArray;
        delete [] pBasicTypePointersWithMaxArray;
        delete [] pBasicTypePointersWithCountAndMaxArray;
    }

    virtual void Randomize()
    {
        Rand<s8>(pBasicTypesArray_CountM, pBasicTypesArray_Max);
        pBasicTypesArray = new BasicTypes[pBasicTypesArray_CountM];
        for (s8 i = 0; i < pBasicTypesArray_CountM; i++) {
            pBasicTypesArray[i].Randomize();
        }

        Rand<u8>(pBasicTypeArraysArray_CountM, pBasicTypeArraysArray_Max);
        pBasicTypeArraysArray = 
            new BasicTypeArrays[pBasicTypeArraysArray_CountM];
        for (u8 i = 0; i < pBasicTypeArraysArray_CountM; i++) {
            pBasicTypeArraysArray[i].Randomize();
        }

        Rand<u16>(pBasicTypeArraysWithCountArray_CountM, 
                  pBasicTypeArraysWithCountArray_Max);
        pBasicTypeArraysWithCountArray = new BasicTypeArraysWithCount
            [pBasicTypeArraysWithCountArray_CountM];
        for (u16 i = 0; i < pBasicTypeArraysWithCountArray_CountM; i++) {
            pBasicTypeArraysWithCountArray[i].Randomize();
        }

        Rand<u32>(pBasicTypePointersWithMaxArray_CountM, 
                  pBasicTypePointersWithMaxArray_Max);
        pBasicTypePointersWithMaxArray = new BasicTypePointersWithMax
            [pBasicTypePointersWithMaxArray_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithMaxArray_CountM; i++) {
            pBasicTypePointersWithMaxArray[i].Randomize();
        }

        Rand<u32>(pBasicTypePointersWithCountAndMaxArray_CountM, 
                  pBasicTypePointersWithCountAndMaxArray_Max);
        pBasicTypePointersWithCountAndMaxArray = new 
            BasicTypePointersWithCountAndMax
            [pBasicTypePointersWithCountAndMaxArray_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithCountAndMaxArray_CountM;
             i++) {
            pBasicTypePointersWithCountAndMaxArray[i].Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructPointersWithCountAndMax";
    }

private:

    friend class ::XrttiAccess;

    static const int pBasicTypesArray_Max = 4;
    s8 pBasicTypesArray_CountM;
    BasicTypes *pBasicTypesArray;

    static const int pBasicTypeArraysArray_Max = 6;
    u8 pBasicTypeArraysArray_CountM;
    BasicTypeArrays *pBasicTypeArraysArray;

    static const int pBasicTypeArraysWithCountArray_Max = 10;
    u16 pBasicTypeArraysWithCountArray_CountM;
    BasicTypeArraysWithCount *pBasicTypeArraysWithCountArray;

    static const int pBasicTypePointersWithMaxArray_Max = 6;
    u32 pBasicTypePointersWithMaxArray_CountM;
    BasicTypePointersWithMax *pBasicTypePointersWithMaxArray;

    static const int pBasicTypePointersWithCountAndMaxArray_Max = 4;
    u32 pBasicTypePointersWithCountAndMaxArray_CountM;
    BasicTypePointersWithCountAndMax *pBasicTypePointersWithCountAndMaxArray;
};

class StructArrayOfPointers : public TestClass
{
public:

    inline StructArrayOfPointers()
    {
        for (u32 i = 0; i < countof(basicTypes); i++) {
            basicTypes[i] = NULL;
        }

        for (u32 i = 0; i < countof(basicTypeArrays); i++) {
            basicTypeArrays[i] = NULL;
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCount); 
             i++) {
            basicTypeArraysWithCount[i] = NULL;
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMax); 
             i++) {
            basicTypePointersWithMax[i] = NULL;
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMax); i++) {
            basicTypePointersWithCountAndMax[i] = NULL;
        }
    }

    virtual ~StructArrayOfPointers()
    {
        for (u32 i = 0; i < countof(basicTypes); i++) {
            delete basicTypes[i];
        }

        for (u32 i = 0; i < countof(basicTypeArrays); i++) {
            delete basicTypeArrays[i];
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCount); 
             i++) {
            delete basicTypeArraysWithCount[i];
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMax); 
             i++) {
            delete basicTypePointersWithMax[i];
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMax); i++) {
            delete basicTypePointersWithCountAndMax[i];
        }
    }

    virtual void Randomize()
    {
        for (u32 i = 0; i < countof(basicTypes); i++) {
            basicTypes[i] = new BasicTypes;
            basicTypes[i]->Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArrays); i++) {
            basicTypeArrays[i] = new BasicTypeArrays;
            basicTypeArrays[i]->Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCount); 
             i++) {
            basicTypeArraysWithCount[i] = new BasicTypeArraysWithCount;
            basicTypeArraysWithCount[i]->Randomize();
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMax); 
             i++) {
            basicTypePointersWithMax[i] = new BasicTypePointersWithMax;
            basicTypePointersWithMax[i]->Randomize();
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMax); i++) {
            basicTypePointersWithCountAndMax[i] =
                new BasicTypePointersWithCountAndMax;
            basicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructArrayOfPointers";
    }

private:

    friend class ::XrttiAccess;

    BasicTypes *basicTypes[4];

    BasicTypeArrays *basicTypeArrays[6];

    BasicTypeArraysWithCount *basicTypeArraysWithCount[10];

    BasicTypePointersWithMax *basicTypePointersWithMax[6];

    BasicTypePointersWithCountAndMax *
    basicTypePointersWithCountAndMax[4];
};

class StructArrayOfPointersWithCount : public TestClass
{
public:

    inline StructArrayOfPointersWithCount()
        : basicTypes_CountM(0),
          basicTypeArrays_CountM(0),
          basicTypeArraysWithCount_CountM(0),
          basicTypePointersWithMax_CountM(0),
          basicTypePointersWithCountAndMax_CountM(0)
    {
    }

    virtual ~StructArrayOfPointersWithCount()
    {
        for (s8 i = 0; i < basicTypes_CountM; i++) {
            delete basicTypes[i];
        }
        
        for (u8 i = 0; i < basicTypeArrays_CountM; i++) {
            delete basicTypeArrays[i];
        }

        for (u16 i = 0; i < basicTypeArraysWithCount_CountM; i++) {
            delete basicTypeArraysWithCount[i];
        }

        for (u32 i = 0; i < basicTypePointersWithMax_CountM; i++) {
            delete basicTypePointersWithMax[i];
        }

        for (u32 i = 0; i < basicTypePointersWithCountAndMax_CountM;
             i++) {
            delete basicTypePointersWithCountAndMax[i];
        }
    }

    virtual void Randomize()
    {
        Rand<s8>(basicTypes_CountM, countof(basicTypes));
        for (s8 i = 0; i < basicTypes_CountM; i++) {
            basicTypes[i] = new BasicTypes;
            basicTypes[i]->Randomize();
        }
        
        Rand<u8>(basicTypeArrays_CountM,
                 countof(basicTypeArrays));
        for (u8 i = 0; i < basicTypeArrays_CountM; i++) {
            basicTypeArrays[i] = new BasicTypeArrays;
            basicTypeArrays[i]->Randomize();
        }

        Rand<u16>(basicTypeArraysWithCount_CountM,
                  countof(basicTypeArraysWithCount));
        for (u16 i = 0; i < basicTypeArraysWithCount_CountM; i++) {
            basicTypeArraysWithCount[i] = new BasicTypeArraysWithCount;
            basicTypeArraysWithCount[i]->Randomize();
        }

        Rand<u32>(basicTypePointersWithMax_CountM,
                  countof(basicTypePointersWithMax));
        for (u32 i = 0; i < basicTypePointersWithMax_CountM; i++) {
            basicTypePointersWithMax[i] = new BasicTypePointersWithMax;
            basicTypePointersWithMax[i]->Randomize();
        }

        Rand<u32>(basicTypePointersWithCountAndMax_CountM,
                  countof(basicTypePointersWithCountAndMax));
        for (u32 i = 0; i < basicTypePointersWithCountAndMax_CountM;
             i++) {
            basicTypePointersWithCountAndMax[i] =
                new BasicTypePointersWithCountAndMax;
            basicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructArrayOfPointersWithCount";
    }

private:

    friend class ::XrttiAccess;
    
    s8 basicTypes_CountM;
    BasicTypes *basicTypes[4];

    u8 basicTypeArrays_CountM;
    BasicTypeArrays *basicTypeArrays[6];

    u16 basicTypeArraysWithCount_CountM;
    BasicTypeArraysWithCount *basicTypeArraysWithCount[10];

    u32 basicTypePointersWithMax_CountM;
    BasicTypePointersWithMax *basicTypePointersWithMax[6];

    u32 basicTypePointersWithCountAndMax_CountM;
    BasicTypePointersWithCountAndMax *
    basicTypePointersWithCountAndMax[4];
};

class StructDoublePointersWithMax : public TestClass
{
public:

    inline StructDoublePointersWithMax()
        : pBasicTypes(NULL),
          pBasicTypeArrays(NULL),
          pBasicTypeArraysWithCount(NULL),
          pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax(NULL)
    {
    }

    virtual ~StructDoublePointersWithMax()
    {
        if (pBasicTypes) {
            for (int i = 0; i < pBasicTypes_Max; i++) {
                delete pBasicTypes[i];
            }
            delete [] pBasicTypes;
        }

        if (pBasicTypeArrays) {
            for (int i = 0; i < pBasicTypeArrays_Max; i++) {
                delete pBasicTypeArrays[i];
            }
            delete [] pBasicTypeArrays;
        }

        if (pBasicTypeArraysWithCount) {
            for (int i = 0; i < pBasicTypeArraysWithCount_Max; i++) {
                delete pBasicTypeArraysWithCount[i];
            }
            delete [] pBasicTypeArraysWithCount;
        }

        if (pBasicTypePointersWithMax) {
            for (int i = 0; i < pBasicTypePointersWithMax_Max; i++) {
                delete pBasicTypePointersWithMax[i];
            }
            delete [] pBasicTypePointersWithMax;
        }

        if (pBasicTypePointersWithCountAndMax) {
            for (int i = 0; i < pBasicTypePointersWithCountAndMax_Max;
                 i++) {
                delete pBasicTypePointersWithCountAndMax[i];
            }
            delete [] pBasicTypePointersWithCountAndMax;
        }
    }

    virtual void Randomize()
    {
        pBasicTypes = new BasicTypes * [pBasicTypes_Max];
        for (int i = 0; i < pBasicTypes_Max; i++) {
            pBasicTypes[i] = new BasicTypes;
            pBasicTypes[i]->Randomize();
        }

        pBasicTypeArrays = new BasicTypeArrays *
            [pBasicTypeArrays_Max];
        for (int i = 0; i < pBasicTypeArrays_Max; i++) {
            pBasicTypeArrays[i] = new BasicTypeArrays;
            pBasicTypeArrays[i]->Randomize();
        }

        pBasicTypeArraysWithCount = new BasicTypeArraysWithCount *
            [pBasicTypeArraysWithCount_Max];
        for (int i = 0; i < pBasicTypeArraysWithCount_Max; i++) {
            pBasicTypeArraysWithCount[i] = new
                BasicTypeArraysWithCount;
            pBasicTypeArraysWithCount[i]->Randomize();
        }

        pBasicTypePointersWithMax = 
            new BasicTypePointersWithMax *
            [pBasicTypePointersWithMax_Max];
        for (int i = 0; i < pBasicTypePointersWithMax_Max; i++) {
            pBasicTypePointersWithMax[i] = new
                BasicTypePointersWithMax;
            pBasicTypePointersWithMax[i]->Randomize();
        }

        pBasicTypePointersWithCountAndMax = 
            new BasicTypePointersWithCountAndMax *
            [pBasicTypePointersWithCountAndMax_Max];
        for (int i = 0; i < pBasicTypePointersWithCountAndMax_Max;
             i++) {
            pBasicTypePointersWithCountAndMax[i] = new
                BasicTypePointersWithCountAndMax;
            pBasicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructDoublePointersWithMax";
    }

private:

    friend class ::XrttiAccess;
    
    static const int pBasicTypes_Max = 4;
    BasicTypes **pBasicTypes;

    static const int pBasicTypeArrays_Max = 6;
    BasicTypeArrays **pBasicTypeArrays;

    static const int pBasicTypeArraysWithCount_Max = 10;
    BasicTypeArraysWithCount **pBasicTypeArraysWithCount;

    static const int pBasicTypePointersWithMax_Max = 6;
    BasicTypePointersWithMax **pBasicTypePointersWithMax;

    static const int pBasicTypePointersWithCountAndMax_Max = 4;
    BasicTypePointersWithCountAndMax **pBasicTypePointersWithCountAndMax;
};

class StructDoublePointersWithCountAndMax : public TestClass
{
public:

    inline StructDoublePointersWithCountAndMax()
        : pBasicTypes_CountM(0),
          pBasicTypes(NULL),
          pBasicTypeArrays_CountM(0),
          pBasicTypeArrays(NULL),
          pBasicTypeArraysWithCount_CountM(0),
          pBasicTypeArraysWithCount(NULL),
          pBasicTypePointersWithMax_CountM(0),
          pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax_CountM(0),
          pBasicTypePointersWithCountAndMax(NULL)
    {
    }

    virtual ~StructDoublePointersWithCountAndMax()
    {
        for (s8 i = 0; i < pBasicTypes_CountM; i++) {
            delete pBasicTypes[i];
        }
        delete [] pBasicTypes;

        for (u8 i = 0; i < pBasicTypeArrays_CountM; i++) {
            delete pBasicTypeArrays[i];
        }
        delete [] pBasicTypeArrays;

        for (u16 i = 0; i < pBasicTypeArraysWithCount_CountM; i++) {
            delete pBasicTypeArraysWithCount[i];
        }
        delete [] pBasicTypeArraysWithCount;

        for (u32 i = 0; i < pBasicTypePointersWithMax_CountM; i++) {
            delete pBasicTypePointersWithMax[i];
        }
        delete [] pBasicTypePointersWithMax;

        for (u32 i = 0; i < pBasicTypePointersWithCountAndMax_CountM;
             i++) {
            delete pBasicTypePointersWithCountAndMax[i];
        }
        delete [] pBasicTypePointersWithCountAndMax;
    }

    virtual void Randomize()
    {
        Rand<s8>(pBasicTypes_CountM, pBasicTypes_Max);
        pBasicTypes = new BasicTypes * [pBasicTypes_CountM];
        for (s8 i = 0; i < pBasicTypes_CountM; i++) {
            pBasicTypes[i] = new BasicTypes;
            pBasicTypes[i]->Randomize();
        }

        Rand<u8>(pBasicTypeArrays_CountM, pBasicTypeArrays_Max);
        pBasicTypeArrays = new BasicTypeArrays * 
            [pBasicTypeArrays_CountM];
        for (u8 i = 0; i < pBasicTypeArrays_CountM; i++) {
            pBasicTypeArrays[i] = new BasicTypeArrays;
            pBasicTypeArrays[i]->Randomize();
        }

        Rand<u16>(pBasicTypeArraysWithCount_CountM,
                  pBasicTypeArraysWithCount_Max);
        pBasicTypeArraysWithCount = new BasicTypeArraysWithCount * 
            [pBasicTypeArraysWithCount_CountM];
        for (u16 i = 0; i < pBasicTypeArraysWithCount_CountM; i++) {
            pBasicTypeArraysWithCount[i] = 
                new BasicTypeArraysWithCount;
            pBasicTypeArraysWithCount[i]->Randomize();
        }

        Rand<u32>(pBasicTypePointersWithMax_CountM,
                  pBasicTypePointersWithMax_Max);
        pBasicTypePointersWithMax = new BasicTypePointersWithMax * 
            [pBasicTypePointersWithMax_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithMax_CountM; i++) {
            pBasicTypePointersWithMax[i] =
                new BasicTypePointersWithMax;
            pBasicTypePointersWithMax[i]->Randomize();
        }

        Rand<u32>(pBasicTypePointersWithCountAndMax_CountM,
                  pBasicTypePointersWithCountAndMax_Max);
        pBasicTypePointersWithCountAndMax = 
            new BasicTypePointersWithCountAndMax * 
            [pBasicTypePointersWithCountAndMax_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithCountAndMax_CountM;
             i++) {
            pBasicTypePointersWithCountAndMax[i] =
                new BasicTypePointersWithCountAndMax;
            pBasicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "StructDoublePointersWithCountAndMax";
    }

private:

    friend class ::XrttiAccess;
    
    static const int pBasicTypes_Max = 4;
    s8 pBasicTypes_CountM;
    BasicTypes **pBasicTypes;

    static const int pBasicTypeArrays_Max = 6;
    u8 pBasicTypeArrays_CountM;
    BasicTypeArrays **pBasicTypeArrays;

    static const int pBasicTypeArraysWithCount_Max = 10;
    u16 pBasicTypeArraysWithCount_CountM;
    BasicTypeArraysWithCount **pBasicTypeArraysWithCount;

    static const int pBasicTypePointersWithMax_Max = 6;
    u32 pBasicTypePointersWithMax_CountM;
    BasicTypePointersWithMax **pBasicTypePointersWithMax;

    static const int pBasicTypePointersWithCountAndMax_Max = 4;
    u32 pBasicTypePointersWithCountAndMax_CountM;
    BasicTypePointersWithCountAndMax **pBasicTypePointersWithCountAndMax;
};

class DeepInnerBasicType : public TestClass
{
public:

    virtual void Randomize()
    {
        Rand<u64>(outer.middle.inner.u64v);
    }

    virtual const char *GetIdentity() const
    {
        return "DeepInnerBasicType";
    }

private:

    friend class ::XrttiAccess;

    struct {
        class {
        public:
            struct {
                u64 u64v;
            } inner;
        } middle;
    } outer;
};

class DeepInnerStructAndHierarchy : public TestClass
{
public:

    virtual void Randomize()
    {
        outer.middle.Randomize();
    }

    virtual const char *GetIdentity() const
    {
        return "DeepInnerStructAndHierarchy";
    }

private:

    friend class ::XrttiAccess;

    struct {
        class {
        public:

            void Randomize()
            {
                inner.upper.Randomize();
                inner.middle.Randomize();
                inner.bottom.Randomize();
            }

            struct {

                class Upper
                {
                public:

                    void Randomize()
                    {
                        upperOuter.Randomize();
                    }
                    
                    class UpperOuter
                    {
                    public:

                        void Randomize()
                        {
                            serialized.middle.Randomize();

                            serialized.bottom.Randomize();

                            RandReal<double>(transient_double);
                        }

                        class Middle
                        {
                        public:

                            void Randomize()
                            {
                                Rand<u8>(u8v);
                            }
                            
                        private:

                            friend class ::XrttiAccess;

                            u8 u8v;
                        };

                        class Bottom : public Middle
                        {
                        public:

                            void Randomize()
                            {
                                this->Middle::Randomize();

                                RandReal<float>(serialized.fl);
                            }

                        private:

                            friend class ::XrttiAccess;

                            struct {
                                float fl;
                            } serialized;
                        };

                        struct {
                            Middle middle;
                            Bottom bottom;
                        } serialized;

                        double transient_double;
                    };

                    UpperOuter upperOuter;
                };

                class Middle : public Upper
                {
                public:

                    void Randomize()
                    {
                        this->Upper::Randomize();

                        Rand<u16>(serialized.u16v);
                        
                        RandReal<double>(transient_double);
                    }

                private:

                    friend class ::XrttiAccess;
                    
                    struct {
                        u16 u16v;
                    } serialized;

                    double transient_double;
                };
                
                class Bottom : public Middle
                {
                public:

                    void Randomize()
                    {
                        this->Middle::Randomize();

                        Rand<u32>(serialized.u32v);

                        RandReal<double>(transient_double);
                    }

                private:

                    friend class ::XrttiAccess;
                    
                    struct {
                        u32 u32v;
                    } serialized;

                    double transient_double;
                };
                
                Upper upper;
                Middle middle;
                Bottom bottom;
            } inner;
        } middle;
    } outer;
};

class BigArray : public TestClass
{
public:

    virtual void Randomize()
    {
        u32 array_count;
        Rand<u32>(array_count, countof(array) - 1);

        for (u32 i = 0; i < array_count; i++) {
            array[i] = (Random(0xFFFFFFFF) % ('z' - 'a')) + 'a';
        }
        array[array_count] = 0;

        for (u32 i = 0; i < countof(array2); i++) {
            RandReal<float>(array2[i]);
        }
    }

    virtual const char *GetIdentity() const
    {
        return "BigArray";
    }

private:

    friend class ::XrttiAccess;

    u8 array[1005];

    float array2[1020];
};

class SimpleStructArray : public TestClass
{
public:
    
    virtual void Randomize()
    {
        Rand<u32>(simpleStructs_CountM, countof(simpleStructs));

        for (u32 i = 0; i < simpleStructs_CountM; i++) {
            Rand<u32>(simpleStructs[i].val1);
            Rand<u32>(simpleStructs[i].val2);
            Rand<u32>(simpleStructs[i].val3);
        }
    }

    virtual const char *GetIdentity() const
    {
        return "SimpleStructArray";
    }

private:

    friend class ::XrttiAccess;

    class SimpleStruct
    {
    public:

        u32 val1, val2, val3;
    };

    u32 simpleStructs_CountM;
    SimpleStruct simpleStructs[100];
};

class Optional : public TestClass
{
public:
    
    Optional()
        : pBasicTypes(NULL), pBasicTypeArrays(NULL), 
          pBasicTypeArraysWithCount(NULL), pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax(NULL), pStructs(NULL),
          pStructArrays(NULL), pStructPointers(NULL), 
          pStructArraysWithCount(NULL), pStructPointersWithCountAndMax(NULL), 
          pStructArrayOfPointers(NULL), pStructArrayOfPointersWithCount(NULL), 
          pStructDoublePointersWithMax(NULL), 
          pStructDoublePointersWithCountAndMax(NULL),
          pDeepInnerBasicType(NULL), pDeepInnerStructAndHierarchy(NULL), 
          pBigArray(NULL), pSimpleStructArray(NULL)
    {
    }

    virtual ~Optional()
    {
        delete pBasicTypes;
        delete pBasicTypeArrays;
        delete pBasicTypeArraysWithCount;
        delete pBasicTypePointersWithMax;
        delete pBasicTypePointersWithCountAndMax;
        delete pStructs;
        delete pStructArrays;
        delete pStructPointers;
        delete pStructArraysWithCount;
        delete pStructPointersWithCountAndMax;
        delete pStructArrayOfPointers;
        delete pStructArrayOfPointersWithCount;
        delete pStructDoublePointersWithMax;
        delete pStructDoublePointersWithCountAndMax;
        delete pDeepInnerBasicType;
        delete pDeepInnerStructAndHierarchy;
        delete pBigArray;
        delete pSimpleStructArray;
    }

    virtual void Randomize()
    {
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypes = new BasicTypes();
            pBasicTypes->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArrays = new BasicTypeArrays();
            pBasicTypeArrays->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArraysWithCount = new BasicTypeArraysWithCount();
            pBasicTypeArraysWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithMax = new BasicTypePointersWithMax();
            pBasicTypePointersWithMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithCountAndMax = new
                BasicTypePointersWithCountAndMax();
            pBasicTypePointersWithCountAndMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructs = new Structs();
            pStructs->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArrays = new StructArrays();
            pStructArrays->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructPointers = new StructPointers();
            pStructPointers->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArraysWithCount = new StructArraysWithCount();
            pStructArraysWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructPointersWithCountAndMax = new
                StructPointersWithCountAndMax();
            pStructPointersWithCountAndMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArrayOfPointers = new StructArrayOfPointers();
            pStructArrayOfPointers->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArrayOfPointersWithCount = new
                StructArrayOfPointersWithCount();
            pStructArrayOfPointersWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructDoublePointersWithMax = new StructDoublePointersWithMax();
            pStructDoublePointersWithMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructDoublePointersWithCountAndMax = new
                StructDoublePointersWithCountAndMax();
            pStructDoublePointersWithCountAndMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pDeepInnerBasicType = new DeepInnerBasicType();
            pDeepInnerBasicType->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pDeepInnerStructAndHierarchy = new DeepInnerStructAndHierarchy();
            pDeepInnerStructAndHierarchy->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBigArray = new BigArray();
            pBigArray->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pSimpleStructArray = new SimpleStructArray();
            pSimpleStructArray->Randomize();
        }
    }

    virtual const char *GetIdentity() const
    {
        return "Optional";
    }

private:

    friend class ::XrttiAccess;

    BasicTypes *pBasicTypes;
    BasicTypeArrays *pBasicTypeArrays;
    BasicTypeArraysWithCount *pBasicTypeArraysWithCount;
    BasicTypePointersWithMax *pBasicTypePointersWithMax;
    BasicTypePointersWithCountAndMax *pBasicTypePointersWithCountAndMax;
    Structs *pStructs;
    StructArrays *pStructArrays;
    StructPointers *pStructPointers;
    StructArraysWithCount *pStructArraysWithCount;
    StructPointersWithCountAndMax *pStructPointersWithCountAndMax;
    StructArrayOfPointers *pStructArrayOfPointers;
    StructArrayOfPointersWithCount *pStructArrayOfPointersWithCount;
    StructDoublePointersWithMax *pStructDoublePointersWithMax;
    StructDoublePointersWithCountAndMax *pStructDoublePointersWithCountAndMax;
    DeepInnerBasicType *pDeepInnerBasicType;
    DeepInnerStructAndHierarchy *pDeepInnerStructAndHierarchy;
    BigArray *pBigArray;
    SimpleStructArray *pSimpleStructArray;
};

class LocalTestClass
{
public:

    virtual ~LocalTestClass() { }

    virtual void Randomize() { }
};

class LocalBasicTypes : public LocalTestClass
{
public:

    // The actual value of fRandomize does not matter
    virtual void Randomize()
    {
        Rand<s8>(s8v);
        Rand<u8>(u8v);
        Rand<s16>(s16v);
        Rand<u16>(u16v);
        Rand<s32>(s32v);
        Rand<u32>(u32v);
        Rand<s64>(s64v);
        Rand<u64>(u64v);
        RandReal<float>(flv);
        RandReal<double>(dlv);
    }

private:

    friend class ::XrttiAccess;
    
    s8 s8v;

    u8 u8v;

    s16 s16v;

    u16 u16v;

    s32 s32v;

    u32 u32v;

    s64 s64v;

    u64 u64v;

    float flv;

    double dlv;
};

class LocalBasicTypeArrays : public LocalTestClass
{
public:
    
    // The actual value of fRandomize does not matter
    virtual void Randomize()
    {
        RandArray<s8>(s8v, countof(s8v));
        RandArray<u8>(u8v, countof(u8v));
        RandArray<s16>(s16v, countof(s16v));
        RandArray<u16>(u16v, countof(u16v));
        RandArray<s32>(s32v, countof(s32v));
        RandArray<u32>(u32v, countof(u32v));
        RandArray<s64>(s64v, countof(s64v));
        RandArray<u64>(u64v, countof(u64v));
        RandRealArray<float>(flv, countof(flv));
        RandRealArray<double>(dlv, countof(dlv));
    }

private:

    friend class ::XrttiAccess;

    s8 s8v[8];

    u8 u8v[8];

    s16 s16v[16];

    u16 u16v[16];

    s32 s32v[32];

    u32 u32v[32];

    s64 s64v[64];

    u64 u64v[64];

    float flv[10];

    double dlv[10];
};

class LocalBasicTypeArraysWithCount : public LocalTestClass
{
public:

    inline LocalBasicTypeArraysWithCount()
        : s8v_CountM(0), u8v_CountM(0), s16v_CountM(0), u16v_CountM(0),
          s32v_CountM(0), u32v_CountM(0), s64v_CountM(0), u64v_CountM(0),
          flv_CountM(0), dlv_CountM(0)
    {
    }

    virtual void Randomize()
    {
        RandArray<s8, s8>(s8v, s8v_CountM, countof(s8v));
        RandArray<u8, u8>(u8v, u8v_CountM, countof(u8v));
        RandArray<s16, s16>(s16v, s16v_CountM, countof(s16v));
        RandArray<u16, u16>(u16v, u16v_CountM, countof(u16v));
        RandArray<s32, s32>(s32v, s32v_CountM, countof(s32v));
        RandArray<u32, u32>(u32v, u32v_CountM, countof(u32v));
        RandArray<s64, s32>(s64v, s64v_CountM, countof(s64v));
        RandArray<u64, u32>(u64v, u64v_CountM, countof(u64v));
        RandRealArray<float, u32>(flv, flv_CountM, countof(flv));
        RandRealArray<double, u32>(dlv, dlv_CountM, countof(dlv));
    }

private:

    friend class ::XrttiAccess;

    s8 s8v_CountM;
    s8 s8v[8];

    u8 u8v_CountM; 
    u8 u8v[8];

    s16 s16v_CountM;
    s16 s16v[16];

    u16 u16v_CountM;
    u16 u16v[16];

    s32 s32v_CountM;
    s32 s32v[32];

    u32 u32v_CountM;
    u32 u32v[32];

    s32 s64v_CountM;
    s64 s64v[64];

    u32 u64v_CountM;
    u64 u64v[64];

    u32 flv_CountM;
    float flv[10];

    u32 dlv_CountM;
    double dlv[10];
};

class LocalBasicTypePointersWithMax : public LocalTestClass
{
public:

    inline LocalBasicTypePointersWithMax()
        : ps8(NULL), pu8(NULL), ps16(NULL), pu16(NULL), ps32(NULL), pu32(NULL),
          ps64(NULL), pu64(NULL), pFl(NULL), pDl(NULL)
    {
    }

    virtual ~LocalBasicTypePointersWithMax()
    {
        delete [] ps8;
        delete [] pu8;
        delete [] ps16;
        delete [] pu16;
        delete [] ps32;
        delete [] pu32;
        delete [] ps64;
        delete [] pu64;
        delete [] pFl;
        delete [] pDl;
    }

    virtual void Randomize()
    {
        CreateRandArray<s8>(ps8, ps8_Max);
        CreateRandArray<u8>(pu8, pu8_Max);
        CreateRandArray<s16>(ps16, ps16_Max);
        CreateRandArray<u16>(pu16, pu16_Max);
        CreateRandArray<s32>(ps32, ps32_Max);
        CreateRandArray<u32>(pu32, pu32_Max);
        CreateRandArray<s64>(ps64, ps64_Max);
        CreateRandArray<u64>(pu64, pu64_Max);
        CreateRandRealArray<float>(pFl, pFl_Max);
        CreateRandRealArray<double>(pDl, pDl_Max);
    }

private:

    friend class ::XrttiAccess;

    static const int ps8_Max = 8;
    s8 *ps8;

    static const int pu8_Max = 8;
    u8 *pu8;

    static const int ps16_Max = 16;
    s16 *ps16;

    static const int pu16_Max = 16;
    u16 *pu16;

    static const int ps32_Max = 32;
    s32 *ps32;

    static const int pu32_Max = 32;
    u32 *pu32;

    static const int ps64_Max = 64;
    s64 *ps64;

    static const int pu64_Max = 64;
    u64 *pu64;

    static const int pFl_Max = 32;
    float *pFl;

    static const int pDl_Max = 64;
    double *pDl;
};

class LocalBasicTypePointersWithCountAndMax : public LocalTestClass
{
public:

    inline LocalBasicTypePointersWithCountAndMax()
        : ps8_CountM(0), ps8(NULL), pu8_CountM(0), pu8(NULL), 
          ps16_CountM(0), ps16(NULL), pu16_CountM(0), pu16(NULL), 
          ps32_CountM(0), ps32(NULL), pu32_CountM(0), pu32(NULL), 
          ps64_CountM(0), ps64(NULL), pu64_CountM(0), pu64(NULL), 
          pFl_CountM(0), pFl(NULL), pDl_CountM(0), pDl(NULL)
    {
    }

    virtual ~LocalBasicTypePointersWithCountAndMax()
    {
        delete [] ps8;
        delete [] pu8;
        delete [] ps16;
        delete [] pu16;
        delete [] ps32;
        delete [] pu32;
        delete [] ps64;
        delete [] pu64;
        delete [] pFl;
        delete [] pDl;
    }

    virtual void Randomize()
    {
        CreateRandArray<s8, s8>(ps8, ps8_CountM, ps8_Max);
        CreateRandArray<u8, u8>(pu8, pu8_CountM, pu8_Max);
        CreateRandArray<s16, s16>(ps16, ps16_CountM, ps16_Max);
        CreateRandArray<u16, u16>(pu16, pu16_CountM, pu16_Max);
        CreateRandArray<s32, s32>(ps32, ps32_CountM, ps32_Max);
        CreateRandArray<u32, u32>(pu32, pu32_CountM, pu32_Max);
        CreateRandArray<s64, s32>(ps64, ps64_CountM, ps64_Max);
        CreateRandArray<u64, u32>(pu64, pu64_CountM, pu64_Max);
        CreateRandRealArray<float, u32>(pFl, pFl_CountM, pFl_Max);
        CreateRandRealArray<double, u32>(pDl, pDl_CountM, pDl_Max);
    }

private:

    friend class ::XrttiAccess;

    static const int ps8_Max = 8;
    s8 ps8_CountM;
    s8 *ps8;

    static const int pu8_Max = 8;
    u8 pu8_CountM; 
    u8 *pu8;

    static const int ps16_Max = 16;
    s16 ps16_CountM;
    s16 *ps16;

    static const int pu16_Max = 16;
    u16 pu16_CountM;
    u16 *pu16;

    static const int ps32_Max = 32;
    s32 ps32_CountM;
    s32 *ps32;

    static const int pu32_Max = 32;
    u32 pu32_CountM;
    u32 *pu32;

    static const int ps64_Max = 64;
    s32 ps64_CountM;
    s64 *ps64;

    static const int pu64_Max = 64;
    u32 pu64_CountM;
    u64 *pu64;

    static const int pFl_Max = 32;
    u32 pFl_CountM;
    float *pFl;

    static const int pDl_Max = 64;
    u32 pDl_CountM;
    double *pDl;
};

class LocalStructs : public LocalTestClass
{
public:

    virtual void Randomize()
    {
        basicTypes.Randomize();
        basicTypeArrays.Randomize();
        basicTypeArraysWithCount.Randomize();
        basicTypePointersWithMax.Randomize();
        basicTypePointersWithCountAndMax.Randomize();
    }

private:

    friend class ::XrttiAccess;

    LocalBasicTypes basicTypes;

    BasicTypeArrays basicTypeArrays;

    LocalBasicTypeArraysWithCount basicTypeArraysWithCount;

    BasicTypePointersWithMax basicTypePointersWithMax;

    LocalBasicTypePointersWithCountAndMax basicTypePointersWithCountAndMax;
};

class LocalStructArrays : public LocalTestClass
{
public:

    virtual void Randomize()
    {
        for (u32 i = 0; i < countof(basicTypesArray); i++) {
            basicTypesArray[i].Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArraysArray); i++) {
            basicTypeArraysArray[i].Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCountArray); 
             i++) {
            basicTypeArraysWithCountArray[i].Randomize();
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMaxArray); 
             i++) {
            basicTypePointersWithMaxArray[i].Randomize();
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMaxArray); i++) {
            basicTypePointersWithCountAndMaxArray[i].Randomize();
        }
    }

private:

    friend class ::XrttiAccess;
    
    BasicTypes basicTypesArray[1];

    LocalBasicTypeArrays basicTypeArraysArray[2];

    BasicTypeArraysWithCount basicTypeArraysWithCountArray[4];

    LocalBasicTypePointersWithMax basicTypePointersWithMaxArray[2];

    BasicTypePointersWithCountAndMax basicTypePointersWithCountAndMaxArray[1];
};

class LocalStructPointers : public LocalTestClass
{
public:

    inline LocalStructPointers()
        : pBasicTypes(NULL), pBasicTypeArrays(NULL), 
          pBasicTypeArraysWithCount(NULL), pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax(NULL)
    {
    }

    virtual ~LocalStructPointers()
    {
        delete pBasicTypes;
        delete pBasicTypeArrays;
        delete pBasicTypeArraysWithCount;
        delete pBasicTypePointersWithMax;
        delete pBasicTypePointersWithCountAndMax;
    }
    
    virtual void Randomize()
    {
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypes = new LocalBasicTypes();
            pBasicTypes->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArrays = new LocalBasicTypeArrays();
            pBasicTypeArrays->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArraysWithCount = new BasicTypeArraysWithCount();
            pBasicTypeArraysWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithMax = new LocalBasicTypePointersWithMax();
            pBasicTypePointersWithMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithCountAndMax =
                new BasicTypePointersWithCountAndMax();
            pBasicTypePointersWithCountAndMax->Randomize();
        }
    }

private:

    friend class ::XrttiAccess;
    
    LocalBasicTypes *pBasicTypes;
    
    LocalBasicTypeArrays *pBasicTypeArrays;

    BasicTypeArraysWithCount *pBasicTypeArraysWithCount;

    LocalBasicTypePointersWithMax *pBasicTypePointersWithMax;

    BasicTypePointersWithCountAndMax *pBasicTypePointersWithCountAndMax;
};

class LocalStructArraysWithCount : public LocalTestClass
{
public:

    inline LocalStructArraysWithCount()
        : basicTypesArray_CountM(0),
          basicTypeArraysArray_CountM(0),
          basicTypeArraysWithCountArray_CountM(0),
          basicTypePointersWithMaxArray_CountM(0),
          basicTypePointersWithCountAndMaxArray_CountM(0)
    {
    }

    virtual void Randomize()
    {
        Rand<s8>(basicTypesArray_CountM, countof(basicTypesArray));
        for (s8 i = 0; i < basicTypesArray_CountM; i++) {
            basicTypesArray[i].Randomize();
        }

        Rand<u8>(basicTypeArraysArray_CountM, 
                 countof(basicTypeArraysArray));
        for (u8 i = 0; i < basicTypeArraysArray_CountM; i++) {
            basicTypeArraysArray[i].Randomize();
        }

        Rand<u16>(basicTypeArraysWithCountArray_CountM,
                  countof(basicTypeArraysWithCountArray));
        for (u16 i = 0; i < basicTypeArraysWithCountArray_CountM; i++) {
            basicTypeArraysWithCountArray[i].Randomize();
        }

        Rand<u32>(basicTypePointersWithMaxArray_CountM,
                  countof(basicTypePointersWithMaxArray));
        for (u32 i = 0; i < basicTypePointersWithMaxArray_CountM; i++) {
            basicTypePointersWithMaxArray[i].Randomize();
        }

        Rand<u32>(basicTypePointersWithCountAndMaxArray_CountM,
                  countof(basicTypePointersWithCountAndMaxArray));
        for (u32 i = 0;
             i < basicTypePointersWithCountAndMaxArray_CountM; i++) {
            basicTypePointersWithCountAndMaxArray[i].Randomize();
        }
    }

private:

    friend class ::XrttiAccess;

    s8 basicTypesArray_CountM;
    BasicTypes basicTypesArray[4];

    u8 basicTypeArraysArray_CountM;
    BasicTypeArrays basicTypeArraysArray[6];

    u16 basicTypeArraysWithCountArray_CountM;
    LocalBasicTypeArraysWithCount basicTypeArraysWithCountArray[10];

    u32 basicTypePointersWithMaxArray_CountM;
    BasicTypePointersWithMax basicTypePointersWithMaxArray[6];

    u32 basicTypePointersWithCountAndMaxArray_CountM;
    LocalBasicTypePointersWithCountAndMax 
    basicTypePointersWithCountAndMaxArray[4];
};

class LocalStructPointersWithCountAndMax : public LocalTestClass
{
public:

    inline LocalStructPointersWithCountAndMax()
        : pBasicTypesArray_CountM(0),
          pBasicTypesArray(NULL),
          pBasicTypeArraysArray_CountM(0),
          pBasicTypeArraysArray(NULL),
          pBasicTypeArraysWithCountArray_CountM(0),
          pBasicTypeArraysWithCountArray(NULL),
          pBasicTypePointersWithMaxArray_CountM(0),
          pBasicTypePointersWithMaxArray(NULL),
          pBasicTypePointersWithCountAndMaxArray_CountM(0),
          pBasicTypePointersWithCountAndMaxArray(NULL)
    {
    }

    virtual ~LocalStructPointersWithCountAndMax()
    {
        delete [] pBasicTypesArray;
        delete [] pBasicTypeArraysArray;
        delete [] pBasicTypeArraysWithCountArray;
        delete [] pBasicTypePointersWithMaxArray;
        delete [] pBasicTypePointersWithCountAndMaxArray;
    }

    virtual void Randomize()
    {
        Rand<s8>(pBasicTypesArray_CountM, pBasicTypesArray_Max);
        pBasicTypesArray = new BasicTypes[pBasicTypesArray_CountM];
        for (s8 i = 0; i < pBasicTypesArray_CountM; i++) {
            pBasicTypesArray[i].Randomize();
        }

        Rand<u8>(pBasicTypeArraysArray_CountM, pBasicTypeArraysArray_Max);
        pBasicTypeArraysArray = 
            new BasicTypeArrays[pBasicTypeArraysArray_CountM];
        for (u8 i = 0; i < pBasicTypeArraysArray_CountM; i++) {
            pBasicTypeArraysArray[i].Randomize();
        }

        Rand<u16>(pBasicTypeArraysWithCountArray_CountM, 
                  pBasicTypeArraysWithCountArray_Max);
        pBasicTypeArraysWithCountArray = new LocalBasicTypeArraysWithCount
            [pBasicTypeArraysWithCountArray_CountM];
        for (u16 i = 0; i < pBasicTypeArraysWithCountArray_CountM; i++) {
            pBasicTypeArraysWithCountArray[i].Randomize();
        }

        Rand<u32>(pBasicTypePointersWithMaxArray_CountM, 
                  pBasicTypePointersWithMaxArray_Max);
        pBasicTypePointersWithMaxArray = new BasicTypePointersWithMax
            [pBasicTypePointersWithMaxArray_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithMaxArray_CountM; i++) {
            pBasicTypePointersWithMaxArray[i].Randomize();
        }

        Rand<u32>(pBasicTypePointersWithCountAndMaxArray_CountM, 
                  pBasicTypePointersWithCountAndMaxArray_Max);
        pBasicTypePointersWithCountAndMaxArray = new 
            BasicTypePointersWithCountAndMax
            [pBasicTypePointersWithCountAndMaxArray_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithCountAndMaxArray_CountM;
             i++) {
            pBasicTypePointersWithCountAndMaxArray[i].Randomize();
        }
    }

private:

    friend class ::XrttiAccess;

    static const int pBasicTypesArray_Max = 4;
    s8 pBasicTypesArray_CountM;
    BasicTypes *pBasicTypesArray;

    static const int pBasicTypeArraysArray_Max = 6;
    u8 pBasicTypeArraysArray_CountM;
    BasicTypeArrays *pBasicTypeArraysArray;

    static const int pBasicTypeArraysWithCountArray_Max = 10;
    u16 pBasicTypeArraysWithCountArray_CountM;
    LocalBasicTypeArraysWithCount *pBasicTypeArraysWithCountArray;

    static const int pBasicTypePointersWithMaxArray_Max = 6;
    u32 pBasicTypePointersWithMaxArray_CountM;
    BasicTypePointersWithMax *pBasicTypePointersWithMaxArray;

    static const int pBasicTypePointersWithCountAndMaxArray_Max = 4;
    u32 pBasicTypePointersWithCountAndMaxArray_CountM;
    BasicTypePointersWithCountAndMax *pBasicTypePointersWithCountAndMaxArray;
};

class LocalStructArrayOfPointers : public LocalTestClass
{
public:

    inline LocalStructArrayOfPointers()
    {
        for (u32 i = 0; i < countof(basicTypes); i++) {
            basicTypes[i] = NULL;
        }

        for (u32 i = 0; i < countof(basicTypeArrays); i++) {
            basicTypeArrays[i] = NULL;
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCount); 
             i++) {
            basicTypeArraysWithCount[i] = NULL;
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMax); 
             i++) {
            basicTypePointersWithMax[i] = NULL;
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMax); i++) {
            basicTypePointersWithCountAndMax[i] = NULL;
        }
    }

    virtual ~LocalStructArrayOfPointers()
    {
        for (u32 i = 0; i < countof(basicTypes); i++) {
            delete basicTypes[i];
        }

        for (u32 i = 0; i < countof(basicTypeArrays); i++) {
            delete basicTypeArrays[i];
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCount); 
             i++) {
            delete basicTypeArraysWithCount[i];
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMax); 
             i++) {
            delete basicTypePointersWithMax[i];
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMax); i++) {
            delete basicTypePointersWithCountAndMax[i];
        }
    }

    virtual void Randomize()
    {
        for (u32 i = 0; i < countof(basicTypes); i++) {
            basicTypes[i] = new LocalBasicTypes;
            basicTypes[i]->Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArrays); i++) {
            basicTypeArrays[i] = new LocalBasicTypeArrays;
            basicTypeArrays[i]->Randomize();
        }

        for (u32 i = 0; i < countof(basicTypeArraysWithCount); 
             i++) {
            basicTypeArraysWithCount[i] = new BasicTypeArraysWithCount;
            basicTypeArraysWithCount[i]->Randomize();
        }

        for (u32 i = 0; i < countof(basicTypePointersWithMax); 
             i++) {
            basicTypePointersWithMax[i] = new LocalBasicTypePointersWithMax;
            basicTypePointersWithMax[i]->Randomize();
        }

        for (u32 i = 0; 
             i < countof(basicTypePointersWithCountAndMax); i++) {
            basicTypePointersWithCountAndMax[i] =
                new LocalBasicTypePointersWithCountAndMax;
            basicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

private:

    friend class ::XrttiAccess;

    LocalBasicTypes *basicTypes[4];

    LocalBasicTypeArrays *basicTypeArrays[6];

    BasicTypeArraysWithCount *basicTypeArraysWithCount[10];

    LocalBasicTypePointersWithMax *basicTypePointersWithMax[6];

    LocalBasicTypePointersWithCountAndMax *
    basicTypePointersWithCountAndMax[4];
};

class LocalStructArrayOfPointersWithCount : public LocalTestClass
{
public:

    inline LocalStructArrayOfPointersWithCount()
        : basicTypes_CountM(0),
          basicTypeArrays_CountM(0),
          basicTypeArraysWithCount_CountM(0),
          basicTypePointersWithMax_CountM(0),
          basicTypePointersWithCountAndMax_CountM(0)
    {
    }

    virtual ~LocalStructArrayOfPointersWithCount()
    {
        for (s8 i = 0; i < basicTypes_CountM; i++) {
            delete basicTypes[i];
        }
        
        for (u8 i = 0; i < basicTypeArrays_CountM; i++) {
            delete basicTypeArrays[i];
        }

        for (u16 i = 0; i < basicTypeArraysWithCount_CountM; i++) {
            delete basicTypeArraysWithCount[i];
        }

        for (u32 i = 0; i < basicTypePointersWithMax_CountM; i++) {
            delete basicTypePointersWithMax[i];
        }

        for (u32 i = 0; i < basicTypePointersWithCountAndMax_CountM;
             i++) {
            delete basicTypePointersWithCountAndMax[i];
        }
    }

    virtual void Randomize()
    {
        Rand<s8>(basicTypes_CountM, countof(basicTypes));
        for (s8 i = 0; i < basicTypes_CountM; i++) {
            basicTypes[i] = new LocalBasicTypes;
            basicTypes[i]->Randomize();
        }
        
        Rand<u8>(basicTypeArrays_CountM,
                 countof(basicTypeArrays));
        for (u8 i = 0; i < basicTypeArrays_CountM; i++) {
            basicTypeArrays[i] = new BasicTypeArrays;
            basicTypeArrays[i]->Randomize();
        }

        Rand<u16>(basicTypeArraysWithCount_CountM,
                  countof(basicTypeArraysWithCount));
        for (u16 i = 0; i < basicTypeArraysWithCount_CountM; i++) {
            basicTypeArraysWithCount[i] = new BasicTypeArraysWithCount;
            basicTypeArraysWithCount[i]->Randomize();
        }

        Rand<u32>(basicTypePointersWithMax_CountM,
                  countof(basicTypePointersWithMax));
        for (u32 i = 0; i < basicTypePointersWithMax_CountM; i++) {
            basicTypePointersWithMax[i] = new BasicTypePointersWithMax;
            basicTypePointersWithMax[i]->Randomize();
        }

        Rand<u32>(basicTypePointersWithCountAndMax_CountM,
                  countof(basicTypePointersWithCountAndMax));
        for (u32 i = 0; i < basicTypePointersWithCountAndMax_CountM;
             i++) {
            basicTypePointersWithCountAndMax[i] =
                new LocalBasicTypePointersWithCountAndMax;
            basicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

private:

    friend class ::XrttiAccess;
    
    s8 basicTypes_CountM;
    LocalBasicTypes *basicTypes[4];

    u8 basicTypeArrays_CountM;
    BasicTypeArrays *basicTypeArrays[6];

    u16 basicTypeArraysWithCount_CountM;
    BasicTypeArraysWithCount *basicTypeArraysWithCount[10];

    u32 basicTypePointersWithMax_CountM;
    BasicTypePointersWithMax *basicTypePointersWithMax[6];

    u32 basicTypePointersWithCountAndMax_CountM;
    LocalBasicTypePointersWithCountAndMax *
    basicTypePointersWithCountAndMax[4];
};

class LocalStructDoublePointersWithMax : public LocalTestClass
{
public:

    inline LocalStructDoublePointersWithMax()
        : pBasicTypes(NULL),
          pBasicTypeArrays(NULL),
          pBasicTypeArraysWithCount(NULL),
          pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax(NULL)
    {
    }

    virtual ~LocalStructDoublePointersWithMax()
    {
        if (pBasicTypes) {
            for (int i = 0; i < pBasicTypes_Max; i++) {
                delete pBasicTypes[i];
            }
            delete [] pBasicTypes;
        }

        if (pBasicTypeArrays) {
            for (int i = 0; i < pBasicTypeArrays_Max; i++) {
                delete pBasicTypeArrays[i];
            }
            delete [] pBasicTypeArrays;
        }

        if (pBasicTypeArraysWithCount) {
            for (int i = 0; i < pBasicTypeArraysWithCount_Max; i++) {
                delete pBasicTypeArraysWithCount[i];
            }
            delete [] pBasicTypeArraysWithCount;
        }

        if (pBasicTypePointersWithMax) {
            for (int i = 0; i < pBasicTypePointersWithMax_Max; i++) {
                delete pBasicTypePointersWithMax[i];
            }
            delete [] pBasicTypePointersWithMax;
        }

        if (pBasicTypePointersWithCountAndMax) {
            for (int i = 0; i < pBasicTypePointersWithCountAndMax_Max;
                 i++) {
                delete pBasicTypePointersWithCountAndMax[i];
            }
            delete [] pBasicTypePointersWithCountAndMax;
        }
    }

    virtual void Randomize()
    {
        pBasicTypes = new BasicTypes * [pBasicTypes_Max];
        for (int i = 0; i < pBasicTypes_Max; i++) {
            pBasicTypes[i] = new BasicTypes;
            pBasicTypes[i]->Randomize();
        }

        pBasicTypeArrays = new LocalBasicTypeArrays *
            [pBasicTypeArrays_Max];
        for (int i = 0; i < pBasicTypeArrays_Max; i++) {
            pBasicTypeArrays[i] = new LocalBasicTypeArrays;
            pBasicTypeArrays[i]->Randomize();
        }

        pBasicTypeArraysWithCount = new LocalBasicTypeArraysWithCount *
            [pBasicTypeArraysWithCount_Max];
        for (int i = 0; i < pBasicTypeArraysWithCount_Max; i++) {
            pBasicTypeArraysWithCount[i] = new
                LocalBasicTypeArraysWithCount;
            pBasicTypeArraysWithCount[i]->Randomize();
        }

        pBasicTypePointersWithMax = 
            new LocalBasicTypePointersWithMax *
            [pBasicTypePointersWithMax_Max];
        for (int i = 0; i < pBasicTypePointersWithMax_Max; i++) {
            pBasicTypePointersWithMax[i] = new
                LocalBasicTypePointersWithMax;
            pBasicTypePointersWithMax[i]->Randomize();
        }

        pBasicTypePointersWithCountAndMax = 
            new BasicTypePointersWithCountAndMax *
            [pBasicTypePointersWithCountAndMax_Max];
        for (int i = 0; i < pBasicTypePointersWithCountAndMax_Max;
             i++) {
            pBasicTypePointersWithCountAndMax[i] = new
                BasicTypePointersWithCountAndMax;
            pBasicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

private:

    friend class ::XrttiAccess;
    
    static const int pBasicTypes_Max = 4;
    BasicTypes **pBasicTypes;

    static const int pBasicTypeArrays_Max = 6;
    LocalBasicTypeArrays **pBasicTypeArrays;

    static const int pBasicTypeArraysWithCount_Max = 10;
    LocalBasicTypeArraysWithCount **pBasicTypeArraysWithCount;

    static const int pBasicTypePointersWithMax_Max = 6;
    LocalBasicTypePointersWithMax **pBasicTypePointersWithMax;

    static const int pBasicTypePointersWithCountAndMax_Max = 4;
    BasicTypePointersWithCountAndMax **pBasicTypePointersWithCountAndMax;
};

class LocalStructDoublePointersWithCountAndMax : public LocalTestClass
{
public:

    inline LocalStructDoublePointersWithCountAndMax()
        : pBasicTypes_CountM(0),
          pBasicTypes(NULL),
          pBasicTypeArrays_CountM(0),
          pBasicTypeArrays(NULL),
          pBasicTypeArraysWithCount_CountM(0),
          pBasicTypeArraysWithCount(NULL),
          pBasicTypePointersWithMax_CountM(0),
          pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax_CountM(0),
          pBasicTypePointersWithCountAndMax(NULL)
    {
    }

    virtual ~LocalStructDoublePointersWithCountAndMax()
    {
        for (s8 i = 0; i < pBasicTypes_CountM; i++) {
            delete pBasicTypes[i];
        }
        delete [] pBasicTypes;

        for (u8 i = 0; i < pBasicTypeArrays_CountM; i++) {
            delete pBasicTypeArrays[i];
        }
        delete [] pBasicTypeArrays;

        for (u16 i = 0; i < pBasicTypeArraysWithCount_CountM; i++) {
            delete pBasicTypeArraysWithCount[i];
        }
        delete [] pBasicTypeArraysWithCount;

        for (u32 i = 0; i < pBasicTypePointersWithMax_CountM; i++) {
            delete pBasicTypePointersWithMax[i];
        }
        delete [] pBasicTypePointersWithMax;

        for (u32 i = 0; i < pBasicTypePointersWithCountAndMax_CountM;
             i++) {
            delete pBasicTypePointersWithCountAndMax[i];
        }
        delete [] pBasicTypePointersWithCountAndMax;
    }

    virtual void Randomize()
    {
        Rand<s8>(pBasicTypes_CountM, pBasicTypes_Max);
        pBasicTypes = new LocalBasicTypes * [pBasicTypes_CountM];
        for (s8 i = 0; i < pBasicTypes_CountM; i++) {
            pBasicTypes[i] = new LocalBasicTypes;
            pBasicTypes[i]->Randomize();
        }

        Rand<u8>(pBasicTypeArrays_CountM, pBasicTypeArrays_Max);
        pBasicTypeArrays = new BasicTypeArrays * 
            [pBasicTypeArrays_CountM];
        for (u8 i = 0; i < pBasicTypeArrays_CountM; i++) {
            pBasicTypeArrays[i] = new BasicTypeArrays;
            pBasicTypeArrays[i]->Randomize();
        }

        Rand<u16>(pBasicTypeArraysWithCount_CountM,
                  pBasicTypeArraysWithCount_Max);
        pBasicTypeArraysWithCount = new BasicTypeArraysWithCount * 
            [pBasicTypeArraysWithCount_CountM];
        for (u16 i = 0; i < pBasicTypeArraysWithCount_CountM; i++) {
            pBasicTypeArraysWithCount[i] = 
                new BasicTypeArraysWithCount;
            pBasicTypeArraysWithCount[i]->Randomize();
        }

        Rand<u32>(pBasicTypePointersWithMax_CountM,
                  pBasicTypePointersWithMax_Max);
        pBasicTypePointersWithMax = new LocalBasicTypePointersWithMax * 
            [pBasicTypePointersWithMax_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithMax_CountM; i++) {
            pBasicTypePointersWithMax[i] =
                new LocalBasicTypePointersWithMax;
            pBasicTypePointersWithMax[i]->Randomize();
        }

        Rand<u32>(pBasicTypePointersWithCountAndMax_CountM,
                  pBasicTypePointersWithCountAndMax_Max);
        pBasicTypePointersWithCountAndMax = 
            new BasicTypePointersWithCountAndMax * 
            [pBasicTypePointersWithCountAndMax_CountM];
        for (u32 i = 0; i < pBasicTypePointersWithCountAndMax_CountM;
             i++) {
            pBasicTypePointersWithCountAndMax[i] =
                new BasicTypePointersWithCountAndMax;
            pBasicTypePointersWithCountAndMax[i]->Randomize();
        }
    }

private:

    friend class ::XrttiAccess;
    
    static const int pBasicTypes_Max = 4;
    s8 pBasicTypes_CountM;
    LocalBasicTypes **pBasicTypes;

    static const int pBasicTypeArrays_Max = 6;
    u8 pBasicTypeArrays_CountM;
    BasicTypeArrays **pBasicTypeArrays;

    static const int pBasicTypeArraysWithCount_Max = 10;
    u16 pBasicTypeArraysWithCount_CountM;
    BasicTypeArraysWithCount **pBasicTypeArraysWithCount;

    static const int pBasicTypePointersWithMax_Max = 6;
    u32 pBasicTypePointersWithMax_CountM;
    LocalBasicTypePointersWithMax **pBasicTypePointersWithMax;

    static const int pBasicTypePointersWithCountAndMax_Max = 4;
    u32 pBasicTypePointersWithCountAndMax_CountM;
    BasicTypePointersWithCountAndMax **pBasicTypePointersWithCountAndMax;
};

class LocalDeepInnerBasicType : public LocalTestClass
{
public:

    virtual void Randomize()
    {
        Rand<u64>(outer.middle.inner.u64v);
    }

private:

    friend class ::XrttiAccess;

    struct {
        typedef struct {
            int a;
        } wonky;
        wonky w;
        class {
        public:
            struct {
                u64 u64v;
            } inner;
        } middle;
    } outer;
};

class LocalDeepInnerStructAndHierarchy : public LocalTestClass
{
public:

    virtual void Randomize()
    {
        outer.middle.Randomize();
    }

private:

    friend class ::XrttiAccess;

    struct {
        class {
        public:

            void Randomize()
            {
                inner.upper.Randomize();
                inner.middle.Randomize();
                inner.bottom.Randomize();
            }

            struct {

                class Upper
                {
                public:

                    void Randomize()
                    {
                        upperOuter.Randomize();
                    }

                    class UpperOuter
                    {
                    public:

                        void Randomize()
                        {
                            serialized.middle.Randomize();

                            serialized.bottom.Randomize();

                            RandReal<double>(transient_double);
                        }

                        class Middle
                        {
                        public:

                            void Randomize()
                            {
                                Rand<u8>(u8v);
                            }
                            
                        private:

                            friend class ::XrttiAccess;

                            u8 u8v;
                        };

                        class Bottom : public Middle
                        {
                        public:

                            void Randomize()
                            {
                                RandReal<float>(serialized.fl);
                            }

                        private:

                            friend class ::XrttiAccess;

                            struct {
                                float fl;
                            } serialized;
                        };

                        struct {
                            Middle middle;
                            Bottom bottom;
                        } serialized;

                        double transient_double;
                    };

                    UpperOuter upperOuter;
                };

                class Middle : public Upper
                {
                public:

                    void Randomize()
                    {
                        Rand<u16>(serialized.u16v);
                        
                        RandReal<double>(transient_double);
                    }

                private:

                    friend class ::XrttiAccess;
                    
                    struct {
                        u16 u16v;
                    } serialized;

                    double transient_double;
                };
                
                class Bottom : public Middle
                {
                public:

                    void Randomize()
                    {
                        Rand<u32>(serialized.u32v);

                        RandReal<double>(transient_double);
                    }

                private:

                    friend class ::XrttiAccess;
                    
                    struct {
                        u32 u32v;
                    } serialized;

                    double transient_double;
                };
                
                Upper upper;
                Middle middle;
                Bottom bottom;
            } inner;
        } middle;
    } outer;
};

class LocalBigArray : public LocalTestClass
{
public:

    virtual void Randomize()
    {
        u32 array_count;
        Rand<u32>(array_count, countof(array) - 1);

        for (u32 i = 0; i < array_count; i++) {
            array[i] = (Random(0xFFFFFFFF) % ('z' - 'a')) + 'a';
        }
        array[array_count] = 0;

        for (u32 i = 0; i < countof(array2); i++) {
            RandReal<float>(array2[i]);
        }
    }

private:

    friend class ::XrttiAccess;

    u8 array[1005];

    float array2[1020];
};

class LocalSimpleStructArray : public LocalTestClass
{
public:
    
    virtual void Randomize()
    {
        Rand<u32>(simpleStructs_CountM, countof(simpleStructs));

        for (u32 i = 0; i < simpleStructs_CountM; i++) {
            Rand<u32>(simpleStructs[i].val1);
            Rand<u32>(simpleStructs[i].val2);
            Rand<u32>(simpleStructs[i].val3);
        }
    }

private:

    class SimpleStruct
    {
    public:

        u32 val1, val2, val3;
    };

    friend class ::XrttiAccess;

    u32 simpleStructs_CountM;
    SimpleStruct simpleStructs[100];
};

class LocalOptional : public LocalTestClass
{
public:
    
    LocalOptional()
        : pBasicTypes(NULL), pBasicTypeArrays(NULL), 
          pBasicTypeArraysWithCount(NULL), pBasicTypePointersWithMax(NULL),
          pBasicTypePointersWithCountAndMax(NULL), pStructs(NULL),
          pStructArrays(NULL), pStructPointers(NULL), 
          pStructArraysWithCount(NULL), pStructPointersWithCountAndMax(NULL), 
          pStructArrayOfPointers(NULL), pStructArrayOfPointersWithCount(NULL), 
          pStructDoublePointersWithMax(NULL), 
          pStructDoublePointersWithCountAndMax(NULL),
          pDeepInnerBasicType(NULL), pDeepInnerStructAndHierarchy(NULL), 
          pBigArray(NULL), pSimpleStructArray(NULL)
    {
    }

    virtual ~LocalOptional()
    {
        delete pBasicTypes;
        delete pBasicTypeArrays;
        delete pBasicTypeArraysWithCount;
        delete pBasicTypePointersWithMax;
        delete pBasicTypePointersWithCountAndMax;
        delete pStructs;
        delete pStructArrays;
        delete pStructPointers;
        delete pStructArraysWithCount;
        delete pStructPointersWithCountAndMax;
        delete pStructArrayOfPointers;
        delete pStructArrayOfPointersWithCount;
        delete pStructDoublePointersWithMax;
        delete pStructDoublePointersWithCountAndMax;
        delete pDeepInnerBasicType;
        delete pDeepInnerStructAndHierarchy;
        delete pBigArray;
        delete pSimpleStructArray;
    }

    virtual void Randomize()
    {
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypes = new LocalBasicTypes();
            pBasicTypes->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArrays = new LocalBasicTypeArrays();
            pBasicTypeArrays->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypeArraysWithCount = new LocalBasicTypeArraysWithCount();
            pBasicTypeArraysWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithMax = new LocalBasicTypePointersWithMax();
            pBasicTypePointersWithMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBasicTypePointersWithCountAndMax = new
                LocalBasicTypePointersWithCountAndMax();
            pBasicTypePointersWithCountAndMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructs = new LocalStructs();
            pStructs->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArrays = new LocalStructArrays();
            pStructArrays->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructPointers = new LocalStructPointers();
            pStructPointers->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArraysWithCount = new LocalStructArraysWithCount();
            pStructArraysWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructPointersWithCountAndMax = new
                StructPointersWithCountAndMax();
            pStructPointersWithCountAndMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArrayOfPointers = new LocalStructArrayOfPointers();
            pStructArrayOfPointers->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructArrayOfPointersWithCount = new
                StructArrayOfPointersWithCount();
            pStructArrayOfPointersWithCount->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructDoublePointersWithMax = 
                new LocalStructDoublePointersWithMax();
            pStructDoublePointersWithMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pStructDoublePointersWithCountAndMax = new
                StructDoublePointersWithCountAndMax();
            pStructDoublePointersWithCountAndMax->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pDeepInnerBasicType = new LocalDeepInnerBasicType();
            pDeepInnerBasicType->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pDeepInnerStructAndHierarchy = new DeepInnerStructAndHierarchy();
            pDeepInnerStructAndHierarchy->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pBigArray = new LocalBigArray();
            pBigArray->Randomize();
        }
        if (Random(0xFFFFFFFF) % 2) {
            pSimpleStructArray = new SimpleStructArray();
            pSimpleStructArray->Randomize();
        }
    }

private:

    friend class ::XrttiAccess;

    LocalBasicTypes *pBasicTypes;
    LocalBasicTypeArrays *pBasicTypeArrays;
    LocalBasicTypeArraysWithCount *pBasicTypeArraysWithCount;
    LocalBasicTypePointersWithMax *pBasicTypePointersWithMax;
    LocalBasicTypePointersWithCountAndMax *pBasicTypePointersWithCountAndMax;
    LocalStructs *pStructs;
    LocalStructArrays *pStructArrays;
    LocalStructPointers *pStructPointers;
    LocalStructArraysWithCount *pStructArraysWithCount;
    StructPointersWithCountAndMax *pStructPointersWithCountAndMax;
    LocalStructArrayOfPointers *pStructArrayOfPointers;
    StructArrayOfPointersWithCount *pStructArrayOfPointersWithCount;
    LocalStructDoublePointersWithMax *pStructDoublePointersWithMax;
    StructDoublePointersWithCountAndMax *pStructDoublePointersWithCountAndMax;
    LocalDeepInnerBasicType *pDeepInnerBasicType;
    DeepInnerStructAndHierarchy *pDeepInnerStructAndHierarchy;
    LocalBigArray *pBigArray;
    SimpleStructArray *pSimpleStructArray;
};

class LocalTester : public TestClass
{
public:

    virtual void Randomize()
    {
        localOptionalM.Randomize();
    }

    virtual const char *GetIdentity() const 
    { 
        return "LocalTester";
    }

private:

    friend class ::XrttiAccess;

    LocalOptional localOptionalM;

};

class Abstract
{
public:

    virtual ~Abstract() { }

    virtual void AbstractMethod() = 0;
};

/* static */
inline TestClass *TestClass::Create(bool fixedId, u32 id)
{
    TestClass *pClass;

    if (!fixedId) {
        id = Random(20);
    }

    switch (id) {
    case 0:
        pClass = new BasicTypes();
        break;
    case 1:
        pClass = new BasicTypeArrays();
        break;
    case 2:
        pClass = new BasicTypeArraysWithCount();
        break;
    case 3:
        pClass = new BasicTypePointersWithCount();
        break;
    case 4:
        pClass = new BasicTypePointersWithMax();
        break;
    case 5:
        pClass = new BasicTypePointersWithCountAndMax();
        break;
    case 6:
        pClass = new Structs();
        break;
    case 7:
        pClass = new StructArrays();
        break;
    case 8:
        pClass = new StructPointers();
        break;
    case 9:
        pClass = new StructArraysWithCount();
        break;
    case 10:
        pClass = new StructPointersWithCountAndMax();
        break;
    case 11:
        pClass = new StructArrayOfPointers();
        break;
    case 12:
        pClass = new StructArrayOfPointersWithCount();
        break;
    case 13:
        pClass = new StructDoublePointersWithMax();
        break;
    case 14:
        pClass = new StructDoublePointersWithCountAndMax();
        break;
    case 15:
        pClass = new DeepInnerBasicType();
        break;
    case 16:
        pClass = new DeepInnerStructAndHierarchy();
        break;
    case 17:
        pClass = new BigArray();
        break;
    case 18:
        pClass = new SimpleStructArray();
        break;
    case 19:
        pClass = new Optional();
        break;
    case 20:
        pClass = new LocalTester();
        break;
    default:
        fprintf(stderr, "Unknown id: %lu\n", (unsigned long) id);
        exit(-1);
    }

    pClass->Randomize();

    return pClass;
}

#endif // TEST_CLASSES_H
