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

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include "Config.h"

#include "Common.h"
#include "Log.h"
#include "Out.h"
#include "Resources.h"

#if BRANDING && BRANDING_LOG
    #include <QtNetwork/QNetworkAccessManager>
    #include <QtNetwork/QNetworkRequest>

    #include "branding/Log.h"

    QNetworkAccessManager *nam;
#endif

QTextStream *out;

#if defined(Qt5)
void Tano::Log::output(QtMsgType type,
                       const QMessageLogContext &context,
                       const QString &msg)
#elif defined(Qt4)
void Tano::Log::output(QtMsgType type,
                       const char *msg)
#endif
{
#if defined(Qt5)
    Q_UNUSED(context)
#endif

    QString debugdate = QDateTime::currentDateTime().toString("yyyy.MM.dd hh:mm:ss");
    switch (type)
    {
    case QtDebugMsg:
        debugdate += " [D]";
        break;
    case QtWarningMsg:
        debugdate += " [W]";
        break;
    case QtCriticalMsg:
        debugdate += " [C]";
        break;
    case QtFatalMsg:
        debugdate += " [F]";
    }
    (*out) << debugdate << " " << msg << endl;

#ifdef QT_DEBUG
    Out(true) << debugdate << " " << msg << endl;
#endif

    if (QtFatalMsg == type) {
        abort();
    }
}

void Tano::Log::setup()
{
#if LOGGING
    QString fileName = Tano::Resources::settingsPath() + "/" + Tano::executable() + ".log";
    QFile *log = new QFile(fileName);
    if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        out = new QTextStream(log);
#if defined(Qt5)
        qInstallMessageHandler(output);
#elif defined(Qt4)
        qInstallMsgHandler(output);
#endif
    } else {
        qDebug() << "Error opening log file '" << fileName << "'. All debug output redirected to console.";
    }
#endif

#if BRANDING && BRANDING_LOG
    nam = new QNetworkAccessManager();
#endif
}

void Tano::Log::login()
{
    qDebug() << "Login successful!";

#if BRANDING && BRANDING_LOG
    nam->get(QNetworkRequest(QUrl(Tano::BrandingLog::login())));
#endif
}

void Tano::Log::logout()
{
    qDebug() << "Logout";

#if BRANDING && BRANDING_LOG
    nam->get(QNetworkRequest(QUrl(Tano::BrandingLog::logout())));
#endif
}

void Tano::Log::playingChannel(int id)
{
    qDebug() << "Playing channel #" << id;

#if BRANDING && BRANDING_LOG
    nam->get(QNetworkRequest(QUrl(Tano::BrandingLog::playingChannel(id))));
#endif
}

void Tano::Log::playlistLoaded()
{
    qDebug() << "Playlist loaded from network";

#if BRANDING && BRANDING_LOG
    nam->get(QNetworkRequest(QUrl(Tano::BrandingLog::playlistLoaded())));
#endif
}

void Tano::Log::stopped()
{
    qDebug() << "Stopped";

#if BRANDING && BRANDING_LOG
   // nam->get(QNetworkRequest(QUrl(Tano::BrandingLog::stopped())));
#endif
}
