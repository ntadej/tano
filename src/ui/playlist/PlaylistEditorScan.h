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

#ifndef TANO_PLAYLISTEDITORSCAN_H_
#define TANO_PLAYLISTEDITORSCAN_H_

#if defined(Qt5)
    #include <QtWidgets/QWidget>
#elif defined(Qt4)
    #include <QtGui/QWidget>
#endif

#include <vlc-qt/Enums.h>

class PlaylistModel;
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class Udpxy;

namespace Ui {
    class PlaylistEditorScan;
}

class PlaylistEditorScan : public QWidget
{
Q_OBJECT
public:
    explicit PlaylistEditorScan(QWidget *parent = 0);
    ~PlaylistEditorScan();

    void setMediaInstance(VlcInstance *instance);
    void setModel(PlaylistModel *model);

protected:
    void changeEvent(QEvent *e);

signals:
    void addItem(const QString &,
                 const QString &);
    void scan(const bool &);

private slots:
    void refreshPlaylist(const bool &refresh);
    void checkIp();
    void checkCurrentIp();
    
private:
    Ui::PlaylistEditorScan *ui;

    PlaylistModel *_model;

	// Update playlist
	QString currentIp();

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    Udpxy  *_udpxy;
    int _refresh;
    int _currentIp[4];
    int _currentPort;
    int _currentTimeout;
    QTimer *_timer;
};

#endif // PLAYLISTEDITORSCAN_H
