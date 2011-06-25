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

#ifndef TANO_SETTINGSBACKEND_H_
#define TANO_SETTINGSBACKEND_H_

#include <QtGui/QWidget>

namespace Ui
{
    class SettingsBackend;
}

class SettingsBackend : public QWidget
{
Q_OBJECT
public:
    SettingsBackend(QWidget *parent = 0);
    ~SettingsBackend();

    bool globalSettings() const;
    void setGlobalSettings(const bool &global);

    bool rememberChannelSettings() const;
    void setRememberChannelSettings(const bool &remember);
    QString audio() const;
    void setAudio(const QString &audio);
    QString sub() const;
    void setSub(const QString &sub);

    bool udpxy() const;
    void setUdpxy(const bool &enabled);
    QString udpxyUrl() const;
    void setUdpxyUrl(const QString &url);
    int udpxyPort() const;
    void setUdpxyPort(const int &port);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsBackend *ui;
};

#endif // TANO_SETTINGSBACKEND_H_
