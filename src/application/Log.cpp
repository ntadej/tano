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

#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QMutex>
#include <QtCore/QStandardPaths>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include "Config.h"

#include "application/Common.h"
#include "application/Log.h"
#include "application/Output.h"

QTextStream *out;
QMutex *outMutex;

void Tano::Log::output(QtMsgType type,
                       const QMessageLogContext &context,
                       const QString &msg)
{
    Q_UNUSED(context)

    QMutexLocker locker(outMutex);

    QString debugdate = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch (type)
    {
#if QT_VERSION >= 0x050500
    case QtInfoMsg:
        debugdate += " [I]";
        break;
#endif
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
    Output(true) << debugdate << " " << msg << endl;
#endif

    if (QtFatalMsg == type) {
        abort();
    }
}

void Tano::Log::setup()
{
#if LOGGING
#if QT_VERSION < 0x050400
    QString directory = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
#else
    QString directory = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
#endif
    QString fileName = directory + "/" + Tano::executable() + ".log";
    QFile *log = new QFile(fileName);
    if (log->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        out = new QTextStream(log);
        outMutex = new QMutex;
        qInstallMessageHandler(output);
    } else {
        qDebug() << "Error opening log file '" << fileName << "'. All debug output redirected to console.";
    }
#endif
}
