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

#include "timers/TimersTimeManager.h"
#include "timers/containers/Timer.h"
#include "timers/models/TimersFilterModel.h"
#include "timers/models/TimersModel.h"

TimersTimeManager::TimersTimeManager(QObject *parent)
    : QObject(parent)
{
    _model = new TimersFilterModel(this);
    _model->setDynamicSortFilter(true);
    _model->setSortRole(Timer::StartDateTimeRole);
    _model->setTimerState(Timer::Enabled);
    _model->sort(0);

    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(check()));
}

TimersTimeManager::~TimersTimeManager()
{
    delete _model;
    delete _timer;
}

void TimersTimeManager::check()
{
    if(!_model->rowCount())
        return;

    Timer *t = _modelCore->row(_model->mapToSource(_model->index(0, 0)).row());

    if (t->endDateTime() < QDateTime::currentDateTime()) {
        t->setState(Timer::Expired);
        return;
    } else if (t->date() > QDate::currentDate()) {
        return;
    }

    if (t->startDateTime() > QDateTime::currentDateTime()) {
        return;
    } else if (t->startDateTime() <= QDateTime::currentDateTime()) {
        qDebug() << "Timer ready:" << t->name();
        emit timer(t);
    }
}

void TimersTimeManager::setTimersModel(TimersModel *model)
{
    _modelCore = model;
    _model->setSourceModel(model);

    _timer->start(500);
}
