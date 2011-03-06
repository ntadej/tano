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

#include "RecorderController.h"

RecorderController::RecorderController(const QString &service,
									   const QString &path,
									   const QDBusConnection &connection,
									   QObject *parent)
	: QDBusAbstractInterface(service,
							 path,
							 staticInterfaceName(),
							 connection,
							 parent) { }
RecorderController::~RecorderController() { }

bool RecorderController::isRecording()
{
	QDBusPendingReply<bool> reply = asyncCall(QLatin1String("recording"));
	return reply.value();
}

bool RecorderController::isTimer()
{
	QDBusPendingReply<bool> reply = asyncCall(QLatin1String("timer"));
	return reply.value();
}

QString RecorderController::output()
{
	QDBusPendingReply<QString> reply = asyncCall(QLatin1String("output"));
	return reply.value();
}

void RecorderController::record(const QString &channel,
								const QString &url,
								const QString &path)
{
	asyncCall(QLatin1String("record"), channel, url, path);
}

void RecorderController::refreshTimers()
{
	asyncCall(QLatin1String("refreshTimer"));
}

void RecorderController::stop()
{
	asyncCall(QLatin1String("stop"));
}
