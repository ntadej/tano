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

#ifndef TANO_SETTINGSRECORDER_H_
#define TANO_SETTINGSRECORDER_H_

#include <QtWidgets/QWidget>

namespace Ui
{
    class SettingsRecorder;
}

class SettingsRecorder : public QWidget
{
Q_OBJECT
public:
    SettingsRecorder(QWidget *parent = 0);
    ~SettingsRecorder();

    QString directory() const;
    void setDirectory(const QString &value);
    QString snapshot() const;
    void setSnapshot(const QString &value);


protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsRecorder *ui;
};

#endif // TANO_SETTINGSRECORDER_H_
