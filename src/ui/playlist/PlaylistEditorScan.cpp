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

#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

#include "container/core/Channel.h"
#include "core/network/NetworkUdpxy.h"
#include "playlist/PlaylistModel.h"

#include "PlaylistEditorScan.h"
#include "ui_PlaylistEditorScan.h"

PlaylistEditorScan::PlaylistEditorScan(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::PlaylistEditorScan)
{
    ui->setupUi(this);

    _media = 0;
    _player = 0;
    _udpxy = new NetworkUdpxy();
    _timer = new QTimer();
    connect(_timer, SIGNAL(timeout()), this, SLOT(checkIp()));
    connect(ui->buttonUpdate, SIGNAL(toggled(bool)), this, SLOT(refreshPlaylist(bool)));
}

PlaylistEditorScan::~PlaylistEditorScan()
{
    delete ui;

    delete _media;
    delete _timer;
}

void PlaylistEditorScan::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void PlaylistEditorScan::refreshPlaylist(const bool &refresh)
{
    emit scan(refresh);

    if (!refresh) {
        _timer->stop();
        _player->stop();
        ui->progressBar->setValue(1);

        QFile::remove(QDir::tempPath() + "/tano-test.ts");
    } else {
        qDebug() << "Scanning:" << ui->ipFrom->text() << ui->ipPort->value() << ui->ipTimeout->value();

        QStringList ipFrom = ui->ipFrom->text().split(".");
        _currentIp[0] = ipFrom[0].toInt();
        _currentIp[1] = ipFrom[1].toInt();
        _currentIp[2] = ipFrom[2].toInt();
        _currentIp[3] = 0;

        _currentPort = ui->ipPort->value();
        _currentTimeout = ui->ipTimeout->value();

        checkIp();
    }
}

void PlaylistEditorScan::checkIp()
{
    if (_currentIp[3] == 255) {
        ui->buttonUpdate->setChecked(false);
        return;
    }

    _currentIp[3]++;
    ui->progressBar->setValue(_currentIp[3]);
    if (_media)
        delete _media;

    _media = new VlcMedia(_udpxy->processUrl(currentIp()), _instance);
    _media->record("test", QDir::tempPath(), Vlc::TS);
    _player->open(_media);

    _timer->start(_currentTimeout);
}

void PlaylistEditorScan::checkCurrentIp()
{
    _timer->stop();

    if (_player->state() != Vlc::Playing) {
        _player->stop();
        checkIp();
    }

    _player->stop();

    bool newChannel = true;
    for (int i = 0; i < _model->rowCount(); i++) {
        if (_model->row(i)->url() == currentIp()) {
            newChannel = false;
            break;
        }
    }

    if (newChannel) {
        qDebug() << "Scanning:" << "Channel Found";
        addItem(tr("New channel from scan %1").arg(currentIp()), currentIp());
    }

    checkIp();
}

QString PlaylistEditorScan::currentIp()
{
    QString ip = "udp://@";
    ip.append(QString().number(_currentIp[0])+".");
    ip.append(QString().number(_currentIp[1])+".");
    ip.append(QString().number(_currentIp[2])+".");
    ip.append(QString().number(_currentIp[3])+":");
    ip.append(QString().number(_currentPort));

    return ip;
}

void PlaylistEditorScan::setMediaInstance(VlcInstance *instance)
{
    _instance = instance;

    if (_player)
        delete _player;
    _player = new VlcMediaPlayer(_instance);
    connect(_player, SIGNAL(vout(int)), this, SLOT(checkCurrentIp()));
}

void PlaylistEditorScan::setModel(PlaylistModel *model)
{
    _model = model;
}
