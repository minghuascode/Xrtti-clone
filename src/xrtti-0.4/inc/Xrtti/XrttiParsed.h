/*****************************************************************************\
 *                                                                           *
 * XrttiParsed.h                                                             *
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


#ifndef XRTTI_PARSED_H
#define XRTTI_PARSED_H

#include <Xrtti/Xrtti.h>


/** **************************************************************************
 * Everything which follows is in the Xrtti C++ namespace.
 ************************************************************************** **/

namespace Xrtti {

#if 0 // This fixes indentation in emacs
}
#endif


/** **************************************************************************
 * ContextSet is a set of all Xrtti objects which have been loaded from
 * parsed header files.  New Xrtti objects can be added by parsing new header
 * files and the resulting set of contexts (and all associated objects)
 * comprise the ContextSet.
 ************************************************************************** **/
class ContextSet
{
public:

    /**
     * Destructor
     **/
    virtual ~ContextSet() { }

    /**
     * Adds a header file to the set; the headef file is parsed and all C++
     * structural information is added to the ContextSet (essentially, all
     * Contexts defined in the header file, and all objects these reference).
     * If the header file conflicts in any way with a previously added header
     * file, or if it cannot be read, this returns false.
     *
     * @param file is the header file to read and parse
     * @param includeCount is the number of elements in the pIncludes
     *        array parameter
     * @param pIncludes is an array of zero-terminated strings, each of which
     *        is a path to search for included header files when parsing
     *        the header file at [path]
     * @param definitionCount is the number of elements in the pDefinitions
     *        array parameter
     * @param pDefinitions is an array of zero-terminated strings, each of
     *        which is a preprocessor macro to define while processing the
     *        header file at [path]
     * @param tmpfile is a temporary file to use; defaults to gccxml.out
     * @return true on success, false on error
     **/
    virtual bool AddHeader(const char *file, u32 includeCount,
                           const char **pIncludes, u32 definitionCount,
                           const char **pDefinitions, const char *tmpfile) = 0;

    /**
     * Returns a string describing the error which caused the most recent
     * call to AddHeader() to return false.
     *
     * @return a string describing the error which caused the most recent
     *         call to AddHeader() to return false.
     **/
    virtual const char *GetLastError() const = 0;

    /**
     * Returns the number of Contexts in this set.
     * 
     * @return the number of Contexts in this set.
     **/
    virtual u32 GetContextCount() const = 0;
    
    /**
     * Returns a Context.  The total number of available contexts is available
     * via a call to GetContextCount().
     *
     * @param index is the index of the Context to get; this value must be
     *        less than the number of Contexts returned by GetContextCount().
     * @return the index'th Context. 
     **/
    virtual const Context *GetContext(u32 index) const = 0;
    
    /**
     * Looks up a Context by full name, and returns it.  Returns NULL if there
     * is no such Context in the ContextSet.
     *
     * @param pFullName is the full name (i.e. foo::bar::baz for C++, or just
     *        the struct/union name for C) of the Context to look up and
     *        return
     * @return the Context with the given full name, or NULL if there is no
     *         such Context
     **/
    virtual const Context *LookupContext(const char *pFullName) const = 0;
};


/**
 * Creates and returns a new ContextSet, which can then be used to parse
 * header files and examine the resulting Xrtti Contexts and their associated
 * objects.
 *
 * @return a new ContextSet, which can then be used to parse
 *         header files and examine the resulting Xrtti Contexts and their
 *         associated objects.
 **/
ContextSet *CreateContextSet();


}; // namespace Xrtti


#endif // XRTTI_PARSED_H
