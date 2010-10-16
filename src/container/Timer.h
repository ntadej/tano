/****************************************************************************
* Timer.cpp: Timer container class
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

#ifndef TANO_TIMER_H_
#define TANO_TIMER_H_

#include <QtCore/QDate>
#include <QtCore/QString>
#include <QtCore/QTime>

class Timer
{
public:
	Timer(const QString &name,
		  const QString &channel = 0,
		  const QString &playlist = 0,
		  const int &num = 0,
		  const QString &url = 0);
	~Timer();

public:
	QString name() const {return _name;}
	void setName(const QString &name) { _name = name; }
	QString channel() const {return _channel;}
	void setChannel(const QString &channel) { _channel = channel; }
	QString playlist() const {return _playlist;}
	void setPlaylist(const QString &playlist) { _playlist = playlist; }
	QString url() const {return _url;}
	void setUrl(const QString &url) { _url = url; }
	int num() const {return _num;}
	void setNum(const int &num) { _num = num; }
	QDate date() const {return _date;}
	void setDate(const QDate &date) { _date = date; }
	QTime startTime() const {return _startTime;}
	void setStartTime(const QTime &startTime) { _startTime = startTime; }
	QTime endTime() const {return _endTime;}
	void setEndTime(const QTime &endTime) { _endTime = endTime; }
	bool isDisabled() const {return _disabled;}
	void setDisabled(const bool &disabled) { _disabled = disabled; }
	bool isActive() const {return _active;}
	void setActive(const bool &active) { _active = active; }

private:
	bool _disabled;
	bool _active;
	QString _name;
	QString _channel;
	QString _playlist;
	QString _url;
	int _num;
	QDate _date;
	QTime _startTime;
	QTime _endTime;
};

#endif // TANO_TIMER_H_
