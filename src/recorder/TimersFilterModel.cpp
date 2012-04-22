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

#include "container/core/Timer.h"

#include "TimersFilterModel.h"

TimersFilterModel::TimersFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    _finished = false;
    _state = -1;
    _timeFilter = false;
    _startTime = QDateTime();
    _endTime = QDateTime();
}

TimersFilterModel::~TimersFilterModel() { }

bool TimersFilterModel::filterAcceptsRow(int sourceRow,
                                         const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    bool name = sourceModel()->data(index, Timer::DisplayRole).toString().contains(filterRegExp());
    bool finished = (sourceModel()->data(index, Timer::StateRole).toInt() == Tano::Finished) == _finished;
    bool state = sourceModel()->data(index, Timer::StateRole).toInt() == _state || _state == -1;

    bool time = true;
    if (_timeFilter) {
        bool s = ((sourceModel()->data(index, Timer::StartDateTimeRole).toDateTime() <= startTime()) && (sourceModel()->data(index, Timer::EndDateTimeRole).toDateTime() >= startTime()));
        bool e = ((sourceModel()->data(index, Timer::StartDateTimeRole).toDateTime() <= endTime()) && (sourceModel()->data(index, Timer::EndDateTimeRole).toDateTime() >= endTime()));
        time = s || e;
    }

    return name && finished && state && time;
}

void TimersFilterModel::setFinished(const bool &finished)
{
    _finished = finished;
    invalidateFilter();
}

void TimersFilterModel::setTimerState(const int &state)
{
    _state = state;
    invalidateFilter();
}

void TimersFilterModel::setTimeFilter(const bool &timeFilter)
{
    _timeFilter = timeFilter;
    invalidateFilter();
}

void TimersFilterModel::setStartTime(const QDateTime &startTime)
{
    _startTime = startTime;
    invalidateFilter();
}

void TimersFilterModel::setEndTime(const QDateTime &endTime)
{
    _endTime = endTime;
    invalidateFilter();
}
