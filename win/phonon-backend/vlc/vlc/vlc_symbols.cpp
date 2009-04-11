/*
 * VLC_Backend backend for the Phonon library
 * Copyright (C) 2007-2008  Tanguy Krotoff <tkrotoff@gmail.com>
 * 					2009	Tadej Novak <tadej@pfusion.co.cc>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vlc_symbols.h"

#include "win32currentdirectory.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QtDebug>
#include <QtCore/QLibrary>
#include <QtCore/QStringList>
#include <QtCore/QDir>
#include <QtCore/QSettings>

static QLibrary * _libvlc = NULL;

QString getVLCPath() {
	static const char * libvlc_name = "libvlc";
	static const char * libvlc_functionToTest = "libvlc_exception_init";

	static QString libvlc_path;

	if (!libvlc_path.isEmpty()) {
		return libvlc_path;
	}

	//Tries to autodetect the VLC_Backend path with a default list of path

	QStringList pathList;
	pathList << QCoreApplication::libraryPaths();

#ifdef Q_OS_LINUX
	pathList << "/usr/local/lib";
#endif	//Q_OS_LINUX

#ifdef Q_OS_WIN
	//Default path under Windows
	//Try it since sometimes there is no registry key
	static const char * common_libvlc_path = "C:\\Program Files\\VideoLAN\\VLC";
	pathList << common_libvlc_path;
	///

	static const char * libvlc_version = "0.9";

	//QSettings allows us to read the Windows registry
	//Check if there is a standard VLC_Backend installation under Windows
	//If there is a VLC_Backend Windows installation, check we get the good version i.e 0.9
	QSettings settings(QSettings::SystemScope, "VideoLAN", "VLC");
	if (settings.value("Version").toString().contains(libvlc_version)) {
		QString vlcInstallDir = settings.value("InstallDir").toString();
		pathList << vlcInstallDir;
	}
#endif	//Q_OS_WIN

	_libvlc = new QLibrary();
	foreach (libvlc_path, pathList) {
		_libvlc->setFileName(libvlc_path + QDir::separator() + libvlc_name);

		//libvlc depends on libvlccore, thus we need the current process
		//directory to be libvlc_path otherwise libvlccore won't be find
		//and thus libvlc won't be loaded
		setCurrentDirectory(libvlc_path);

		if (_libvlc->load() && _libvlc->resolve(libvlc_functionToTest)) {
			qDebug() << "VLC path found:" << libvlc_path;
			return libvlc_path;
		}
		qDebug() << "Warning:" << _libvlc->errorString();
	}

	unloadLibVLC();
	qFatal("Cannot find '%s' on your system", libvlc_name);
	return libvlc_path;
}

QString getVLCPluginPath() {
	QString vlcPath = getVLCPath();

#ifdef Q_OS_WIN
	QString vlcPluginPath(vlcPath + "\\plugins");
#endif	//Q_OS_WIN

#ifdef Q_OS_LINUX
	QString vlcPluginPath(vlcPath + "/vlc");
#endif	//Q_OS_LINUX

	qDebug() << "VLC plugin path:" << vlcPluginPath;

	return vlcPluginPath;
}

void * resolve(const char * name) {
	if (!_libvlc) {
		qFatal("_libvlc cannot be NULL");
	}

	if (!_libvlc->isLoaded()) {
		qFatal("Library '%s' not loaded", _libvlc->fileName().toAscii().constData());
		return NULL;
	}

	void * func = _libvlc->resolve(name);
	if (!func) {
		qFatal("Cannot resolve '%s' in library '%s'", name, _libvlc->fileName().toAscii().constData());
	}

	return func;
}

void unloadLibVLC() {
	_libvlc->unload();
	delete _libvlc;
	_libvlc = NULL;
}
