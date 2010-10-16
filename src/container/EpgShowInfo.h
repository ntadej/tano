/****************************************************************************
* EpgShowInfo.cpp: Epg show information container class
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#ifndef TANO_EPGSHOWINFO_H_
#define TANO_EPGSHOWINFO_H_

#include <QtCore/QDateTime>
#include <QtCore/QString>

class EpgShowInfo
{
public:
	EpgShowInfo(const QString &title);
	~EpgShowInfo();

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
