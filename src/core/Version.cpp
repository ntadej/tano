/****************************************************************************
* Version.cpp: Version info
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "Config.h"
#include "core/Version.h"

#if DESKTOP
	#include <vlc-qt/Instance.h>
#endif

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
	if(QString(VERSION_PATCH) != "0") {
		version.append("("+QString(VERSION_PATCH)+")");
	}
#endif

	return version;
}

QString Version::Qt()
{
	QString version;

#ifdef QT_VERSION_MAJOR
	version = QString::number(QT_VERSION_MAJOR)+"."+QString::number(QT_VERSION_MINOR)+"."+QString::number(QT_VERSION_PATCH);
#else
	version = "Unknown";
#endif
	
	return version;
}

QString Version::libVLCQt()
{
#if DESKTOP
	return VlcInstance::libVersion();
#else
	return QString("Unknown");
#endif
}

QString Version::libVLC()
{
#if DESKTOP
	return VlcInstance::version();
#else
	return QString("Unknown");
#endif
}
