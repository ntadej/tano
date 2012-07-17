/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_WINDOWS_H_
#define TANO_WINDOWS_H_

#include <QtGlobal>

namespace Tano
{
    namespace Windows
    {
        // Windows specific
#if defined(Q_OS_WIN32)
        void pauseConsole();
        void showConsole();
#endif
    }
}

char *make_utf8_string(const wchar_t *unicode);
wchar_t *make_unicode_string(const unsigned char *utf8);
int utf8_encode(const char *from, char **to);
int utf8_decode(const char *from, char **to);

#endif // TANO_WINDOWS_H_
