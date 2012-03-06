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

#include <private/StringUtils.h>

using namespace std;
using namespace Xrtti;


/* static */
void StringUtils::FormatV(wstring &result, const wchar_t *pFormat,
						  va_list valist)
{
    // We support a maximum formatted string length of 16K.  It would be
    // nice to be able to compose the string up in chunks or something, or
    // be able to test how many characters are needed in the result, but
    // vswprintf (and its Windows equivalent) is too dumb.  Eventually this
    // method should be rewritten to be completely self-contained and
    // *correct*.  Note that Windows only supports 1024 bytes anyway so on
    // that platform, this is overkill.
    wchar_t tmpbuf[16384];

#ifdef MSWIN
    wstring format(pFormat);

    u32 len = format.length();

    for (u32 i = 0; i < (len - 1); i++) {
        if (format[i] == '%') {
            if (format[i + 1] == 'c') {
                format[i + 1] = 'C';
            }
            else if (format[i + 1] == 's') {
                format[i + 1] = 'S';
            }
            else if (format[i + 1] == 'C') {
                format[i + 1] = 'c';
            }
            else if (format[i + 1] == 'S') {
                format[i + 1] = 'S';
            }
        }
    }

    pFormat = format.data();

    wvsprintfW(tmpbuf, pFormat, valist);
#else
    vswprintf(tmpbuf, sizeof(tmpbuf) / sizeof(tmpbuf[0]), pFormat, valist);
#endif

    result = tmpbuf;
}

/* static */
s64 StringUtils::ToS64(const wstring &str)
{
    const wchar_t *pValue = str.data();

    bool negative = false;
    s64 ret = 0;

    if (*pValue == '-') {
        negative = true;
        pValue++;
    }

    while (*pValue) {
        ret *= 10;
        ret += (*pValue - '0');
        pValue++;
    }

    return negative ? -ret : ret;
}

/* static */
s64 StringUtils::ToS64(const string &str)
{
    const char *pValue = str.data();

    bool negative = false;
    s64 ret = 0;

    if (*pValue == '-') {
        negative = true;
        pValue++;
    }

    while (*pValue) {
        ret *= 10;
        ret += (*pValue - '0');
        pValue++;
    }

    return negative ? -ret : ret;
}

/* static */
u64 StringUtils::ToU64(const wstring &str)
{
    const wchar_t *pValue = str.data();

    u64 ret = 0;

    while (*pValue) {
        ret *= 10;
        ret += (*pValue - '0');
        pValue++;
    }

    return ret;
}

/* static */
u64 StringUtils::ToU64(const string &str)
{
    const char *pValue = str.data();

    u64 ret = 0;

    while (*pValue) {
        ret *= 10;
        ret += (*pValue - '0');
        pValue++;
    }

    return ret;
}

#ifdef DEBUG
/* static */
void StringUtils::dumpwc(const wchar_t *pWstr)
{
    printf("%s\n", ToString(pWstr).c_str());
    fflush(stdout);
}

/* static */
void StringUtils::dumpws(const std::wstring &str)
{
    dumpwc(str.c_str());
}
#endif
