/*****************************************************************************\
 *                                                                           *
 * Util.h                                                                    *
 *                                                                           *
 * ------------------------------------------------------------------------- *
 * Copyright (C) 2009 Bryan Ischo <bryan@ischo.com>                          *
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

#ifndef UTIL_H
#define UTIL_H

#include "private/Types.h"

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


/**
 * Defines a countof() operator like C's sizeof() operator, but countof()
 * works on arrays ONLY and returns the number of elements in the array.  This
 * may be used anywhere that a statically defined array's size is needed.
 *
 * @param a is the array whose count is to be determed; this array must have
 *        been declared with a specific element count for this macro to work
 * @return the number of elements in the array
 **/
#define countof(array) (sizeof(array) / sizeof(*(array)))


/**
 * Seeds the random number generator with a specific value.
 *
 * @param seed is the seed to use
 **/
void SeedRandom(u32 seed = 1);


/**
 * Returns a random number seed based on the current time, to be used in
 * a call like:
 * SeedRandom(CurrentTimeSeed());
 *
 * @return a random number seed based on the current time
 **/
u32 CurrentTimeSeed();


/**
 * Obtains a random number between 0 and max.  This is better than relying
 * on POSIX rand() because POSIX rand() only returns a value between 0 and
 * RAND_MAX, which could be any number 32768 or greater.  Thus if you need
 * random values between 0 and N, this method will conveniently provide
 * them to you by calling rand() multiple times if necessary to get all of
 * the bits required.  It is most efficient to request a max that is
 * accurate for your needs, rather than just specifying 0xFFFFFFFF and
 * then doing your own mod.
 *
 * Note that this method is not thread-safe; you should use the
 * thread-safe version in the Thread class in multi-threaded programs.
 *
 * @param max defines the range of numbers in which the returned random
 *        number will lie, between 0 and max (inclusive)
 * @return a random number between 0 and max, inclusive
 **/
u32 Random(u32 max = 0xFFFFFFFF);

} // namespace Xrtti

#endif // UTIL_H

