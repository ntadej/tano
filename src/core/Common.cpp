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

#include "Config.h"
#include "core/Common.h"

QString Tano::name()
{
#if EDITOR
    return "Tano Editor";
#else
    return "Tano";
#endif
}

QString Tano::nameGroup()
{
    return "Tano";
}

QString Tano::nameShort()
{
#if EDITOR
    return "Editor";
#else
    return "Player";
#endif
}


QString Tano::executable()
{
#if EDITOR
    return "tano-editor";
#else
    return "tano";
#endif
}

QString Tano::version()
{
    QString version;

#ifdef VERSION
    version.append(QString(VERSION));
#else
    version.append("Unknown");
#endif

    return version;
}

QString Tano::changeset()
{
    QString version;

#ifdef VERSION_PATCH
    if(QString(VERSION_PATCH) != "0" && QString(VERSION_PATCH) != "") {
        version.append("(" + QString(VERSION_PATCH) + ")");
    }
#endif

    return version;
}

QString Tano::buildHostname()
{
    QString hostname;

#ifdef HOSTNAME
    hostname.append(QString(HOSTNAME));
#else
    hostname.append("Unknown");
#endif

    return hostname;
}

QString Tano::buildSystem()
{
    QString system;

#ifdef SYSTEM
    system.append(QString(SYSTEM));
#else
    system.append("Unknown");
#endif

    return system;
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
