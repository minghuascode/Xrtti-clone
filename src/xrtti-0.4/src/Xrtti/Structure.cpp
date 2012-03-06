/*****************************************************************************\
 *                                                                           *
 * Structure.cpp                                                             *
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


bool ConstructorsEqual(u32 count, const Structure &a, const Structure &b)
{
    for (u32 i = 0; i < count; i++) {
        bool match = false;
        const Constructor &aConstructor = a.GetConstructor(i);
        for (u32 j = 0; j < count; j++) {
            if (aConstructor == b.GetConstructor(j)) {
                match = true;
                break;
            }
        }
        if (!match) {
            return false;
        }
    }

    return true;
}

bool Structure::operator ==(const Structure &other) const
{
    if (!((const Context *) this)->Context::operator ==
        ((const Context &) other)) {
        return false;
    }

    if (this->GetAccessType() != other.GetAccessType()) {
        return false;
    }

    u32 count = this->GetBaseCount();
    if (count != other.GetBaseCount()) {
        return false;
    }
    
    // Bases must be in the same order
    for (u32 i = 0; i < count; i++) {
        if (this->GetBase(i) != other.GetBase(i)) {
            return false;
        }
    }

    count = this->GetFieldCount();
    if (count != other.GetFieldCount()) {
        return false;
    }
    // Fields must be in the same order
    for (u32 i = 0; i < count; i++) {
        if (this->GetField(i) != other.GetField(i)) {
            return false;
        }
    }

    if (this->IsAnonymous() != other.IsAnonymous()) {
        return false;
    }

    count = this->GetConstructorCount();
    if (count != other.GetConstructorCount()) {
        return false;
    }

    // Constructors can be in any order
    if (!ConstructorsEqual(count, *this, other) ||
        !ConstructorsEqual(count, other, *this)) {
        return false;
    }

    if (this->HasDestructor()) {
        if (!other.HasDestructor() || 
            (this->GetDestructor() != other.GetDestructor())) {
            return false;
        }
    }
    else if (other.HasDestructor()) {
        return false;
    }

    if (this->IsCreatable() != other.IsCreatable()) {
        return false;
    }

    return (this->IsDeletable() == other.IsDeletable());
}


}; // namespace Xrtti
