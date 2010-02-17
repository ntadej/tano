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

#include <QtCore/QDir>
#include <QtCore/QFileInfo>

#include "Common.h"
#include "Ver.h"
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

	// Try application exe working path + src for development
	else if (QFileInfo(QDir::currentPath() + "/src/lang/" + file).exists())
		path = QFileInfo(QDir::currentPath() + "/src/lang/" + file).absoluteFilePath();

	// Try application exe directory
	else if (QFileInfo(QCoreApplication::applicationDirPath() + "/lang/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/lang/" + file).absoluteFilePath();

	// Try application exe directory + src for development
	else if (QFileInfo(QCoreApplication::applicationDirPath() + "/src/lang/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/src/lang/" + file).absoluteFilePath();

#ifdef DEFAULT_DATA_DIR
	else if (QFileInfo(QString(DEFAULT_DATA_DIR) + "/lang/" + file).exists())
		path = QFileInfo(QString(DEFAULT_DATA_DIR) + "/lang/" + file).absoluteFilePath();
#endif

	return path.replace(QString("/" + file), QString(""));
}

QString Common::version()
{
	if(SvnRevision() != "")
		return TanoVersion()+"-r"+SvnRevision();
	else
		return TanoVersion();
}

void Common::about(QWidget *parent)
{
	About about(parent, version());
	about.exec();
}

QSettings* Common::settings()
{
	return new QSettings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
}

QString Common::settingsPath()
{
	QSettings *s = settings();
	QString path = s->fileName();
	path.replace("Settings.ini","");

	delete s;

	return path;
}

QList<const char *> Common::libvlcArgs()
{
	QSettings *s = settings();
	QByteArray tmp;

	QList<const char *> args;
	if(s->value("ignore-config",true).toBool())
		args << "--ignore-config";

	args << "--intf=dummy"
		 << "--no-media-library"
		 << "--reset-plugins-cache"
		 << "--no-stats"
		 << "--no-osd"
		 << "--no-video-title-show";

#ifdef Q_WS_X11
#if VLC_TRUNK

#else
	args << "--vout-event"
		 << "3";
#endif
#endif

	if(s->value("network","").toString() != "") {
		tmp = s->value("network","").toString().toLatin1();
		args << "--miface-addr"
			 << tmp.constData();
	}

	delete s;

	return args;
}
