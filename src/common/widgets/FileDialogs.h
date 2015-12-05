/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include "common/File.h"

namespace FileDialogs
{
    QString filterByType(const File::Type &type);

    QString openByType(const File::Type &type,
                       const QString &arg = QString());
    QString openDirectory(const QString &dir = QString());
    QString openFile(const QString &file = QString());
    File openChannelsList();
    QString openChannelsSimple(const QString &file = QString());
    QString openSubtitles(const QString &file = QString());
    QString openUrl();
    QString openXmltvFile(const QString &file = QString());

    QString saveByType(const File::Type &type,
                       const QString &arg = QString());
    File saveChannelsList();
    QString saveXmltv(const QString &file = QString());
}

#endif // TANO_FILEDIALOGS_H_
