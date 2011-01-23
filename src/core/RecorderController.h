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
#include <QtDBus/QDBusPendingReply>

class RecorderController : public QDBusAbstractInterface
{
Q_OBJECT
public:
	RecorderController(const QString &service,
					   const QString &path,
					   const QDBusConnection &connection,
					   QObject *parent = 0);
	~RecorderController();

	static const char *staticInterfaceName() { return "si.tano.TanoPlayer.Recorder"; }

public slots:
	QDBusPendingReply<bool> isRecording();
	QDBusPendingReply<bool> isTimer();
	QDBusPendingReply<QString> output();

	QDBusPendingReply<> record(const QString &channel,
							   const QString &url,
							   const QString &path);
	QDBusPendingReply<> refreshBackend();
	QDBusPendingReply<> refreshTimers();
	QDBusPendingReply<> stop();

signals:
	void elapsed(const int &);
};

#endif // TANO_RECORDERCONTROLLER_H_
