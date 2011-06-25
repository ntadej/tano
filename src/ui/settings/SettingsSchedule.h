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

#ifndef TANO_SETTINGSSCHEDULE_H_
#define TANO_SETTINGSSCHEDULE_H_

#include <QtGui/QWidget>

namespace Ui
{
    class SettingsSchedule;
}

class SettingsSchedule : public QWidget
{
Q_OBJECT
public:
    SettingsSchedule(QWidget *parent = 0);
    ~SettingsSchedule();

    bool xmltv() const;
    void setXmltv(const bool &enabled);
    QString grabber() const;
    void setGrabber(const QString &grabber);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsSchedule *ui;
};

#endif // TANO_SETTINGSSCHEDULE_H_
