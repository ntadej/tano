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

#ifndef TANO_SETTINGSPLAYLIST_H_
#define TANO_SETTINGSPLAYLIST_H_

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

namespace Ui
{
    class SettingsPlaylist;
}

class SettingsPlaylist : public QWidget
{
Q_OBJECT
public:
    SettingsPlaylist(QWidget *parent = 0);
    ~SettingsPlaylist();

    QString playlist() const;
    void setPlaylist(const QString &playlist);
    bool playlistUpdate() const;
    void setPlaylistUpdate(const bool &enabled);
    QString playlistUpdateUrl() const;
    void setPlaylistUpdateUrl(const QString &playlist);

    QString radioCategory() const;
    void setRadioCategory(const QString &category);
    QString hdCategory() const;
    void setHdCategory(const QString &category);

    bool udpxy() const;
    void setUdpxy(const bool &enabled);
    QString udpxyUrl() const;
    void setUdpxyUrl(const QString &url);
    int udpxyPort() const;
    void setUdpxyPort(const int &port);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsPlaylist *ui;
};

#endif // TANO_SETTINGSPLAYLIST_H_
