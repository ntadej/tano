/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_RECORDERTIMERSWIDGET_H_
#define TANO_RECORDERTIMERSWIDGET_H_

#include <QtCore/QModelIndex>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>

#include "timers/models/TimersModel.h"

class FancyLineEdit;
class Timer;
class TimersFilterModel;
class TimersModel;

class RecorderTimersWidget : public QMainWindow
{
Q_OBJECT
public:
    explicit RecorderTimersWidget(QWidget *parent = 0);
    ~RecorderTimersWidget();

    Timer *currentTimer() { return _current; }
    void setCurrentTimer(Timer *timer);

    void setModel(TimersModel *model);

protected:
    void changeEvent(QEvent *e);

signals:
    void requestNew();
    void requestQuick();
    void recordingSelected(Timer *);
    void timerSelected(Timer *);

private slots:
    void processFilters();
    void timerClicked(const QModelIndex &index);

private:
    Timer *_current;
    TimersModel *_model;
    TimersFilterModel *_filterModel;

    QListView *_list;

    QToolBar *_toolbarTop;
    QToolBar *_toolbarBottomSearch;

    QLabel *_labelRecorder;
    QAction *_actionQuick;
    QAction *_actionNew;

    FancyLineEdit *_search;
};

#endif // TANO_RECORDERTIMERSWIDGET_H_
