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

#include "Config.h"

#include <QtGlobal>

#include <assert.h>
#include <sys/types.h>
#if defined(Q_OS_WIN32)
#  include <io.h>
#endif
#include <errno.h>
#include <wctype.h>

#include "core/Unicode.h"

/**
 * Formats an UTF-8 string as vfprintf(), then print it, with
 * appropriate conversion to local encoding.
 */
int utf8_vfprintf(FILE *stream, const char *fmt, va_list ap)
{
#if ASSUME_UTF8
    return vfprintf(stream, fmt, ap);
#elif defined(Q_OS_WIN32)
    /* Writing to the console is a lot of fun on Microsoft Windows.
     * If you use the standard I/O functions, you must use the OEM code page,
     * which is different from the usual ANSI code page. Or maybe not, if the
     * user called "chcp". Anyway, we prefer Unicode. */
    char *str;
    int res;
    int fd = _fileno (stream);
    if (likely(fd != -1) && _isatty (fd))
    {
        res = vasprintf (&str, fmt, ap);
        if (unlikely(res == -1))
            return -1;

        size_t wlen = 2 * (res + 1);
        wchar_t *wide = malloc (wlen);
        if (likely(wide != NULL))
        {
            wlen = MultiByteToWideChar (CP_UTF8, 0, str, res + 1, wide, wlen);
            if (wlen > 0)
            {
                HANDLE h = (HANDLE)(intptr_t)_get_osfhandle (fd);
                DWORD out;

                WriteConsoleW (h, wide, wlen - 1, &out, NULL);
            }
            else
                res = -1;
            free (wide);
        }
        else
            res = -1;
        free (str);
        return res;
    }
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
