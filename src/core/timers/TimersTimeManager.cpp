/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include "timers/TimersSql.h"
#include "timers/TimersTimeManager.h"
#include "timers/containers/Timer.h"

TimersTimeManager::TimersTimeManager(TimersSql *db,
                                     QObject *parent)
    : QObject(parent),
      _db(db)
{
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(check()));
    _timer->start(500);
}

TimersTimeManager::~TimersTimeManager()
{
    delete _timer;
}

void TimersTimeManager::check()
{
    Timer *t = _db->timerToRecord();

    if(!t)
        return;

    if (t->endDateTime() < QDateTime::currentDateTime()) {
        t->setState(Timer::Expired);
        _db->updateTimer(t);
        delete t;
        return;
    } else if (t->date() > QDate::currentDate()) {
        delete t;
        return;
    }

    if (t->startDateTime() > QDateTime::currentDateTime()) {
        delete t;
        return;
    } else if (t->startDateTime() <= QDateTime::currentDateTime()) {
        qDebug() << "Timer ready:" << t->name();
        emit timer(t);
    }
}
