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

#ifndef TANO_RECORDERINFOWIDGET_H_
#define TANO_RECORDERINFOWIDGET_H_

#include <QtGui/QStackedWidget>

class Timer;

namespace Ui {
    class RecorderInfoWidget;
}

class RecorderInfoWidget : public QStackedWidget
{
Q_OBJECT
public:
    explicit RecorderInfoWidget(QWidget *parent = 0);
    ~RecorderInfoWidget();

    void setAction(QAction *action);
    void start(const QString &name,
               const QString &channel,
               const QString &output,
               const QString &end = 0);
    void stop();

protected:
    void changeEvent(QEvent *e);

public slots:
     void time(const int &time);

private slots:
     void backToMain();
     void recordingInfo(Timer *timer);
     void timerInfo(Timer *timer);

private:
    Ui::RecorderInfoWidget *ui;

    QAction *_actionRecord;
};

#endif // TANO_RECORDERINFOWIDGET_H_
