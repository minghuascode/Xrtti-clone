/*****************************************************************************\
 *                                                                           *
 * Context.cpp                                                               *
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

bool Context::operator ==(const Context &other) const
{
    if (this->GetType() != other.GetType()) {
        return false;
    }

    if (strcmp(this->GetName(), other.GetName())) {
        return false;
    }

    if (strcmp(this->GetFullName(), other.GetFullName())) {
        return false;
    }

    const Context *pThisContext = this->GetContext();
    const Context *pOtherContext = other.GetContext();

    // Don't test context recursively, just test that the context names are
    // the same.
    if (pThisContext) {
        return (pOtherContext && !strcmp(pThisContext->GetFullName(),
                                         pOtherContext->GetFullName()));
    }
    else {
        return (!pOtherContext);
    }
}


}; // namespace Xrtti
