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

#include <QVlc/Config.h>

#include "Common.h"
#include "core/Settings.h"
#include "ui/About.h"

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

#ifdef Q_WS_X11
		else if (QFileInfo("/usr/bin/" + file).exists())
			path = QFileInfo("/usr/bin/" + file).absoluteFilePath();
#endif

#ifdef DEFAULT_DATA_DIR
	else if (QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + file).exists())
		path = QFileInfo(QString(DEFAULT_DATA_DIR) + "/" + file).absoluteFilePath();
#endif

	return path;
}

QString Common::locateLang(const QString &file)
{
	QString path;

	if (QFileInfo(file).exists())
		path = QFileInfo(file).absoluteFilePath();

	// Try application exe working path
	else if (QFileInfo(QDir::currentPath() + "/lang/" + file).exists())
		path = QFileInfo(QDir::currentPath() + "/lang/" + file).absoluteFilePath();

	// Try application exe directory
	else if (QFileInfo(QCoreApplication::applicationDirPath() + "/lang/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/lang/" + file).absoluteFilePath();

	// Try application exe directory without src for development
	else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/src","") + "/lang/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath().replace("/src","") + "/lang/" + file).absoluteFilePath();

#ifdef DEFAULT_DATA_DIR
	else if (QFileInfo(QString(DEFAULT_DATA_DIR) + "/lang/" + file).exists())
		path = QFileInfo(QString(DEFAULT_DATA_DIR) + "/lang/" + file).absoluteFilePath();
#endif

	return path.replace(QString("/" + file), QString(""));
}

QString Common::language(const QString &locale)
{
	QString language = QLocale::languageToString(QLocale(locale).language());

	if(language == "C")
		return "English";
	else
		return language;
}

QStringList Common::loadLocale()
{
	QDir dir(Common::locateLang("tano_sl_SI.qm"));
	QStringList list;
	QLocale locale = QLocale::English;
	list << QLocale::languageToString(locale.language());

	foreach (QString fileName, dir.entryList(QDir::Files)) {
		if(fileName.contains(".qm")) {
			locale = QLocale(fileName.replace("tano_",""));
			list << locale.name();
		}
	}

	return list;
}

void Common::about(QWidget *parent)
{
	About about(parent);
	about.exec();
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

#ifdef Q_WS_X11	
#if VLC_1_1
#else
	args << "--vout-event"
		 << "3";
#endif
#endif

	return args;
}
