/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_ENUMS_H_
#define TANO_ENUMS_H_

#include <QtCore/QString>

namespace Tano
{
    // App type
    enum AppType
    {
        Player,
        Editor,
        Daemon,
        Mobile
    };

    // ID
    enum Id
    {
        Main,
        Record,
        Schedule
    };

    // FileTypes
    enum FileType
    {
        CSV = 20,
        Directory = 0,
        Files = 1,
        JS = 21,
        M3U = 10,
        M3UClean = 11,
        M3UUdpxy = 12,
        Subtitles = 3,
        TanoOld = 40,
        Url = 2,
        XmltvId = 30
    };

    // Timer types
    enum TimerType
    {
        Once,
        Daily,
        Weekly
    };

    QString timerTypeString(const TimerType &type);
}

#endif // TANO_ENUMS_H_
