/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
* Copyright (C) Peter Harris <peter.harris@hummingbird.com>
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

#include "core/Unicode.h"
#include "platform/windows/Windows.h"

#if defined(Q_OS_WIN32)
#   include <windows.h>
#endif

#if defined(Q_OS_WIN32)
void Tano::Windows::pauseConsole()
{
    if(getenv("PWD")) return; /* Cygwin shell or Wine */

    utf8_fprintf(stderr, "\nPress the RETURN key to continue...\n");
    getchar();
    fclose(stdout);
}
#endif

#if defined(Q_OS_WIN32)
void Tano::Windows::showConsole()
{
    if(getenv( "PWD" )) return; /* Cygwin shell or Wine */

    AllocConsole();

    SetConsoleOutputCP(GetACP());
    SetConsoleTitle("Tano");

    freopen("CONOUT$", "w", stderr);
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
}
#endif

char *make_utf8_string(const wchar_t *unicode)
{
    int size = 0, index = 0, out_index = 0;
    unsigned char *out;
    unsigned short c;

    /* first calculate the size of the target string */
    c = unicode[index++];
    while(c) {
        if(c < 0x0080) {
            size += 1;
        } else if(c < 0x0800) {
            size += 2;
        } else {
            size += 3;
        }
        c = unicode[index++];
    }

    out = (unsigned char *)malloc(size + 1);
    if (out == NULL)
        return NULL;
    index = 0;

    c = unicode[index++];
    while(c)
    {
        if(c < 0x080) {
            out[out_index++] = (unsigned char)c;
        } else if(c < 0x800) {
            out[out_index++] = 0xc0 | (c >> 6);
            out[out_index++] = 0x80 | (c & 0x3f);
        } else {
            out[out_index++] = 0xe0 | (c >> 12);
            out[out_index++] = 0x80 | ((c >> 6) & 0x3f);
            out[out_index++] = 0x80 | (c & 0x3f);
        }
        c = unicode[index++];
    }
    out[out_index] = 0x00;

    return (char *)out;
}

wchar_t *make_unicode_string(const unsigned char *utf8)
{
    int size = 0, index = 0, out_index = 0;
    wchar_t *out;
    unsigned char c;

    /* first calculate the size of the target string */
    c = utf8[index++];
    while(c) {
        if((c & 0x80) == 0) {
            index += 0;
        } else if((c & 0xe0) == 0xe0) {
            index += 2;
        } else {
            index += 1;
        }
        size += 1;
        c = utf8[index++];
    }

    out = (wchar_t *)malloc((size + 1) * sizeof(wchar_t));
    if (out == NULL)
        return NULL;
    index = 0;

    c = utf8[index++];
    while(c)
    {
        if((c & 0x80) == 0) {
            out[out_index++] = c;
        } else if((c & 0xe0) == 0xe0) {
            out[out_index] = (c & 0x1F) << 12;
            c = utf8[index++];
            out[out_index] |= (c & 0x3F) << 6;
            c = utf8[index++];
            out[out_index++] |= (c & 0x3F);
        } else {
            out[out_index] = (c & 0x3F) << 6;
            c = utf8[index++];
            out[out_index++] |= (c & 0x3F);
        }
        c = utf8[index++];
    }
    out[out_index] = 0;

    return out;
}

int utf8_encode(const char *from, char **to)
{
    wchar_t *unicode;
    int wchars, err;

    wchars = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, from,
            strlen(from), NULL, 0);

    if(wchars == 0)
    {
        fprintf(stderr, "Unicode translation error %d\n", GetLastError());
        return -1;
    }

    unicode = (wchar_t *)calloc(wchars + 1, sizeof(unsigned short));
    if(unicode == NULL)
    {
        fprintf(stderr, "Out of memory processing string to UTF8\n");
        return -1;
    }

    err = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, from,
            strlen(from), unicode, wchars);
    if(err != wchars)
    {
        free(unicode);
        fprintf(stderr, "Unicode translation error %d\n", GetLastError());
        return -1;
    }

    /* On NT-based windows systems, we could use WideCharToMultiByte(), but
     * MS doesn't actually have a consistent API across win32.
     */
    *to = make_utf8_string(unicode);

    free(unicode);
    return 0;
}

int utf8_decode(const char *from, char **to)
{
    wchar_t *unicode;
    int chars, err;

    /* On NT-based windows systems, we could use MultiByteToWideChar(CP_UTF8), but
     * MS doesn't actually have a consistent API across win32.
     */
    unicode = make_unicode_string((const unsigned char*)from);
    if(unicode == NULL)
    {
        fprintf(stderr, "Out of memory processing string from UTF8 to UNICODE16\n");
        return -1;
    }

    chars = WideCharToMultiByte(GetConsoleCP(), WC_COMPOSITECHECK, unicode,
            -1, NULL, 0, NULL, NULL);

    if(chars == 0)
    {
        fprintf(stderr, "Unicode translation error %d\n", GetLastError());
        free(unicode);
        return -1;
    }

    *to = (char *)calloc(chars + 1, sizeof(unsigned char));
    if(*to == NULL)
    {
        fprintf(stderr, "Out of memory processing string to local charset\n");
        free(unicode);
        return -1;
    }

    err = WideCharToMultiByte(GetConsoleCP(), WC_COMPOSITECHECK, unicode,
            -1, *to, chars, NULL, NULL);
    if(err != chars)
    {
        fprintf(stderr, "Unicode translation error %d\n", GetLastError());
        free(unicode);
        free(*to);
        *to = NULL;
        return -1;
    }

    free(unicode);
    return 0;
}
