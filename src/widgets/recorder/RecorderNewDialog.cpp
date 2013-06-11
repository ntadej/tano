/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include "RecorderNewDialog.h"
#include "ui_RecorderNewDialog.h"

#if QT_VERSION >= 0x050000
    #include <QtWidgets/QMenu>
    #include <QtWidgets/QMessageBox>
    #include <QtWidgets/QWidgetAction>
#else
    #include <QtGui/QMenu>
    #include <QtGui/QMessageBox>
    #include <QtGui/QWidgetAction>
#endif

#include "core/network/NetworkUdpxy.h"
#include "core/playlist/PlaylistModel.h"
#include "core/playlist/containers/Channel.h"
#include "core/timers/TimersSql.h"
#include "core/timers/containers/Timer.h"
#include "core/xmltv/containers/XmltvProgramme.h"

RecorderNewDialog::RecorderNewDialog(TimersSql *db,
                                     QWidget *parent)
    : QDialog(parent),
      ui(new Ui::RecorderNewDialog),
      _currentChannel(0),
      _currentTimer(0),
      _db(db)
{
    ui->setupUi(this);

    ui->quickBox->hide();
    ui->timerBox->hide();

    // TODO: filter
    /*_menu = new QMenu(this);
    _action = new QWidgetAction(this);
    ui->playlistWidget->filter()->show();
    _action->setDefaultWidget(ui->playlistWidget->filter());
    _menu->addAction(_action);
    ui->buttonFilter->setMenu(_menu);*/

    _udpxy = new NetworkUdpxy();
    _udpxy->createSettings();

    connect(ui->buttonNewCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->buttonNewTimer, SIGNAL(clicked()), this, SLOT(processNewTimer()));
    connect(ui->buttonQuickCancel, SIGNAL(clicked()), this, SLOT(reject()));
    connect(ui->buttonQuickRecord, SIGNAL(clicked()), this, SLOT(processQuickRecord()));

    connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playlist(Channel *)));
}

RecorderNewDialog::~RecorderNewDialog()
{
    delete ui;
}

void RecorderNewDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void RecorderNewDialog::newQuick()
{
    ui->quickBox->show();
    ui->timerBox->hide();
}

void RecorderNewDialog::newTimer()
{
    ui->quickBox->hide();
    ui->timerBox->show();
}

void RecorderNewDialog::newTimerFromSchedule(XmltvProgramme *programme)
{
    ui->playlistWidget->channelSelected(programme->channel());

    if (!_currentChannel) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("You don't have this channel in your playlist."));
        return;
    }

    Timer *timer = _db->createTimer(programme->title(), _currentChannel->name(), _udpxy->processUrl(_currentChannel->url()));
    timer->setState(Timer::Disabled);
    timer->setDate(programme->start().date());
    timer->setStartTime(programme->start().time());
    timer->setEndTime(programme->stop().time());
    _db->updateTimer(timer);

    _currentTimer = timer;
}

void RecorderNewDialog::playlist(Channel *channel)
{
    _currentChannel = channel;

    ui->valueSelectedNew->setText("<b>" + channel->name() + "</b>");
    ui->valueSelectedQuick->setText("<b>" + channel->name() + "</b>");
}

void RecorderNewDialog::processNewTimer()
{
    if (!_currentChannel) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Please, select a channel."));
        return;
    }

    _currentTimer = _db->createTimer(ui->editNameNew->text(), _currentChannel->name(), _udpxy->processUrl(_currentChannel->url()));

    accept();
}

void RecorderNewDialog::processQuickRecord()
{
    if (!_currentChannel) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Please, select a channel."));
        return;
    }

    _currentTimer = _db->createTimer(ui->editNameQuick->text(), _currentChannel->name(), _udpxy->processUrl(_currentChannel->url()), Timer::Instant);

    accept();
}

void RecorderNewDialog::refreshPlaylistModel()
{
    //ui->playlistWidget->refreshModel();
}

void RecorderNewDialog::setPlaylistModel(PlaylistModel *model)
{
    ui->playlistWidget->setModel(model);
}
