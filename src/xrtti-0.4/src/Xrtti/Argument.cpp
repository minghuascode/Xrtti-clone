/*****************************************************************************\
 *                                                                           *
 * Argument.cpp                                                              *
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

#include <Xrtti/Xrtti.h>

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif

bool Argument::operator ==(const Argument &other) const
{
    if (!Equals(this->GetType(), other.GetType())) {
        return false;
    }

    if (this->HasDefault()) {
        if (!other.HasDefault()) {
            return false;
        }

        // Now compare the actual default values - this will have to be
        // handled individually for each type
        return true;
#if 0
        return (String::Equal
                ((const char *) 
                 (const char *) other.GetDefault().pointer_v));
#endif
    }
    else {
        return (!other.HasDefault());
    }
}


}; // namespace Xrtti
