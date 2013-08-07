/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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
#include <QtNetwork/QNetworkInterface>

#if defined(Q_OS_MAC)
    #include "platform/OSX.h"
#endif

#include "Config.h"
#include "Common.h"
#include "plugins/Plugins.h"

QString Tano::name()
{
    return globalConfig ? globalConfig->name() : "Tano";
}

QString Tano::executable()
{   
#if defined(Q_OS_LINUX)
    return name().toLower();
#else
    return name();
#endif
}

QString Tano::version()
{
    return globalConfig ? globalConfig->version() : versionCore();
}

QString Tano::versionCore()
{
#ifdef VERSION
    return QString(VERSION);
#else
    return "Unknown";
#endif
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

bool Tano::is64bit()
{
    bool bit = false;

#if VERSION_X64
    bit = true;
#endif

    return bit;
}

QString Tano::uid()
{
#if defined(Q_OS_LINUX)
    QFile file("/var/lib/dbus/machine-id");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return QString("error");

    QTextStream in(&file);
    QString uid = in.readLine();
#elif defined(Q_OS_MAC)
    QString uid = Tano::OSX::uuid();
#else
    QString uid("error");
    foreach (QNetworkInterface netInterface, QNetworkInterface::allInterfaces()) {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            uid = netInterface.hardwareAddress();
    }
#endif

    return QString(QCryptographicHash::hash(uid.toLocal8Bit(), QCryptographicHash::Md5).toHex());
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
