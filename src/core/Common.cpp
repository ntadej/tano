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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLocale>

#include "Config.h"
#include "core/Common.h"
#include "core/Settings.h"

#if WITH_EDITOR_VLCQT
    #include <vlc-qt/Common.h>
    #include <vlc-qt/Instance.h>
#endif

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

QString Tano::locateResource(const QString &file)
{
    QString path;

    if (QFileInfo(file).exists())
        path = QFileInfo(file).absoluteFilePath();

    // Try application exe working path
    else if (QFileInfo(QDir::currentPath() + "/" + file).exists())
        path = QFileInfo(QDir::currentPath() + "/" + file).absoluteFilePath();

    // Try application exe directory
    else if (QFileInfo(QCoreApplication::applicationDirPath() + "/" + file).exists())
        path = QFileInfo(QCoreApplication::applicationDirPath() + "/" + file).absoluteFilePath();

    // Try application exe directory without editor or src for development
    else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/editor", "") + "/" + file).exists())
        path = QFileInfo(QCoreApplication::applicationDirPath().replace("/editor", "") + "/" + file).absoluteFilePath();

    else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "") + "/" + file).exists())
        path = QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "") + "/" + file).absoluteFilePath();

#ifdef Q_WS_X11
    else if (QFileInfo("/usr/bin/" + file).exists())
        path = QFileInfo("/usr/bin/" + file).absoluteFilePath();
#endif

#ifdef DEFAULT_DATA_DIR
    else if (QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + file).exists())
        path = QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + file).absoluteFilePath();
#endif

    return path;
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

QString Tano::settingsPath()
{
    QSettings *settings = new QSettings(QSettings::IniFormat,
                                        QSettings::UserScope,
                                        "Tano",
                                        "Main");
    QString path = settings->fileName().replace("Main.ini", "");
    delete settings;

    return path;
}

QString Tano::timersPath()
{
    return settingsPath() + "recordings.xml";
}

QStringList Tano::vlcQtArgs()
{
    QStringList args;
#if WITH_EDITOR_VLCQT
    args = VlcCommon::args();
#endif

    return args;
}

QString Tano::vlcQtVersionCore()
{
    QString version = "/";
#if WITH_EDITOR_VLCQT
    version = VlcInstance::version();
#endif

    return version;
}

QString Tano::vlcQtVersionLibrary()
{
    QString version = "/";
#if WITH_EDITOR_VLCQT
    version = VlcInstance::libVersion();
#endif

    return version;
}
