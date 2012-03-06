/*****************************************************************************\
 *                                                                           *
 * Types.h                                                                   *
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

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif

typedef uint8_t                u8;      /**< 8-bit unsigned */
typedef int8_t                 s8;      /**< 8-bit signed */
typedef uint16_t              u16;      /**< 16-bit unsigned */
typedef int16_t               s16;      /**< 16-bit signed */
typedef uint32_t              u32;      /**< 32-bit unsigned */
typedef int32_t               s32;      /**< 32-bit signed */
typedef uint64_t              u64;      /**< 64-bit unsigned */
typedef int64_t               s64;      /**< 64-bit signed */
typedef uintptr_t             upt;      /**< Integer same size as pointer */

} // namespace Xrtti

#endif // TYPES_H
