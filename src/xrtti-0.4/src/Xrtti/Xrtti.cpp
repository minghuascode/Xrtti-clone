/*****************************************************************************\
 *                                                                           *
 * Xrtti.cpp                                                                 *
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

#include <Xrtti/XrttiPrivate.h>
#include <private/Parsed.h>

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


bool Equals(const Context &c1, const Context &c2)
{
    Context::Type t1 = c1.GetType();
    if (t1 != c2.GetType()) {
        return false;
    }

    switch (t1) {
    case Context::Type_Class:
    case Context::Type_Struct:
        return ((const Struct &) c1 == (const Struct &) c2);
    case Context::Type_Namespace:
        return (c1 == c2);
    default: // Type_Union
        return ((const Structure &) c1 == (const Structure &) c2);
    }
}


bool Equals(const Type &type1, const Type &type2)
{
    Type::BaseType t1 = type1.GetBaseType();
    if (t1 != type2.GetBaseType()) {
        return false;
    }

    switch (t1) {
    case Type::BaseType_Enumeration:
        return ((const TypeEnumeration &) type1 ==
                (const TypeEnumeration &) type2);
    case Type::BaseType_Function:
        return ((const TypeFunction &) type1 ==
                (const TypeFunction &) type2);
    case Type::BaseType_Structure:
        return ((const TypeStructure &) type1 ==
                (const TypeStructure &) type2);
    default: // fundamental types
        return (type1 == type2);
    }
}


u32 GetContextCount()
{
    return CompiledContextSet::GetContextCount();
}


const Context *GetContext(u32 index)
{
    return CompiledContextSet::GetContext(index);
}


const Context *LookupContext(const char *pFullName)
{
    return CompiledContextSet::LookupContext(pFullName);
}


const Structure *LookupStructure(const std::type_info &typeinfo)
{
    return CompiledContextSet::LookupStructure(typeinfo);
}


}; // namespace Xrtti
