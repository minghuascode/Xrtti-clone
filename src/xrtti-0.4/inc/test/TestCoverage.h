/*****************************************************************************\
 *                                                                           *
 * TestCoverage.h                                                            *
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

#ifndef TEST_COVERAGE_H
#define TEST_COVERAGE_H

// This is a stress test of specific xrtti functionality.  It is meant to
// demonstrate correctness of xrtti; any bugs should end up with code in here
// to run through xrtti and demonstrate that the bug has been fixed.  The
// TestCoverage program should produce this same file, after running this
// file through cpp, and ignoring minor whitespace variations.  So to get the
// "golden" file do something like:
// cpp - - < TestCoverage.h > TestCoverage.golden
// TestCoverage > TestCoverage.out
// Then diff the two.  Ignore whitespace.  This can be most easily
// accomplished by using indent on both resulting files; indent will give both
// files almost exactly the same whitespace.  Then tell your diffing program
// to ignore the other whitespace differences, and the two files should end up
// exactly the same, with no diffs.
//
// Note that any time a class or anything is added to this header file, it
// will almost inevitably break the diff because it is not easy to add new
// code here that is in exactly the same format as the TestCoverage program
// outputs.  So after the first run, look at the diffs.  For anything that is
// just a difference of syntax, should be modified in this header file to
// match the output of TestCoverage.  Things like, putting things in the
// right order, declaring default constructors, etc.

// ***************************************************************************
// All function types used must be typedef'd here before use
// ***************************************************************************

typedef int (function_1) (int, char **, int = 100);
typedef int (function_2) (int);

struct XrttiAccess;

// Tests anonymous structures
typedef struct YAnonymousStructs
{
    // This doesn't actually define any fields of this struct, it just
    // uselessly defines a struct
    typedef struct HasAName
    {
        HasAName(const HasAName &_ctor_arg);

        HasAName();

        int field_a;
    } HasAName;
    
    typedef struct HasANameToo
    {
        HasANameToo(const HasANameToo &_ctor_arg);

        HasANameToo();

        int field_a;
    } HasANameToo;

    YAnonymousStructs(const YAnonymousStructs &_ctor_arg);

    YAnonymousStructs();

    HasANameToo field_j;

    int field_a;

    HasAName field_h;

    struct
    {
        int method_a(int arg_a, int arg_b) throw(int, const void *);
        
        int method_b(int arg_a, int arg_b) throw(HasANameToo);

        int field_a;
    } field_i;

    struct
    {
        int field_b;

        struct
        {
            int field_c;
        };

        struct
        {
            int field_a;
        } field_d;

        struct
        {
            int field_e;
            
            struct
            {
                int field_f;
            };
            
            struct
            {
                int field_a;
            } field_g;
        };
    };

#if 0
    // Weird; in C++ this is supposed to just declare the definition of a
    // structure named CStyleAnonymous; this seems to actually result in
    // a field here though.  Bug???
    struct CStyleAnonymous;
#endif

    struct
    {
        typedef struct wonky
        {
            int a;
        } wonky;

        wonky w;

        class
        {
            struct
            {
                int s32;
            } inner;
        } middle;
    } outer;

} YAnonymousStructs;

// basic types
typedef struct YBasicTypes
{
    YBasicTypes(const YBasicTypes &_ctor_arg);

    YBasicTypes(int a);

    YBasicTypes(float f = 1.234, const char *pc = "a string");

    ~YBasicTypes();

    const void *field_a;

    bool field_b;

    const char field_c;

    char field_d;

    unsigned char field_e;

    const wchar_t field_f;

    short field_g;

    unsigned short field_h;

    const int field_i;

    int field_j;

    unsigned int field_k;

    long field_l;

    const unsigned long field_m;

    long long field_n;

    unsigned long long field_o;

    float field_p;

    double field_q;

    const long double field_r;

    int field_s:3;

    const int field_t:5;

    static int field_u;
} YBasicTypes;

// YSimpleClass - tests class definition, no base classes, and one field and
// method member of each access type (public, protected, private).
class YSimpleClass
{
    enum Enum {
        a = 0,
        b = 1,
        c = 2
    };

public:

    enum Enum2 {
        d = 3,
        e = 4
    };
    
    YSimpleClass(const YSimpleClass &_ctor_arg);

    YSimpleClass();
    
    virtual ~YSimpleClass();
    
    int method_a(int arg_a, int arg_b) const;

    virtual int method_b(int arg_a = 100, int arg_b = 200);

protected:

    static int method_c(int arg_a, int arg_b);

    virtual int method_d(int arg_a, int arg_b) const;

private:

    int method_e(int arg_a, int arg_b);

    virtual int method_f(int arg_a, int arg_b);

    const char *method_g();

public:

    Enum **** const *** volatile ** const volatile *field_a[][4][15][1];

protected:

    Enum2 field_b;

private:
    
    function_1 *field_c;

    friend class ::XrttiAccess;
};

// DerivedClass - tests class derivation, and fields and methods with the same
// name and different names as the parent class.
class YDerivedClass : protected virtual YSimpleClass
{
public:

    YDerivedClass(const YDerivedClass &_ctor_arg);

    YDerivedClass();
    
    virtual ~YDerivedClass();

    int method_a(int arg_a, int arg_b);
    
    virtual int method_b(int arg_a, int arg_b);

    int method_g(int arg_a, int arg_b);
    
    virtual int method_h(int arg_a, int arg_b);

    const YSimpleClass get_simple_class();

    const YSimpleClass &get_simple_class_ref();

    int field_a;

    YSimpleClass field_d;
};

// Test random namespace placement
namespace YFirstNamespace
{
    // Tests that abstract classes work
    class AbstractClass : public YDerivedClass
    {
    public:

        AbstractClass(const AbstractClass &_ctor_arg);

        AbstractClass();

        virtual ~AbstractClass();

        virtual int method_i() = 0;
    };

    namespace SecondNamespace
    {
        // Tests classes defined within other classes
        class ClassDefiner
        {
            class Defined
            {
            public:

                Defined(const Defined &_ctor_arg);

                Defined();
                
                int field_a;
            };
            
            ClassDefiner(const ClassDefiner &_ctor_arg);

            ClassDefiner();

            class
            {
            public:
                
                class
                {
                public:
                    int field_a;
                } field_a;
            } field_b;
            
            class
            {
            public:
                
                int method_a(int arg_a, int arg_b);
                
                int method_b(int arg_a, int arg_b);

                int field_a;
            } AnonymousClass;

            friend class ::XrttiAccess;
        };
        
#if 0

        class ClassDefiner2
        {
        public:
            
            ClassDefiner::Defined field_a;
        };
        
#endif
        
        class InaccessibleClassDefiner
        {
            class InaccessiblyDefined
            {
            };
        };
    }
}

// Tests that incomplete classes work
struct YIncompleteClass;

// Tests offsetofs
typedef struct YOffsetofs
{
    int a;

    float b;

    char c;
} YOffsetofs;

// This class is used just for SecondMultiYDerivedClass
class YSecondBaseClass : public virtual YSimpleClass
{
public:

    YSecondBaseClass(const YSecondBaseClass &_ctor_arg);

    YSecondBaseClass();

    virtual ~YSecondBaseClass();

    virtual int method_b(int arg_a, int arg_b);

    int field_a;
};

// Tests that each field type works.
// SecondMultiYDerivedClass - tests a class with second level derivation, as
// well as multiple inheritence.
class YSecondMultiDerivedClass : public virtual YDerivedClass, public virtual YSecondBaseClass
{
public:
    
    YSecondMultiDerivedClass(const YSecondMultiDerivedClass &_ctor_arg);

    YSecondMultiDerivedClass();

    virtual ~YSecondMultiDerivedClass();

    int method_a(int arg_a, int arg_b);
    
    virtual int method_b(int arg_a, int arg_b);

    int method_g(int arg_a, int arg_b);

    virtual int method_h(int arg_a, int arg_b);

    int field_a;

    int field_d;

    function_2 **pafuncs[10];
};

// Unions defined within a class.
class YUnionDefiner
{
public:

    // Simple basic union
    typedef union SimpleUnion
    {
        int field_a;
        int field_b;
    } SimpleUnion;

    YUnionDefiner(const YUnionDefiner &_ctor_arg);

    YUnionDefiner();
    
    int field_a;

    union
    {
        int field_b;
        int field_c;
    };

    union
    {
        int field_a;
        int field_b;
    } union_b;

    SimpleUnion simpleUnion;
};


#endif // TEST_COVERAGE_H
