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

#ifndef TANO_COMMON_H_
#define TANO_COMMON_H_

#include <QtCore/QString>
#include <QtGui/QWidget>

namespace Tano {
	// About dialog
	void about(QWidget *parent = 0 );

	// Resources locators
	QString locateResource(const QString &file,
						   const bool &translation = false);

	// Backend settings
	QList<const char *> vlcQtArgs();

	// Epg types
	enum EpgType {
		Slovenia,
		XMLTV
	};

	EpgType epgType(const QString &type);
	QString epgType(const EpgType &type);

	// Epg ID
	enum Id {
		Main,
		Schedule
	};
};

#endif // TANO_COMMON_H_
