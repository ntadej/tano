/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_ENUMS_H_
#define TANO_ENUMS_H_

#include <QtCore/QString>

namespace Tano {
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

	// Timer types
	enum TimerType {
		Once,
		Daily,
		Weekly
	};

	TimerType timerType(const int &type);
	int timerType(const TimerType &type);
	QString timerTypeString(const TimerType &type);
};

#endif // TANO_ENUMS_H_
