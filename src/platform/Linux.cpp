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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include "platform/Linux.h"

#if defined(Q_OS_LINUX)
QString Tano::Linux::picturesPath()
{
    QString path = "Pictures";
    QFile file(QDir::homePath() + "/.config/user-dirs.dirs");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return path;

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        QString tmp = in.readLine();
        if (tmp.contains("XDG_PICTURES_DIR")) {
            tmp.replace("XDG_PICTURES_DIR=", "");
            tmp.replace("\"", "");
            tmp.replace("$HOME/", "");

            path = tmp;
            break;
        }
    }

    return path;
}
#endif

#if defined(Q_OS_LINUX)
QString Tano::Linux::videoPath()
{
    QString path = "Videos";
    QFile file(QDir::homePath() + "/.config/user-dirs.dirs");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return path;

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        QString tmp = in.readLine();
        if (tmp.contains("XDG_VIDEOS_DIR")) {
            tmp.replace("XDG_VIDEOS_DIR=", "");
            tmp.replace("\"", "");
            tmp.replace("$HOME/", "");

            path = tmp;
            break;
        }
    }

    return path;
}
#endif
