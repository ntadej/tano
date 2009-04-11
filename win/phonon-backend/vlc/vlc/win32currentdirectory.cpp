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

#include "win32currentdirectory.h"

#include <QtCore/QDebug>
#include <QtCore/QtGlobal>

#ifdef Q_OS_WIN
	#include <windows.h>
#endif	//Q_OS_WIN

/**
 * Only under Windows.
 *
 * Use Win32 API function: SetCurrentDirectory()
 *
 * @see saveCurrentDirectory()
 * @see changeBackToCurrentDirectory()
 */
void setCurrentDirectory(const QString & dir) {
#ifdef Q_OS_WIN
	//Change current directory in order to load all the *.dll (libvlc.dll + all the plugins)
	if (!SetCurrentDirectoryA(dir.toUtf8().constData())) {
		qFatal("SetCurrentDirectory() failed (%d)\n", GetLastError());
	}
#endif	//Q_OS_WIN
}

QString getCurrentDirectory() {
#ifdef Q_OS_WIN
	char currentDirectory[MAX_PATH];
	DWORD dwRet = GetCurrentDirectoryA(MAX_PATH, currentDirectory);
	if (dwRet == 0 || dwRet > MAX_PATH) {
		qFatal("GetCurrentDirectory() failed (%d)", GetLastError());
	}
	return currentDirectory;
#endif	//Q_OS_WIN
}
