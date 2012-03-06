/*****************************************************************************\
 *                                                                           *
 * Compiled.cpp                                                              *
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
 * These are private data structure definitions shared between               *
 * xrttigen-generated code and the xrtti library.                            *
 *                                                                           *
 * THIS HEADER FILE IS NOT MEANT TO BE INSPECTED OR USED BY ANY END-USER OF  *
 * THE XRTTI LIBRARY.  IGNORE THE CONTENTS OF THIS FILE.                     *
 *                                                                           *
\*****************************************************************************/

#include <Xrtti/XrttiPrivate.h>


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


const Base &CompiledStructure::GetBase(u32 index) const
{
    return *(pBasesM[index]);
}


const Structure &CompiledStructure::GetFriend(u32 index) const
{
    return *(pFriendsM[index]);
}


const Field &CompiledStructure::GetField(u32 index) const
{
    return *(pFieldsM[index]);
}


const Constructor &CompiledStructure::GetConstructor(u32 index) const
{
    return *(pConstructorsM[index]);
}


const Destructor &CompiledStructure::GetDestructor() const
{
    return *pDestructorM;
}


const Method &CompiledStruct::GetMethod(u32 index) const
{
    return *(pMethodsM[index]);
}


const Type &CompiledField::GetType() const
{
    return *pTypeM;
}


const Type &CompiledArgument::GetType() const
{
    return *pTypeM;
}


const Type &CompiledMethodSignature::GetReturnType() const
{
    return *pReturnTypeM;
}


const ArrayOrPointer &CompiledType::GetArrayOrPointer(u32 index) const
{
    return *(pArrayOrPointersM[index]);
}

const EnumerationValue &CompiledEnumeration::GetValue(u32 index) const
{
    return *(pValuesM[index]);
}


CompiledRegister::CompiledRegister(u32 contextCount, Context **pContexts)
{
    for (u32 i = 0; i < contextCount; i++) {
        CompiledContextSet::RegisterContext(pContexts[i]);
    }
}


}; // namespace Xrtti
