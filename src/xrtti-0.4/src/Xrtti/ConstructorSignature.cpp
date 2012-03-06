/*****************************************************************************\
 *                                                                           *
 * ConstructorSignature.cpp                                                  *
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


bool ConstructorSignature::
operator ==(const ConstructorSignature &other) const
{
    if (!((DestructorSignature *) this)->DestructorSignature::operator ==
        ((const DestructorSignature &) other)) {
        return false;
    }

    u32 count = this->GetArgumentCount();
    if (count != other.GetArgumentCount()) {
        return false;
    }

    for (u32 i = 0; i < count; i++) {
        if (this->GetArgument(i) != other.GetArgument(i)) {
            return false;
        }
    }

    return (this->HasEllipsis() == other.HasEllipsis());
}


}; // namespace Xrtti
