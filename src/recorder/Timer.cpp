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

#include "Timer.h"

Timer::Timer(const QString &fname,
			 const QString &fchannel,
			 const QString &fplaylist,
			 const int &fnum,
			 const QString &furl)
	: _name(fname), _channel(fchannel), _playlist(fplaylist), _num(fnum), _url(furl)
{
	_date = QDate::currentDate();
	_startTime = QTime::currentTime().addSecs(300);
	_endTime = QTime::currentTime().addSecs(3900);
	_disabled = false;
	_active = false;
}

Timer::~Timer() {

}

void Timer::setName(const QString &sname)
{
	_name = sname;
}

void Timer::setChannel(const QString &schannel)
{
	_channel = schannel;
}

void Timer::setPlaylist(const QString &splaylist)
{
	_playlist = splaylist;
}

void Timer::setUrl(const QString &surl)
{
	_url = surl;
}

void Timer::setNum(const int &snum)
{
	_num = snum;
}

void Timer::setDate(const QDate &sdate)
{
	_date = sdate;
}

void Timer::setStartTime(const QTime &sstartTime)
{
	_startTime = sstartTime;
}

void Timer::setEndTime(const QTime &sendTime)
{
	_endTime = sendTime;
}

void Timer::setDisabled(const bool &disabled)
{
	_disabled = disabled;
}

void Timer::setActive(const bool &active)
{
	_active = active;
}
