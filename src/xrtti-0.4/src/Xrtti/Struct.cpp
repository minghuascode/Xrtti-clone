/*****************************************************************************\
 *                                                                           *
 * Struct.cpp                                                                *
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


static bool MethodsEqual(u32 count, const Struct &a, const Struct &b)
{
    // First compare virtual methods in order
    for (u32 i = 0, bIndex = 0; i < count; i++) {
        const Method &aMethod = a.GetMethod(i);
        if (!aMethod.IsVirtual()) {
            continue;
        }
        // Find the next virtual method in b
        bool wasFound = false;
        for ( ; bIndex < count; bIndex++) {
            const Method &bMethod = b.GetMethod(bIndex);
            if (!bMethod.IsVirtual()) {
                continue;
            }
            if (aMethod != bMethod) {
                return false;
            }
            wasFound = true;
            bIndex++;
            break;
        }
        if (!wasFound) {
            // If there is nothing left in b, then there can be no match
            return false;
        }
    }

    // Now compare non-virtual methods.  Since the order might be different
    // for non-virtual methods, a match is done.
    for (u32 i = 0; i < count; i++) {
        const Method &aMethod = a.GetMethod(i);
        if (aMethod.IsVirtual()) {
            continue;
        }
        bool match = false;
        for (u32 j = 0; j < count; j++) {
            if (aMethod == b.GetMethod(j)) {
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


bool Struct::operator ==(const Struct &other) const
{
    if (!((const Structure *) this)->Structure::operator ==
        ((const Structure &) other)) {
        return false;
    }

    if (this->IsAbstract() != other.IsAbstract()) {
        return false;
    }

    u32 count = this->GetMethodCount();
    if (count != other.GetMethodCount()) {
        return false;
    }

    if (!MethodsEqual(count, *this, other) ||
        !MethodsEqual(count, other, *this)) {
        return false;
    }

    return true;
}


}; // namespace Xrtti
