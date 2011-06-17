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

#ifndef TANO_DBUSADAPTORRECORDER_H_
#define TANO_DBUSADAPTORRECORDER_H_

#include <QtCore/QObject>
#include <QtDBus/QtDBus>

#include "daemon/recorder/RecorderCore.h"

class DaemonMain;

class DBusAdaptorRecorder : public QDBusAbstractAdaptor
{
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "si.Tano.Recorder")

public:
    DBusAdaptorRecorder(RecorderCore *recorder, DaemonMain *daemon);
    ~DBusAdaptorRecorder();

public slots:
    QString output() const { return _core->output(); }
    void record(const QString &channel,
                const QString &url,
                const QString &path) { _core->record(channel, url, path); }
    bool recording() const { return _core->isRecording(); }
    void refreshSettings() { _core->settings(); }
    void refreshTimers() { _core->updateTimers(); }
    void stop() { _core->stop(); }
    bool timer() const { return _core->isTimer(); }
    QString timerEndTime() const { return _core->timerEndTime(); }
    void timerInfo() { _core->timerInfo(); }

signals:
    void elapsed(const int &);
    void timer(const QString &,
               const QString &);
    void timerStop();

private:
    RecorderCore *_core;
};

#endif // TANO_DBUSADAPTORRECORDER_H_
