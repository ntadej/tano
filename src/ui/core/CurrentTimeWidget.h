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

#ifndef TANO_CURRENTTIMEWIDGET_H_
#define TANO_CURRENTTIMEWIDGET_H_

#include <QtCore/QTimer>

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

class QLabel;

class CurrentTimeWidget : public QWidget
{
Q_OBJECT
public:
    CurrentTimeWidget(QWidget *parent = 0);
    ~CurrentTimeWidget();

private slots:
    void updateTime();

private:
    QLabel *_time;
    QTimer *_timer;
};

#endif // TANO_CURRENTTIMEWIDGET_H_
