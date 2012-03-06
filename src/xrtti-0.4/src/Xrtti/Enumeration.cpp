/*****************************************************************************\
 *                                                                           *
 * Enumeration.cpp                                                           *
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

#include <string.h>
#include "Xrtti/Xrtti.h"


namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif

static bool EnumerationValuesEqual(u32 count, const Enumeration &a,
                                   const Enumeration &b)
{
    for (u32 i = 0; i < count; i++) {
        bool match = false;
        const EnumerationValue &aValue = a.GetValue(i);
        for (u32 j = 0; j < count; j++) {
            if (aValue == b.GetValue(j)) {
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

bool Enumeration::operator ==(const Enumeration &other) const
{
    if (this->GetAccessType() != other.GetAccessType()) {
        return false;
    }

    if (strcmp(this->GetContext().GetFullName(),
               other.GetContext().GetFullName())) {
        return false;
    }

    if (strcmp(this->GetName(), other.GetName())) {
        return false;
    }

    u32 count = this->GetValueCount();
    if (count != other.GetValueCount()) {
        return false;
    }

    // The enumeration values might not be in the same order, so test them in
    // an order-independent manner.  This algorithm is n^2 and could be made
    // nlogn if performance ends up being a concern.
    return (EnumerationValuesEqual(count, *this, other) &&
            EnumerationValuesEqual(count, other, *this));
}


}; // namespace Xrtti
