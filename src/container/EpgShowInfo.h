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

#ifndef TANO_EPGSHOWINFO_H_
#define TANO_EPGSHOWINFO_H_

#include <QtCore/QDateTime>
#include <QtCore/QString>

class EpgShowInfo
{
public:
	EpgShowInfo(const bool valid = true);
	~EpgShowInfo();

	bool isValid() const { return _valid; }
	void setValid(const bool &valid) { _valid = valid; }
	QString title() const { return _title; }
	void setTitle(const QString &title) { _title = title; }
	QString channel() const { return _channel; }
	void setChannel(const QString &channel) { _channel = channel; }
	QDateTime startTime() const { return _startTime; }
	void setStartTime(const QDateTime &startTime) { _startTime = startTime; }
	QDateTime endTime() const { return _endTime; }
	void setEndTime(const QDateTime &endTime) { _endTime = endTime; }
	QString info() const { return _info; }
	void setInfo(const QString &info) { _info = info; }
	QString description() const { return _description; }
	void setDescription(const QString &description) { _description = description; }
	QString image() const { return _image; }
	void setImage(const QString &image) { _image = image; }
	QString previous() const { return _previous; }
	void setPrevious(const QString &previous) { _previous = previous; }
	QString next() const { return _next; }
	void setNext(const QString &next) { _next = next; }

private:
	bool _valid;
	QString _title;
	QString _channel;
	QDateTime _startTime;
	QDateTime _endTime;
	QString _info;
	QString _description;
	QString _image;
	QString _previous;
	QString _next;
};

#endif // TANO_EPGSHOWINFO_H_
