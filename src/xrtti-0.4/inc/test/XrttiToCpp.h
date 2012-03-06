/*****************************************************************************\
 *                                                                           *
 * XrttiToCpp.h                                                              *
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

#include <stdio.h>
#include "Xrtti/XrttiParsed.h"

// Converts all contexts compiled into an application back into C++.
// These are all of the Contexts available via Xrtti::GetContext()
void XrttiToCpp(FILE *file);

// Converts all contexts loaded into a ContextSet back into C++.
void XrttiToCpp(const Xrtti::ContextSet &set, FILE *file);
