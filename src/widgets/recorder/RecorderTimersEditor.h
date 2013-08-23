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

#ifndef TANO_RECORDERTIMERSEDITOR_H_
#define TANO_RECORDERTIMERSEDITOR_H_

#include <QtCore/QDate>

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

class Timer;
class TimersFilterModel;
class TimersModel;

namespace Ui
{
    class RecorderTimersEditor;
}

class RecorderTimersEditor : public QWidget
{
Q_OBJECT
public:
    explicit RecorderTimersEditor(QWidget *parent = 0);
    ~RecorderTimersEditor();

    void edit(Timer *item);
    bool save();
    void setModel(TimersModel *model);

protected:
    void changeEvent(QEvent *e);

private:
    bool validate();

    Ui::RecorderTimersEditor *ui;

    Timer *_currentTimer;
    TimersFilterModel *_validateModel;
    TimersModel *_modelCore;
};

#endif // TANO_RECORDERTIMERSEDITOR_H_
