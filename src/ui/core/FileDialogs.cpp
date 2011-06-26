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

#include <QtCore/QObject>
#include <QtGui/QFileDialog>

#include "FileDialogs.h"

QString FileDialogs::openByType(const Type &type,
                                const QString &arg)
{
    if(type == Directory)
        return openDirectory(arg);
    else if(type == M3U)
        return openM3U();
}

QString FileDialogs::openDirectory(const QString &dir)
{
    QString file = QFileDialog::getExistingDirectory(0, QObject::tr("Open directory"),
                                                    dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    return file;
}

QString FileDialogs::openM3U()
{
    QString file = QFileDialog::getOpenFileName(0, QObject::tr("Open channel list"),
                                                QDir::homePath(),
                                                QObject::tr("Tano TV channel list files(*.m3u)"));
    return file;
}
