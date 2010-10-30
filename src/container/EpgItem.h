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

#ifndef TANO_EPGITEM_H_
#define TANO_EPGITEM_H_

#include <QtCore/QString>
#include <QtCore/QTime>

class EpgItem
{
public:
	EpgItem(const QTime &time,
			const QString &url,
			const QString &title);
	~EpgItem();

	QTime time() const { return _time; }
	QString url() const { return _url; }
	QString title() const { return _title; }

private:
	QTime _time;
	QString _url;
	QString _title;
};

#endif // TANO_EPGITEM_H_
