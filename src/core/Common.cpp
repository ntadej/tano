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

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLocale>

#ifdef EDITOR
	#if WITH_EDITOR_VLCQT
		#include <vlc-qt/Common.h>
		#include <vlc-qt/Instance.h>
	#endif
#else
	#include <vlc-qt/Common.h>
	#include <vlc-qt/Instance.h>
#endif

#include "Config.h"
#include "core/Common.h"
#include "core/Settings.h"

QString Tano::version()
{
	QString version;

#ifdef VERSION
	version.append(QString(VERSION));
#else
	version.append("Unknown");
#endif

	return version;
}

QString Tano::changeset()
{
	QString version;

#ifdef VERSION_PATCH
	if(QString(VERSION_PATCH) != "0" && QString(VERSION_PATCH) != "") {
		version.append("(" + QString(VERSION_PATCH) + ")");
	}
#endif

	return version;
}

QString Tano::buildHostname()
{
	QString hostname;

#ifdef HOSTNAME
	hostname.append(QString(HOSTNAME));
#else
	hostname.append("Unknown");
#endif

	return hostname;
}

QString Tano::buildSystem()
{
	QString system;

#ifdef SYSTEM
	system.append(QString(SYSTEM));
#else
	system.append("Unknown");
#endif

	return system;
}

QString Tano::locateResource(const QString &file)
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

	// Try application exe directory without editor, recorder or src for development
	else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/editor", "") + "/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath().replace("/editor", "") + "/" + file).absoluteFilePath();

	else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/recorder", "") + "/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath().replace("/recorder", "") + "/" + file).absoluteFilePath();

	else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "") + "/" + file).exists())
		path = QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "") + "/" + file).absoluteFilePath();

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

QString Tano::recorder()
{
	QString path;

	// Try application executable directory
	if (QFileInfo(QCoreApplication::applicationDirPath() + "/" + "tano-recorder").exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/" + "tano-recorder").absoluteFilePath();

	// Try development directory
	else if (QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "/recorder") + "/" + "tano-recorder").exists())
		path = QFileInfo(QCoreApplication::applicationDirPath().replace("/src", "/recorder") + "/" + "tano-recorder").absoluteFilePath();

#ifdef Q_WS_WIN
	// Try Windows directory
	if (QFileInfo(QCoreApplication::applicationDirPath() + "/" + "tano-recorder.exe").exists())
		path = QFileInfo(QCoreApplication::applicationDirPath() + "/" + "tano-recorder.exe").absoluteFilePath();
#endif

	return path;
}

QStringList Tano::vlcQtArgs()
{
	QStringList args;

	Settings *s = new Settings();
#ifdef EDITOR
#if WITH_EDITOR_VLCQT
	args = VlcCommon::args(s->globalSettings());
#endif
#else
	args = VlcCommon::args(s->globalSettings());
#endif
	delete s;

#ifdef Q_WS_WIN
	args << "--plugin-path=vlc\\plugins\\";
#endif

	return args;
}

QStringList Tano::vlcQtRecorderArgs(const QString &file)
{
	QStringList args;

#ifdef EDITOR
#if WITH_EDITOR_VLCQT
	args = VlcCommon::recorderArgs(file);
#endif
#else
	args = VlcCommon::recorderArgs(file);
#endif

#ifdef Q_WS_WIN
	args << "--plugin-path=vlc\\plugins\\";
#endif

	return args;
}

QString Tano::vlcQtVersionCore()
{
	QString version = "/";
#ifdef EDITOR
#if WITH_EDITOR_VLCQT
	version = VlcInstance::version();
#endif
#else
	version = VlcInstance::version();
#endif

	return version;
}

QString Tano::vlcQtVersionLibrary()
{
	QString version = "/";
#ifdef EDITOR
#if WITH_EDITOR_VLCQT
	version = VlcInstance::libVersion();
#endif
#else
	version = VlcInstance::libVersion();
#endif

	return version;
}

bool Tano::vlcQtDisabled()
{
	bool disabled = false;
#ifdef EDITOR
#if WITH_EDITOR_VLCQT
#else
	disabled = true;
#endif
#endif

	return disabled;
}
