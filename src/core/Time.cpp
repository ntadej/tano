/****************************************************************************
* Time.cpp: Main time manager
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

#include <QtCore/QDebug>

#include "core/Time.h"

Time::Time()
{
	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(check()));
	_timer->start(6000);
}

Time::~Time()
{
	delete _timer;
}

void Time::check()
{
	for(int i=0; i<_timersList.size(); i++) {
		if(_timersList[i]->startTime() <= QTime::currentTime() && _timersList[i]->endTime() >= QTime::currentTime()) {
			if(!_timersList[i]->isActive()) {
				qDebug() << "Timer starting:" << _timersList[i]->name();
				_timersList[i]->setActive(true);
				emit startTimer(_timersList[i]);
				emit timerStatus(_timersList[i], true);
			}
		} else if(_timersList[i]->isActive()) {
			qDebug() << "Timer ending:" << _timersList[i]->name();
			_timersList[i]->setActive(false);
			emit stopTimer(_timersList[i]);
			emit timerStatus(_timersList[i], false);
		}
	}
}

void Time::addTimer(Timer *t)
{
	if(!_timersList.contains(t))
		_timersList.append(t);
}

void Time::removeTimer(Timer *t)
{
	if(_timersList.contains(t))
		_timersList.removeOne(t);
}
