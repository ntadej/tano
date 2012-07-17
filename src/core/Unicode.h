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

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/**
 * Formats an UTF-8 string as vfprintf(), then print it, with
 * appropriate conversion to local encoding.
 */
int utf8_vfprintf(FILE *stream, const char *fmt, va_list ap);

/**
 * Formats an UTF-8 string as fprintf(), then print it, with
 * appropriate conversion to local encoding.
 */
int utf8_fprintf(FILE *stream, const char *fmt, ...);

char *FromWide(const wchar_t *wide);
wchar_t *ToWide(const char *utf8);
char *ToCodePage(unsigned cp, const char *utf8);
char *FromCodePage(unsigned cp, const char *mb);
char *FromANSI(const char *ansi);
char *ToANSI(const char *utf8);
