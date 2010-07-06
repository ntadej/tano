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
#include "Version.h"

#if DESKTOP
	#include <QVlc/Instance.h>
#endif

QString Version::Tano()
{
	QString version;
	int svn;

#ifdef VERSION
	version = QString(VERSION);
#else
	version = "Unknown";
#endif
#ifdef SVN_REVISION
	svn = SVN_REVISION;
#else
	svn = 0;
#endif

	if(svn != 0)
		return version+"-r"+QString::number(svn);
	else
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

QString Version::libQVlc()
{
#if DESKTOP
	return QVlc::Instance::version();
#else
	return QString().number(0);
#endif
}

QString Version::libVLC()
{
#if DESKTOP
	return QVlc::Instance::libVlcVersion();
#else
	return QString().number(0);
#endif
}
