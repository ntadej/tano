/****************************************************************************
* Timer.cpp: Timer class
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
	Timer(const QString &fname,
		  const QString &fchannel = 0,
		  const QString &fplaylist = 0,
		  const int &fnum = 0,
		  const QString &furl = 0);
	~Timer();

public:
	QString name() const {return _name;}
	void setName(const QString &sname);
	QString channel() const {return _channel;}
	void setChannel(const QString &schannel);
	QString playlist() const {return _playlist;}
	void setPlaylist(const QString &splaylist);
	QString url() const {return _url;}
	void setUrl(const QString &surl);
	int num() const {return _num;}
	void setNum(const int &snum);
	QDate date() const {return _date;}
	void setDate(const QDate &sdate);
	QTime startTime() const {return _startTime;}
	void setStartTime(const QTime &sstartTime);
	QTime endTime() const {return _endTime;}
	void setEndTime(const QTime &sendTime);
	bool isDisabled() const {return _disabled;}
	void setDisabled(const bool &disabled);
	bool isActive() const {return _active;}
	void setActive(const bool &active);

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
