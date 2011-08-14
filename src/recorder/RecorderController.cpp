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

#include "daemon/DaemonCommon.h"
#include "recorder/RecorderController.h"

RecorderController::RecorderController(QObject *parent)
    : QDBusAbstractInterface(Tano::Daemon::service(),
                             Tano::Daemon::path(),
                             staticInterfaceName(),
                             QDBusConnection::sessionBus(),
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

QString RecorderController::timerEndTime()
{
	QDBusPendingReply<QString> reply = asyncCall(QLatin1String("timerEndTime"));
	return reply.value();
}

void RecorderController::record(const QString &channel,
								const QString &url,
								const QString &path)
{
	asyncCall(QLatin1String("record"), channel, url, path);
}

void RecorderController::refreshSettings()
{
	asyncCall(QLatin1String("refreshSettings"));
}

void RecorderController::refreshTimers()
{
	asyncCall(QLatin1String("refreshTimers"));
}

void RecorderController::stop()
{
	asyncCall(QLatin1String("stop"));
}

void RecorderController::timerInfo()
{
	asyncCall(QLatin1String("timerInfo"));
}
