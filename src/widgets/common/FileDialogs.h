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

#ifndef TANO_FILEDIALOGS_H_
#define TANO_FILEDIALOGS_H_

#include <QtCore/QDir>
#include <QtCore/QString>

#include "core/File.h"

#include "WidgetsSharedExport.h"

namespace FileDialogs
{
    QString TANO_WIDGETS_EXPORT filterByType(const File::Type &type);

    QString TANO_WIDGETS_EXPORT openByType(const File::Type &type,
                       const QString &arg = "");
    QString TANO_WIDGETS_EXPORT openDirectory(const QString &dir = QDir::homePath());
    QString TANO_WIDGETS_EXPORT openFile();
    File TANO_WIDGETS_EXPORT openPlaylist();
    QString TANO_WIDGETS_EXPORT openPlaylistSimple();
    QString TANO_WIDGETS_EXPORT openSubtitles(const QString &dir = QDir::homePath());
    QString TANO_WIDGETS_EXPORT openUrl();
    QString TANO_WIDGETS_EXPORT openXmltvFile(const QString &file = QDir::homePath() + "xmltv.xml");

    QString TANO_WIDGETS_EXPORT saveByType(const File::Type &type,
                       const QString &arg = "");
    File TANO_WIDGETS_EXPORT savePlaylist();
    QString TANO_WIDGETS_EXPORT saveXmltv();
}

#endif // TANO_FILEDIALOGS_H_
