/*****************************************************************************\
 *                                                                           *
 * Type.cpp                                                                  *
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

#include "Xrtti/Xrtti.h"

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


bool Type::operator ==(const Type &other) const
{
    if (this->GetBaseType() != other.GetBaseType()) {
        return false;
    }

    if (this->IsConst() != other.IsConst()) {
        return false;
    }

    if (this->IsVolatile() != other.IsVolatile()) {
        return false;
    }

    if (this->IsReference() != other.IsReference()) {
        return false;
    }

    u32 count = this->GetArrayOrPointerCount();
    if (count != other.GetArrayOrPointerCount()) {
        return false;
    }

    for (u32 i = 0; i < count; i++) {
        const ArrayOrPointer &arrayOrPointer = this->GetArrayOrPointer(i);
        if (arrayOrPointer.GetType() == ArrayOrPointer::Type_Array) {
            if (((const Array &) arrayOrPointer) != 
                other.GetArrayOrPointer(i)) {
                return false;
            }
        }
        else {
            if (((const Pointer &) arrayOrPointer) != 
                other.GetArrayOrPointer(i)) {
                return false;
            }
        }
    }

    return true;
}


}; // namespace Xrtti
