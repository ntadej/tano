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

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "container/core/Channel.h"
#include "container/core/Timer.h"
#include "core/Enums.h"
#include "core/Settings.h"
#include "core/Udpxy.h"
#include "playlist/PlaylistModel.h"
#include "recorder/RecorderCore.h"
#include "recorder/RecorderTimeManager.h"
#include "recorder/TimersModel.h"
#include "ui/core/TrayIcon.h"
#include "ui/recorder/RecorderInfoWidget.h"

#include "Recorder.h"
#include "ui_Recorder.h"

Recorder::Recorder(QWidget *parent)
    : QStackedWidget(parent),
      ui(new Ui::Recorder),
      _actionRecord(0),
      _currentTimer(0),
      _info(0),
      _trayIcon(0)
{
    ui->setupUi(this);

    _core = new RecorderCore(this);
    _manager = new RecorderTimeManager(this);
    _model = new TimersModel(this);
    _udpxy = new Udpxy();

    _manager->setTimersModel(_model);

    ui->listRecordings->setModel(_model, true);
    ui->listTimers->setModel(_model);

    // Connections
    connect(ui->buttonQuickCancel, SIGNAL(clicked()), this, SLOT(quickRecordCancel()));
    connect(ui->buttonQuickRecord, SIGNAL(clicked()), this, SLOT(quickRecordStart()));

	connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playlist(Channel *)));

    connect(_core, SIGNAL(timerStop()), this, SLOT(recordStop()));

    connect(_manager, SIGNAL(timer(Timer *)), this, SLOT(recordStart(Timer *)));
}

Recorder::~Recorder()
{
    delete ui;

    delete _core;
    delete _model;
}

void Recorder::changeEvent(QEvent *e)
{
    QStackedWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void Recorder::createSettings()
{
    Settings *settings = new Settings(this);
    _directory = settings->recorderDirectory();
    delete settings;

    _core->setDefaultOutputPath(_directory);
    _udpxy->createSettings();
}

bool Recorder::isRecording() const
{
    return _core->isRecording();
}

void Recorder::playlist(Channel *channel)
{
    _currentChannel = channel;

    ui->valueSelectedQuick->setText("<b>" + channel->name() + "</b>");
}

void Recorder::quickRecord()
{
    setCurrentIndex(1);
}

void Recorder::quickRecordCancel()
{
    setCurrentIndex(0);
}

void Recorder::quickRecordStart()
{
    Timer *timer = _model->createTimer(ui->editNameQuick->text(), _currentChannel->name(), _udpxy->processUrl(_currentChannel->url()), Tano::Instant);

    recordStart(timer);

    setCurrentIndex(0);
}

void Recorder::recordStart(Timer *timer)
{
    if (!QDir(_directory).exists()) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Cannot write to %1.")
                    .arg(_directory));
        return;
    } else if (timer->url().isEmpty() || timer->channel().isEmpty()) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Recording not valid!"));
        delete timer;
        return;
    }

    _currentTimer = timer;
    if (timer->type() == Tano::Instant) {
        timer->setDate(QDate::currentDate());
        timer->setStartTime(QTime::currentTime());
    }

    _core->record(timer);

    if (_core->isTimer())
        _info->start(timer->name(), timer->channel(), timer->file(), timer->endTime().toString("hh:mm"));
    else
        _info->start(timer->name(), timer->channel(), timer->file());

    if (_actionRecord)
        _actionRecord->setEnabled(true);

    if (_trayIcon) {
        QString title = QString("%1 (%2)").arg(timer->name(), timer->channel());
        _trayIcon->changeToolTip(Tano::Record, title);
        if (_core->isTimer())
            _trayIcon->message(Tano::Record, QStringList() << title << timer->file() << timer->endTime().toString("hh:mm"));
        else
            _trayIcon->message(Tano::Record, QStringList() << title << timer->file());
    }
}


void Recorder::recordStop()
{
    if(!_core->isRecording())
        return;

    _core->stop();

    _info->stop();
    if (_actionRecord)
        _actionRecord->setEnabled(false);

    if (_trayIcon) {
        _trayIcon->changeToolTip(Tano::Record);
        _trayIcon->message(Tano::Record, QStringList());
    }

    if (_currentTimer->type() == Tano::Instant)
        _currentTimer->setEndTime(QTime::currentTime());
    _currentTimer->setState(Tano::Finished);
    _model->writeTimers();
    _currentTimer = 0;
}

void Recorder::refreshPlaylistModel()
{
    ui->playlistWidget->refreshModel();
}

void Recorder::setMediaInstance(VlcInstance *instance)
{
    _core->setMediaInstance(instance);
}

void Recorder::setPlaylistModel(PlaylistModel *model)
{
    _playlistModel = model;
    ui->playlistWidget->setModel(_playlistModel);
}

void Recorder::setWidgets(QAction *action,
                          RecorderInfoWidget *info,
                          TrayIcon *icon)
{
    _actionRecord = action;
    connect(_actionRecord, SIGNAL(triggered()), this, SLOT(recordStop()));

    _info = info;
    _info->setAction(_actionRecord);
    connect(_core, SIGNAL(elapsed(int)), _info, SLOT(time(int)));
    connect(ui->listRecordings, SIGNAL(itemSelected(Timer *)), _info, SLOT(recordingInfo(Timer *)));
    connect(ui->listTimers, SIGNAL(itemSelected(Timer *)), _info, SLOT(timerInfo(Timer *)));

    _trayIcon = icon;
}
