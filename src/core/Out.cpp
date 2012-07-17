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

#include <QtCore/QTextCodec>

#ifdef Q_OS_WIN32
#   include <windows.h>
#   include <iostream>
#endif

#include "core/Out.h"

Out::Out(const bool &error)
    : QTextStream(error ? stderr : stdout, QIODevice::WriteOnly)
{
    setCodec(QTextCodec::codecForName("UTF-8"));
}

Out::~Out() { }

Out& Out::operator<<(const QString &string)
{
#ifdef Q_OS_WIN32
    WriteConsoleW(GetStdHandle(STD_OUTPUT_HANDLE), string.utf16(), string.size(), NULL, NULL);
#else
    QString msgstr = codec()->toUnicode(string.toAscii());
    QTextStream::operator<<(msgstr);

    flush();
#endif

    return *this;
}
