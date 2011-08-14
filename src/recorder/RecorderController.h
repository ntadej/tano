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

#ifndef TANO_RECORDERCONTROLLER_H_
#define TANO_RECORDERCONTROLLER_H_

#include <QtDBus/QDBusAbstractInterface>
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusPendingReply>

class RecorderController : public QDBusAbstractInterface
{
Q_OBJECT
public:
    RecorderController(QObject *parent = 0);
	~RecorderController();

    static const char *staticInterfaceName() { return "si.Tano.Recorder"; }

public slots:
	bool isRecording();
	bool isTimer();
	QString output();
	QString timerEndTime();

	void record(const QString &channel,
				const QString &url,
				const QString &path);
	void refreshSettings();
	void refreshTimers();
	void stop();
	void timerInfo();

signals:
	void elapsed(const int &);
	void timer(const QString &,
			   const QString &);
	void timerStop();
};

#endif // TANO_RECORDERCONTROLLER_H_
