/****************************************************************************
* Common.cpp: Basic functions for Tano application
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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLocale>

#include "Config.h"
#include "Common.h"
#include "core/Settings.h"

#if DESKTOP
	#include "ui/About.h"
#endif

void Common::about(QWidget *parent)
{
#if DESKTOP
	About about(parent);
	about.exec();
#endif
}

QString Common::locateResource(const QString &file)
{
	QString path;

	if (QFileInfo(file).exists())
		path = QFileInfo(file).absoluteFilePath();

	// Try application exe working path
	else if (QFileInfo(QDir::currentPath() + "/" + file).exists())
		path = QFileInfo(QDir::currentPath() + "/" + file).absoluteFilePath();

	// Try application exe directory
	else if (QFileInfo(QCoreApplication::applicationDirPath() + "/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/" + file).absoluteFilePath();

	// Try application exe directory without src for development
	else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/src","") + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath().replace("/src","") + file).absoluteFilePath();

#ifdef Q_WS_X11
		else if (QFileInfo("/usr/bin/" + file).exists())
			path = QFileInfo("/usr/bin/" + file).absoluteFilePath();
#endif

#ifdef DEFAULT_DATA_DIR
	else if (QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + file).exists())
		path = QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + file).absoluteFilePath();
#endif

#if MOBILE
	else if (QFileInfo("/usr/local/share/tano/" + file).exists())
		path = QFileInfo("/usr/local/share/tano/" + file).absoluteFilePath();
#endif

	return path;
}

QString Common::locateTranslation(const QString &file)
{
	QString path = locateResource("/lang/" + file);

	return path.replace(QString("/" + file), QString(""));
}

QList<const char *> Common::libvlcArgs()
{
	QList<const char *> args;

	Settings *s = new Settings();
	if(s->globalSettings())
		args << "--ignore-config";
	delete s;

	args << "--intf=dummy"
		 << "--no-media-library"
		 << "--reset-plugins-cache"
		 << "--no-stats"
		 << "--no-osd"
		 << "--no-video-title-show";

#ifdef Q_WS_WIN
	args << "--plugin-path=vlc\\plugins\\";
#endif

	return args;
}
