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

#ifndef TANO_TIMERSFILTERMODEL_H_
#define TANO_TIMERSFILTERMODEL_H_

#include <QtCore/QDateTime>

#if defined(Qt5)
    #include <QtCore/QSortFilterProxyModel>
#elif defined(Qt4)
    #include <QtGui/QSortFilterProxyModel>
#endif

#include "CoreSharedExport.h"

class TANO_CORE_EXPORT TimersFilterModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    TimersFilterModel(QObject *parent = 0);
    ~TimersFilterModel();

    inline int timerState() const { return _state; }
    void setTimerState(const int &state);

    inline bool timeFilter() const { return _timeFilter; }
    void setTimeFilter(const bool &timeFilter);
    inline QDateTime startTime() const { return _startTime; }
    void setStartTime(const QDateTime &startTime);
    inline QDateTime endTime() const { return _endTime; }
    void setEndTime(const QDateTime &endTime);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

private:
    int _state;

    bool _timeFilter;
    QDateTime _startTime;
    QDateTime _endTime;
};

#endif // TANO_TIMERSFILTERMODEL_H_
