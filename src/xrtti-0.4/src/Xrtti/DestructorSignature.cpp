/*****************************************************************************\
 *                                                                           *
 * DestructorSignature.cpp                                                   *
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

static bool ThrowsEqual(u32 count, const DestructorSignature &a,
                        const DestructorSignature &b)
{
    for (u32 i = 0; i < count; i++) {
        bool match = false;
        const Type &aThrow = a.GetThrow(i);
        for (u32 j = 0; j < count; j++) {
            if (Equals(aThrow, b.GetThrow(j))) {
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

bool DestructorSignature::operator ==(const DestructorSignature &other) const
{
    u32 count = this->GetThrowCount();

    if (count != other.GetThrowCount()) {
        return false;
    }

    // The throws might not be in the same order, so check to make sure that
    // the sets are the same.  This is done both ways to ensure that if there
    // are any duplicate throws in one or the other, they are detected.  This
    // algorithm is n^2 and could be made nlogn if performance ends up being
    // a concern.
    return (ThrowsEqual(count, *this, other) && 
            ThrowsEqual(count, other, *this));
}


}; // namespace Xrtti
