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

#ifndef TANO_EPGDAYLIST_H_
#define TANO_EPGDAYLIST_H_

#include <QtCore/QDate>
#include <QtCore/QList>

#include "container/EpgItem.h"

class EpgDayList : public QList<EpgItem>
{
public:
	EpgDayList(const QString &channel = 0,
			   const int &day = 0);
	~EpgDayList();

	QString channel() const { return _channel; }
	QDate date() const { return _date; }
	int day() const { return _day; }
	bool isValid() const { return _valid; }
	void setValid(const bool &valid) { _valid = valid; }

private:
	QString _channel;
	QDate _date;
	int _day;
	bool _valid;
};

#endif // TANO_EPGDAYLIST_H_
