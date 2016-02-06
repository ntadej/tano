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

#ifndef TANO_CHANNELSSCAN_H_
#define TANO_CHANNELSSCAN_H_

#include <QtWidgets/QWidget>

#include <VLCQtCore/Enums.h>

class ChannelsModel;
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class NetworkUdpxy;

namespace Ui {
    class ChannelsScan;
}

class ChannelsScan : public QWidget
{
    Q_OBJECT
public:
    explicit ChannelsScan(QWidget *parent = 0);
    ~ChannelsScan();

    void setMediaInstance(VlcInstance *instance);
    void setChannelsModel(ChannelsModel *model);

protected:
    void changeEvent(QEvent *e);

signals:
    void addItem(const QString &,
                 const QString &);
    void scan(bool );

private slots:
    void refreshChannels(bool refresh);
    void checkIp();
    void checkCurrentIp();
    void setState(const Vlc::State &state);
    
private:
    Ui::ChannelsScan *ui;

    ChannelsModel *_model;

    // Update channels
	QString currentIp();

    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMediaPlayer *_player;
    NetworkUdpxy  *_udpxy;
    int _refresh;
    int _currentIp[4];
    int _currentPort;
    int _currentTimeout;
    bool _currentIpPlaying;
    QTimer *_timer;
};

#endif // TANO_CHANNELSSCAN_H_
