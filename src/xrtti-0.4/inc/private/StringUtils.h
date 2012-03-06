/*****************************************************************************\
 *                                                                           *
 * StringUtils.h                                                             *
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

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <algorithm>
#include <stdarg.h>
#include <string>
#include "private/Types.h"
#include "Xrtti/XrttiParsed.h"


namespace Xrtti
{

/** **************************************************************************
 * @class StringUtils
 *
 * Utility string methods.
 *
 ************************************************************************** **/
class StringUtils
{
public:

    /**
     * Creates a wide version of a narrow string.
     *
     * @param str is the string to create a wide version of
     * @return a wide version of the string
     **/
    static std::wstring FromString(const std::string &str)
    {
        std::wstring tmp(str.length(), L' ');
        std::copy(str.begin(), str.end(), tmp.begin());
        return tmp;
    }

    /**
     * Creates a wide version of a narrow string.
     *
     * @param pString is the NULL-terminated narrow string to create a wide
     *        version of
     * @return a wide version of the string
     **/
    static std::wstring FromString(const char *pString)
    {
        return FromString(std::string(pString));
    }

    /**
     * Creates a narrow version of a wide string.
     *
     * @param str is the wstring to create a narrow version of
     * @return a narrow version of the wstring
     **/
    static std::string ToString(const std::wstring &str)
    {
        std::string tmp(str.length(), ' ');
        std::copy(str.begin(), str.end(), tmp.begin());
        return tmp;
    }

    /**
     * Creates a narrow version of a wide string.
     *
     * @param pString is the NULL-terminated wide string to create a narrow
     *        version of
     * @return a narrow version of the wstring
     **/
    static std::string ToString(const wchar_t *pString)
    {
        return ToString(std::wstring(pString));
    }

    /**
     * This returns true if the string starts with the given prefix.
     *
     * @param str is the string to test
     * @param pPrefix is the null-terminated prefix to test against
     * @return true if the string starts with the given prefix
     **/
    static bool StartsWith(const std::wstring &str, const wchar_t *pPrefix)
    {
        return StartsWith(str, pPrefix,
                          std::char_traits<wchar_t>::length(pPrefix));
    }

    /**
     * This returns true if the string starts with the given prefix.
     *
     * @param str is the string to test
     * @param pPrefix is the null-terminated prefix to test against
     * @return true if the string starts with the given prefix
     **/
    static bool StartsWith(const std::string &str, const char *pPrefix)
    {
        return StartsWith(str, pPrefix,
                          std::char_traits<char>::length(pPrefix));
    }

    /**
     * This returns true if the string starts with the given prefix.
     *
     * @param str is the string to test
     * @param pPrefix is the prefix to test against
     * @param prefixLen is the number of characters in pPrefix to test against
     * @return true if the string starts with the given prefix
     **/
    static bool StartsWith(const std::wstring &str, const wchar_t *pPrefix, 
                           u32 prefixLen)
    {
        return (str.compare(0, prefixLen, pPrefix) == 0);
    }

    /**
     * This returns true if the string starts with the given prefix.
     *
     * @param str is the string to test
     * @param pPrefix is the prefix to test against
     * @param prefixLen is the number of characters in pPrefix to test against
     * @return true if the string starts with the given prefix
     **/
    static bool StartsWith(const std::string &str, const char *pPrefix, 
                           u32 prefixLen)
    {
        return (str.compare(0, prefixLen, pPrefix) == 0);
    }

    /**
     * This returns true if the string starts with the given prefix.
     *
     * @param str is the string to test
     * @param prefix is the prefix to test against
     * @return true if the string starts with the given prefix
     **/
    static bool StartsWith(const std::wstring &str, const std::wstring &prefix)
    {
        return StartsWith(str, prefix.data(), prefix.length());
    }

    /**
     * This returns true if the string starts with the given prefix.
     *
     * @param str is the string to test
     * @param prefix is the prefix to test against
     * @return true if the string starts with the given prefix
     **/
    static bool StartsWith(const std::string &str, const std::string &prefix)
    {
        return StartsWith(str, prefix.data(), prefix.length());
    }

    /**
     * This returns true if the string ends with the given suffix.
     *
     * @param str is the string to test
     * @param pSuffix is the null-terminated suffix to test against
     * @return true if the string ends with the given suffix
     **/
    static bool EndsWith(const std::wstring &str, const wchar_t *pSuffix)
    {
        return EndsWith(str, pSuffix, 
                        std::char_traits<wchar_t>::length(pSuffix));
    }

    /**
     * This returns true if the string ends with the given suffix.
     *
     * @param str is the string to test
     * @param pSuffix is the null-terminated suffix to test against
     * @return true if the string ends with the given suffix
     **/
    static bool EndsWith(const std::string &str, const char *pSuffix)
    {
        return EndsWith(str, pSuffix, 
                        std::char_traits<char>::length(pSuffix));
    }

    /**
     * This returns true if the string ends with the given suffix.
     *
     * @param str is the string to test
     * @param pSuffix is the suffix to test against
     * @param suffixLen is the number of characters in pSuffix to test against
     * @return true if the string ends with the given suffix
     **/
    static bool EndsWith(const std::wstring &str, const wchar_t *pSuffix,
                         u32 suffixLen)
    {
        u32 strLen = str.length();

        if (suffixLen > strLen) {
            return false;
        }
        
        return (str.compare(strLen - suffixLen, suffixLen, pSuffix) == 0);
    }

    /**
     * This returns true if the string ends with the given suffix.
     *
     * @param str is the string to test
     * @param pSuffix is the suffix to test against
     * @param suffixLen is the number of characters in pSuffix to test against
     * @return true if the string ends with the given suffix
     **/
    static bool EndsWith(const std::string &str, const char *pSuffix,
                         u32 suffixLen)
    {
        u32 strLen = str.length();

        if (suffixLen > strLen) {
            return false;
        }
        
        return (str.compare(strLen - suffixLen, suffixLen, pSuffix) == 0);
    }

    /**
     * This returns true if the string ends with the given suffix.
     *
     * @param str is the string to test
     * @param suffix is the suffix to test against
     * @return true if the string ends with the given suffix
     **/
    static bool EndsWith(const std::wstring &str, const std::wstring &suffix)
    {
        return EndsWith(str, suffix.data(), suffix.length());
    }

    /**
     * This returns true if the string ends with the given suffix.
     *
     * @param str is the string to test
     * @param suffix is the suffix to test against
     * @return true if the string ends with the given suffix
     **/
    static bool EndsWith(const std::string &str, const std::string &suffix)
    {
        return EndsWith(str, suffix.data(), suffix.length());
    }

    static std::wstring Format(const wchar_t *pFormat, ...)
    {
        va_list valist;
    
        va_start(valist, pFormat);

        std::wstring result;

        FormatV(result, pFormat, valist);

        va_end(valist);

        return result;
    }

    static std::wstring FormatV(const wchar_t *pFormat, va_list valist)
    {
        std::wstring result;

        FormatV(result, pFormat, valist);

        return result;
    }

    static void Format(std::wstring &result, const wchar_t *pFormat, ...)
    {
        va_list valist;

        va_start(valist, pFormat);

        FormatV(result, pFormat, valist);

        va_end(valist);
    }

    static void FormatV(std::wstring &result, const wchar_t *pFormat,
                        va_list valist);

    static s8 ToS8(const std::wstring &str)
    {
        return (s8) ToS64(str);
    }

    static s8 ToS8(const std::string &str)
    {
        return (s8) ToS64(str);
    }

    static u8 ToU8(const std::wstring &str)
    {
        return (u8) ToU64(str);
    }

    static u8 ToU8(const std::string &str)
    {
        return (u8) ToU64(str);
    }

    static s16 ToS16(const std::wstring &str)
    {
        return (s16) ToS64(str);
    }

    static s16 ToS16(const std::string &str)
    {
        return (s16) ToS64(str);
    }

    static u16 ToU16(const std::wstring &str)
    {
        return (u16) ToU64(str);
    }

    static u16 ToU16(const std::string &str)
    {
        return (u16) ToU64(str);
    }

    static s32 ToS32(const std::wstring &str)
    {
        return (s32) ToS64(str);
    }

    static s32 ToS32(const std::string &str)
    {
        return (s32) ToS64(str);
    }

    static u32 ToU32(const std::wstring &str)
    {
        return (u32) ToU64(str);
    }

    static u32 ToU32(const std::string &str)
    {
        return (u32) ToU64(str);
    }

    static s64 ToS64(const std::wstring &str);

    static s64 ToS64(const std::string &str);

    static u64 ToU64(const std::wstring &str);

    static u64 ToU64(const std::string &str);

#ifdef DEBUG
    static void dumpwc(const wchar_t *pWstr);
    static void dumpws(const std::wstring &str);
#endif

};

} // namespace Xrtti


#endif // STRING_UTILS_H
