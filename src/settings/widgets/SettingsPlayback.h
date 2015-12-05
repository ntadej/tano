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

#ifndef TANO_SETTINGSPLAYBACK_H_
#define TANO_SETTINGSPLAYBACK_H_

#include <QtWidgets/QWidget>

namespace Ui
{
    class SettingsPlayback;
}

class SettingsPlayback : public QWidget
{
    Q_OBJECT
public:
    SettingsPlayback(QWidget *parent = 0);
    ~SettingsPlayback();

protected:
    void changeEvent(QEvent *e);

signals:
    void playbackRememberChanged();
    void playbackDefaultsChanged();
    void playbackMiscChanged();

private slots:
    void rememberChanged();
    void defaultsChanged();
    void miscChanged();

private:
    void setAudio(const QString &audio);
    void setSub(const QString &sub);

    Ui::SettingsPlayback *ui;
};

#endif // TANO_SETTINGSPLAYBACK_H_
