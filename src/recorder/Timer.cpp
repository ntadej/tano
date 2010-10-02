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

Timer::Timer(const QString &name,
			 const QString &channel,
			 const QString &playlist,
			 const int &num,
			 const QString &url)
	: _name(name),
	_channel(channel),
	_playlist(playlist),
	_num(num),
	_url(url)
{
	_date = QDate::currentDate();
	_startTime = QTime::currentTime().addSecs(300);
	_endTime = QTime::currentTime().addSecs(3900);
	_disabled = false;
	_active = false;
}

Timer::~Timer() { }
