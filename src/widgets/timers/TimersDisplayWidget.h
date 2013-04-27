/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_TIMERSDISPLAYWIDGET_H_
#define TANO_TIMERSDISPLAYWIDGET_H_

#include <QtCore/QModelIndex>
#include <QtCore/QPointer>

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QWidget>
#else
    #include <QtGui/QWidget>
#endif

#include "timers/models/TimersModel.h"

class Timer;
class TimersFilterModel;
class TimersSql;

namespace Ui
{
    class TimersDisplayWidget;
}

class TimersDisplayWidget : public QWidget
{
Q_OBJECT
public:
    TimersDisplayWidget(QWidget *parent = 0);
    ~TimersDisplayWidget();

    void setDatabase(TimersSql *db);

protected:
    void changeEvent(QEvent *e);

signals:
    void recordingSelected(Timer *);
    void timerSelected(Timer *);

private slots:
    void processFilters();
    void timerClicked(const QModelIndex &index);

private:
    Ui::TimersDisplayWidget *ui;

    TimersSql *_db;
    QPointer<TimersModel> _model;
    TimersFilterModel *_filterModel;
};

#endif // TANO_TIMERSDISPLAYWIDGET_H_
