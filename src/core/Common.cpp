/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLocale>

#include <vlc-qt/Common.h>

#include "Config.h"
#include "core/Common.h"
#include "core/Settings.h"

#include "ui/dialogs/AboutDialog.h"

void Tano::about(QWidget *parent)
{
	AboutDialog about(parent);
	about.exec();
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

	return path;
}

QList<const char *> Tano::vlcQtArgs()
{
	QList<const char *> args;

	Settings *s = new Settings();
	args = VlcCommon::libvlcArgs(s->globalSettings());
	delete s;

#ifdef Q_WS_WIN
	args << "--plugin-path=vlc\\plugins\\";
#endif

	return args;
}

Tano::EpgType Tano::epgType(const QString &type)
{
	if(type == "slovenia")
		return Tano::Slovenia;
	else if(type == "xmltv")
		return Tano::XMLTV;
	else
		return Tano::Slovenia;
}

QString Tano::epgType(const EpgType &type)
{
	if(type == Tano::Slovenia)
		return QString("slovenia");
	else if(type == Tano::XMLTV)
		return QString("xmltv");
}
