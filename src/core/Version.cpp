/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
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

#include "Config.h"
#include "core/Version.h"

#include <vlc-qt/Instance.h>

QString Version::version()
{
	QString version;

#ifdef VERSION
	version.append(QString(VERSION));
#else
	version.append("Unknown");
#endif

	return version;
}

QString Version::changeset()
{
	QString version;

#ifdef VERSION_PATCH
	if(QString(VERSION_PATCH) != "0" && QString(VERSION_PATCH) != "") {
		version.append("("+QString(VERSION_PATCH)+")");
	}
#endif

	return version;
}

QString Version::qt()
{
	QString version;

#ifdef QT_VERSION_MAJOR
	version = QString::number(QT_VERSION_MAJOR)+"."+QString::number(QT_VERSION_MINOR)+"."+QString::number(QT_VERSION_PATCH);
#else
	version = "Unknown";
#endif
	
	return version;
}

QString Version::libVlcqt()
{
	return VlcInstance::libVersion();
}

QString Version::libVlc()
{
	return VlcInstance::version();
}
