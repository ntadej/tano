/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QCryptographicHash>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "Config.h"
#include "Common.h"

QString Tano::name()
{
    return QString(PROJECT_NAME);
}

QString Tano::executable()
{   
#if defined(Q_OS_LINUX)
    return name().toLower();
#else
    return name();
#endif
}

QString Tano::domain()
{
    return "tano.si";
}

QString Tano::localServer()
{
    return QString(name() + "localserver").toLower();
}

QString Tano::version()
{
#ifdef VERSION
    return QString(VERSION);
#else
    return "Unknown";
#endif
}

QString Tano::build()
{
#ifdef VERSION_BUILD
    return "(" + QString(VERSION_BUILD) + ")";
#else
    return "";
#endif
}

QString Tano::changeset()
{
    QString version;

#ifdef VERSION_VCS
    if(QString(VERSION_VCS) != "0" && QString(VERSION_VCS) != "") {
        version.append("(" + QString(VERSION_VCS) + ")");
    }
#endif

    return version;
}

bool Tano::is64bit()
{
    bool bit = false;

#if VERSION_X64
    bit = true;
#endif

    return bit;
}

QString Tano::recordingFileName(const QString &name,
                                const QString &channel,
                                const QDate &date,
                                const QTime &time)
{
    QString f;
    f.append(QString(name).replace(" ", "_"));
    f.append("-");
    f.append(QString(channel).replace(" ", "_"));
    f.append("-");
    f.append(date.toString("yyyyMMdd"));
    f.append("-");
    f.append(time.toString("hhmmss"));

    return f;
}
