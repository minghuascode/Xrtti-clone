/*****************************************************************************\
 *                                                                           *
 * Util.cpp                                                                  *
 *                                                                           *
 * ------------------------------------------------------------------------- *
 * Copyright (C) 2007-2009 Bryan Ischo <bryan@ischo.com>                     *
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "test/Util.h"

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


/* static */
void SeedRandom(u32 seed)
{
    srand(seed);
}

/* static */
u32 CurrentTimeSeed()
{
    struct timeval tval;
    gettimeofday(&tval, NULL);
    // Use milliseconds because it just feels better
    return (tval.tv_sec * 1000) + (tval.tv_usec / 1000);
}

/* static */
u32 Random(u32 max)
{
    u32 result = 0;

    // Calculate the number of times that rand() must be called and its entire
    // return value used
    u32 div = max / RAND_MAX;

    while (div--) {
        result *= RAND_MAX;
        result += rand();
    }

    // Now calculate the fractional amount which must be added
    u32 mod = (max + 1) % RAND_MAX;
    
    if (mod > 1) {
        result += (rand() % mod);
    }

    return result;
}

} // namespace Xrtti
