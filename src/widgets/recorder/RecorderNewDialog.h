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

#ifndef TANO_RECORDERNEWDIALOG_H_
#define TANO_RECORDERNEWDIALOG_H_

#include <QDialog>

class QMenu;
class QWidgetAction;

class Channel;
class ChannelsModel;
class Timer;
class TimersModel;
class XmltvProgramme;

namespace Ui {
    class RecorderNewDialog;
}

class RecorderNewDialog : public QDialog
{
Q_OBJECT
public:
    explicit RecorderNewDialog(bool quick,
                               TimersModel *timers,
                               ChannelsModel *channels,
                               QWidget *parent = 0);
    ~RecorderNewDialog();

    Timer *timer() { return _currentTimer; }

public slots:
    Timer *newTimerFromSchedule(XmltvProgramme *programme);
    
protected:
    void changeEvent(QEvent *e);

private slots:
    void channels(Channel* channel);
    void processFilters();
    void processNewTimer();
    void processQuickRecord();
    void validate();

private:
    Ui::RecorderNewDialog *ui;

    Channel *_currentChannel;
    Timer *_currentTimer;

    TimersModel *_model;
};

#endif // TANO_RECORDERNEWDIALOG_H_
