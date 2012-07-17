/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
* Copyright (C) 2005-2010 Rémi Denis-Courmont
* Copyright (C) 2005-2006 VLC authors and VideoLAN
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtGlobal>

#include <assert.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#if defined(Q_OS_WIN32)
#  include <windows.h>
#  include <io.h>
#endif
#include <errno.h>

#include "core/Unicode.h"
#include "platform/windows/vasprintf.h"

/**
 * Formats an UTF-8 string as vfprintf(), then print it, with
 * appropriate conversion to local encoding.
 */
int utf8_vfprintf( FILE *stream, const char *fmt, va_list ap )
{
#ifndef Q_OS_WIN32
    return vfprintf(stream, fmt, ap);
#else
    char *str;
    char *ansi;
    int res = vasprintf(&str, fmt, ap);
    if (res == -1)
        return -1;

    /* Writing to the console is a lot of fun on Microsoft Windows.
     * If you use the standard I/O functions, you must use the OEM code page,
     * which is different from the usual ANSI code page. Or maybe not, if the
     * user called "chcp". Anyway, we prefer Unicode. */
    int fd = _fileno(stream);
    if (fd != -1 && _isatty(fd)) {
        wchar_t *wide = ToWide(str);
        if (wide != NULL) {
            HANDLE h = (HANDLE)((uintptr_t)_get_osfhandle (fd));
            DWORD out;
            /* XXX: It is not clear whether WriteConsole() wants the number of
             * Unicode characters or the size of the wchar_t array. */
            BOOL ok = WriteConsoleW(h, wide, wcslen(wide), &out, NULL);
            free(wide);
            if (ok)
                goto outpart;
        }
    }

    ansi = ToANSI(str);
    if (ansi != NULL) {
        fputs(ansi, stream);
        free(ansi);
    }
    else
        res = -1;
outpart:
    free(str);
    return res;
#endif
}

/**
 * Formats an UTF-8 string as fprintf(), then print it, with
 * appropriate conversion to local encoding.
 */
int utf8_fprintf(FILE *stream, const char *fmt, ...)
{
    va_list ap;
    int res;

    va_start(ap, fmt);
    res = utf8_vfprintf(stream, fmt, ap);
    va_end(ap);
    return res;
}

char *FromWide(const wchar_t *wide)
{
    size_t len = WideCharToMultiByte(CP_UTF8, 0, wide, -1, NULL, 0, NULL, NULL);
    if (len == 0)
        return NULL;

    char *out = (char *)malloc(len);

    if (out)
        WideCharToMultiByte(CP_UTF8, 0, wide, -1, out, len, NULL, NULL);
    return out;
}

wchar_t *ToWide(const char *utf8)
{
    int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
    if (len == 0)
        return NULL;

    wchar_t *out = (wchar_t *)malloc(len * sizeof (wchar_t));

    if (out)
        MultiByteToWideChar(CP_UTF8, 0, utf8, -1, out, len);
    return out;
}

char *ToCodePage(unsigned cp, const char *utf8)
{
    wchar_t *wide = ToWide(utf8);
    if (wide == NULL)
        return NULL;

    size_t len = WideCharToMultiByte(cp, 0, wide, -1, NULL, 0, NULL, NULL);
    if (len == 0)
        return NULL;

    char *out = (char *)malloc(len);
    if (out != NULL)
        WideCharToMultiByte(cp, 0, wide, -1, out, len, NULL, NULL);
    free(wide);
    return out;
}

char *FromCodePage(unsigned cp, const char *mb)
{
    int len = MultiByteToWideChar(cp, 0, mb, -1, NULL, 0);
    if (len == 0)
        return NULL;

    wchar_t *wide = (wchar_t *)malloc(len * sizeof(wchar_t));
    if (wide == NULL)
        return NULL;
    MultiByteToWideChar(cp, 0, mb, -1, wide, len);

    char *utf8 = FromWide(wide);
    free(wide);
    return utf8;
}

char *FromANSI(const char *ansi)
{
    return FromCodePage(GetACP(), ansi);
}

char *ToANSI(const char *utf8)
{
    return ToCodePage(GetACP(), utf8);
}
