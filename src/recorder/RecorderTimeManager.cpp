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

#include "container/core/Timer.h"
#include "recorder/RecorderTimeManager.h"
#include "recorder/TimersModel.h"

RecorderTimeManager::RecorderTimeManager(QObject *parent)
    : QObject(parent)
{
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(check()));
}

RecorderTimeManager::~RecorderTimeManager()
{
    delete _timer;
}

void RecorderTimeManager::check()
{
    for (int i = 0; i < _model->rowCount(); i++) {
        if (_model->row(i)->startTime() <= QDateTime::currentDateTime() && _model->row(i)->endTime() >= QDateTime::currentDateTime()) {
            if (_model->row(i)->state() == Tano::Enabled) {
                emit timer(_model->row(i));
                qDebug() << "Timer" << _model->row(i)->name() << "started";
            }
        }
    }
}

void RecorderTimeManager::setTimersModel(TimersModel *model)
{
    _model = model;

    _timer->start(3000);
}
