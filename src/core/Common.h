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

#ifndef TANO_COMMON_H_
#define TANO_COMMON_H_

#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTime>

namespace Tano
{
    QString application();
    QString applicationGroup();
    QString applicationShort();
    QString executable();

    // Version
    QString version();
    QString changeset();

    // Build system information
    QString buildHostname();
    QString buildSystem();

    // Resources locators
    QString locateResource(const QString &file);
    QString settingsPath();
    QString timersPath();

    // Misc
    QString recordingFileName(const QString &name,
                              const QString &channel,
                              const QDate &date,
                              const QTime &time);

    // Backend settings
    QStringList vlcQtArgs();
    QString vlcQtVersionCore();
    QString vlcQtVersionLibrary();

    // System specific
#if defined(Q_OS_LINUX)
    QString linuxPicturesPath();
    QString linuxVideoPath();
#endif
}

#endif // TANO_COMMON_H_
