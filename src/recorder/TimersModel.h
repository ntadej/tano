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

#ifndef TANO_TIMERSMODEL_H_
#define TANO_TIMERSMODEL_H_

#include "core/Enums.h"
#include "core/ListModel.h"

class Timer;

class TimersModel : public ListModel
{
Q_OBJECT
public:
    TimersModel(QObject *parent = 0);
    ~TimersModel();

    Timer *find(const QString &id) const;
    Timer *row(const int &row);
    Timer *takeRow(const int &row);

    Timer *createTimer(const QString &name,
                       const QString &channel,
                       const QString &url,
                       const Tano::TimerType &type = Tano::Once);
    void deleteTimer(Timer *timer);

    void readTimers();
    void writeTimers();
};

#endif // TANO_TIMERSMODEL_H_
