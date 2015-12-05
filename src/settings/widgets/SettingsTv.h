/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_SETTINGSTV_H_
#define TANO_SETTINGSTV_H_

#include <QtCore/QTimer>
#include <QtWidgets/QWidget>

namespace Ui
{
    class SettingsTv;
}

class SettingsTv : public QWidget
{
    Q_OBJECT
public:
    SettingsTv(QWidget *parent = 0);
    ~SettingsTv();

protected:
    void changeEvent(QEvent *e);

signals:
    void channelsChanged();
    void udpxyChanged();
    void xmltvChanged();

private slots:
    void channelsSettingsChanged();
    void udpxySettingsChanged();
    void xmltvSettingsChanged();

private:
    Ui::SettingsTv *ui;

    QTimer *_channelsTimer;
};

#endif // TANO_SETTINGSTV_H_
