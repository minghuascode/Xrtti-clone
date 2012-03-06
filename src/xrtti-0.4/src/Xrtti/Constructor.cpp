/*****************************************************************************\
 *                                                                           *
 * Constructor.cpp                                                           *
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


bool Constructor::operator ==(const Constructor &other) const
{
    if (!((const Member *) this)->Member::operator ==((const Member &) other)) {
        return false;
    }

    if (this->GetSignature() != other.GetSignature()) {
        return false;
    }

    u32 count = this->GetSignature().GetArgumentCount();
    for (u32 i = 0; i < count; i++) {
        if (strcmp(this->GetArgumentName(i), other.GetArgumentName(i))) {
            return false;
        }
    }

    return (this->IsInvokeable() == other.IsInvokeable());
}


}; // namespace Xrtti
