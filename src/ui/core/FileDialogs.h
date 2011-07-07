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

#ifndef TANO_FILEDIALOGS_H_
#define TANO_FILEDIALOGS_H_

#include <QtCore/QDir>
#include <QtCore/QString>

class File;

namespace FileDialogs
{
    enum Type
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
        Xmltv = 30
    };

    QString filterByType(const Type &type);

    QString openByType(const Type &type,
                       const QString &arg = "");
    QString openDirectory(const QString &dir = QDir::homePath());
    QString openFile();
    File openPlaylist();
    QString openPlaylistSimple();
    QString openSubtitles(const QString &dir = QDir::homePath());
    QString openUrl();

    QString saveByType(const Type &type,
                       const QString &arg = "");
    File savePlaylist();
    QString saveXmltv();
};

#endif // TANO_FILEDIALOGS_H_
