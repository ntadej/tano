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

QDBusPendingReply<bool> RecorderController::isRecording()
{
	return asyncCall(QLatin1String("recording"));
}

QDBusPendingReply<bool> RecorderController::isTimer()
{
	return asyncCall(QLatin1String("timer"));
}

QDBusPendingReply<QString> RecorderController::output()
{
	return asyncCall(QLatin1String("output"));
}

QDBusPendingReply<> RecorderController::record(const QString &channel,
											   const QString &url,
											   const QString &path)
{
	return asyncCall(QLatin1String("record"), channel, url, path);
}

QDBusPendingReply<> RecorderController::refreshTimers()
{
	return asyncCall(QLatin1String("refreshTimer"));
}

QDBusPendingReply<> RecorderController::stop()
{
	return asyncCall(QLatin1String("stop"));
}
