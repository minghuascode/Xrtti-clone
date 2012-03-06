/*****************************************************************************\
 *                                                                           *
 * TestCompiled.cpp                                                          *
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
 * See TestCompiled.h                                                        *
 *                                                                           *
\*****************************************************************************/

#include <Xrtti/Xrtti.h>
#include "test/TestCoverage.h"
#include "test/XrttiToCpp.h"

using namespace Xrtti;

/** **************************************************************************
 * Method definitions
 ************************************************************************** **/

YAnonymousStructs::HasAName::HasAName(const HasAName & /* _ctor_arg */)
{
}

YAnonymousStructs::HasAName::HasAName()
{
}

YAnonymousStructs::HasANameToo::HasANameToo(const HasANameToo & /* _ctor_arg */)
{
}

YAnonymousStructs::HasANameToo::HasANameToo()
{
}

YAnonymousStructs::YAnonymousStructs(const YAnonymousStructs & /* _ctor_arg */)
{
}

YAnonymousStructs::YAnonymousStructs()
{
}

int YBasicTypes::field_u;

YBasicTypes::YBasicTypes(const YBasicTypes & /* _ctor_arg */)
    : field_a((void *) 0), field_c('1'), field_f(2), field_i(3), field_m(4),
      field_r(5.0), field_t(6)
{
}

YBasicTypes::YBasicTypes(int a)
    : field_a((void *) 0), field_c('1'), field_f(2), field_i(3), field_m(4),
      field_r(5.0), field_t(6)
{
    field_j = a;
}

YBasicTypes::YBasicTypes(float f, const char * /* pc */)
    : field_a((void *) 0), field_c('1'), field_f(2), field_i(3), field_m(4),
      field_p(f), field_r(5.0), field_t(6)
{
}

YBasicTypes::~YBasicTypes()
{
}

YSimpleClass::YSimpleClass(const YSimpleClass & /* _ctor_arg */)
{
}

const YSimpleClass YDerivedClass::get_simple_class()
{
	return field_d;
}

const YSimpleClass &YDerivedClass::get_simple_class_ref()
{
	return field_d;
}

YSimpleClass::YSimpleClass()
{
}

YSimpleClass::~YSimpleClass()
{
}

int YSimpleClass::method_a(int arg_a, int arg_b) const
{
    return arg_a + arg_b;
}

int YSimpleClass::method_b(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YSimpleClass::method_c(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YSimpleClass::method_d(int arg_a, int arg_b) const
{
    return arg_a + arg_b;
}

int YSimpleClass::method_e(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YSimpleClass::method_f(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

const char *YSimpleClass::method_g()
{
    return "a";
}

YDerivedClass::~YDerivedClass()
{
}

int YDerivedClass::method_a(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YDerivedClass::method_b(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YDerivedClass::method_g(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YDerivedClass::method_h(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

YDerivedClass::YDerivedClass(const YDerivedClass & /* _ctor_arg */)
	: YSimpleClass()
{
}

YDerivedClass::YDerivedClass()
{
}

YFirstNamespace::AbstractClass::AbstractClass(const AbstractClass & 
											 /* _ctor_arg */)
	: YSimpleClass(), YDerivedClass()
{
}

YFirstNamespace::AbstractClass::AbstractClass()
{
}

YFirstNamespace::AbstractClass::~AbstractClass()
{
}

YFirstNamespace::SecondNamespace::ClassDefiner::
ClassDefiner(const ClassDefiner & /* _ctor_arg */)
{
}

YFirstNamespace::SecondNamespace::ClassDefiner::ClassDefiner()
{
}

YFirstNamespace::SecondNamespace::ClassDefiner::Defined::
Defined(const Defined & /* _ctor_arg */)
{
}

YFirstNamespace::SecondNamespace::ClassDefiner::Defined::Defined()
{
}

YSecondBaseClass::YSecondBaseClass(const YSecondBaseClass & /* _ctor_arg */)
	: YSimpleClass()
{
}

YSecondBaseClass::YSecondBaseClass()
{
}

YSecondBaseClass::~YSecondBaseClass()
{
}

int YSecondBaseClass::method_b(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

YSecondMultiDerivedClass::
YSecondMultiDerivedClass(const YSecondMultiDerivedClass & /* _ctor_arg */)
	: YSimpleClass(), YDerivedClass(), YSecondBaseClass()
{
}

YSecondMultiDerivedClass::YSecondMultiDerivedClass()
{
}

YSecondMultiDerivedClass::~YSecondMultiDerivedClass()
{
}

int YSecondMultiDerivedClass::method_a(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YSecondMultiDerivedClass::method_b(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YSecondMultiDerivedClass::method_g(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

int YSecondMultiDerivedClass::method_h(int arg_a, int arg_b)
{
    return arg_a + arg_b;
}

YUnionDefiner::YUnionDefiner(const YUnionDefiner & /* _ctor_arg */)
{
}

YUnionDefiner::YUnionDefiner()
{
}

int main(int /* argc */, char ** /* argv */)
{
    XrttiToCpp(stdout);

    return 0;
}
