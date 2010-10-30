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

#ifndef TANO_EPGSLOVENIA_H_
#define TANO_EPGSLOVENIA_H_

#include <QtCore/QProcess>
#include <QtCore/QString>
#include <QtNetwork/QHttpRequestHeader>

#include "container/EpgShowInfo.h"

class EpgSlovenia
{
public:
	EpgSlovenia();
	~EpgSlovenia();

	QString host() const;
	QHttpRequestHeader httpHeader(const QString &url) const;
	bool init(const QString &text);
	QString load(const QString &input,
				 const int &arg = 0) const;
	QString processDate(const QString &input) const;
	QStringList processSchedule(const QString &input) const;
	EpgShowInfo processShow(const QString &input) const;

private:
	QString _flag;
};

#endif // TANO_EPGSLOVENIA_H_
