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

#include <QtCore/QDir>
#include <QtCore/QFile>

#if defined(Qt5)
    #include <QtWidgets/QMessageBox>
#elif defined(Qt4)
    #include <QtGui/QMessageBox>
#endif

#include "container/core/Channel.h"
#include "container/core/Timer.h"
#include "container/xmltv/XmltvProgramme.h"
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
      _currentChannel(0),
      _currentTimer(0),
      _info(0),
      _trayIcon(0)
{
    ui->setupUi(this);

    cancel();

    _core = new RecorderCore(this);
    _manager = new RecorderTimeManager(this);
    _model = new TimersModel(this);
    _udpxy = new Udpxy();

    _manager->setTimersModel(_model);

    ui->listRecordings->setModel(_model, true);
    ui->listTimers->setModel(_model);

    // Connections
    connect(ui->buttonNewCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->buttonNewTimer, SIGNAL(clicked()), this, SLOT(processNewTimer()));
    connect(ui->buttonQuickCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(ui->buttonQuickRecord, SIGNAL(clicked()), this, SLOT(processQuickRecord()));

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

void Recorder::cancel()
{
    setCurrentIndex(0);

    ui->quickBox->hide();
    ui->timerBox->hide();
}

void Recorder::createSettings()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _directory = settings->recorderDirectory();

    _core->setDefaultOutputPath(_directory);
    _udpxy->createSettings();
}

bool Recorder::isRecording() const
{
    return _core->isRecording();
}

Timer *Recorder::newInstantTimer(const QString &channel,
                                 const QString &url)
{
    Timer *timer = _model->createTimer(tr("Quick %1").arg(channel), channel, _udpxy->processUrl(url), Tano::Instant);

    return timer;
}

void Recorder::newTimer()
{
    ui->timerBox->show();

    setCurrentIndex(1);
}

void Recorder::newTimerFromSchedule(XmltvProgramme *programme)
{
    ui->playlistWidget->channelSelected(programme->channel());

    if (!_currentChannel) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("You don't have this channel in your playlist."));
        return;
    }

    Timer *timer = _model->createTimer(programme->title(), _currentChannel->name(), _udpxy->processUrl(_currentChannel->url()));
    timer->setState(Tano::Disabled);
    timer->setDate(programme->start().date());
    timer->setStartTime(programme->start().time());
    timer->setEndTime(programme->stop().time());

    _info->timerInfo(timer);

    cancel();
}

void Recorder::playlist(Channel *channel)
{
    _currentChannel = channel;

    ui->valueSelectedNew->setText("<b>" + channel->name() + "</b>");
    ui->valueSelectedQuick->setText("<b>" + channel->name() + "</b>");
}

void Recorder::processNewTimer()
{
    if (!_currentChannel) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Please, select a channel."));
        return;
    }

    Timer *timer = _model->createTimer(ui->editNameNew->text(), _currentChannel->name(), _udpxy->processUrl(_currentChannel->url()));
    timer->setState(Tano::Disabled);

    _info->timerInfo(timer);

    cancel();
}

void Recorder::processQuickRecord()
{
    if (!_currentChannel) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Please, select a channel."));
        return;
    }

    Timer *timer = _model->createTimer(ui->editNameQuick->text(), _currentChannel->name(), _udpxy->processUrl(_currentChannel->url()), Tano::Instant);

    recordStart(timer);

    cancel();
}


void Recorder::quickRecord()
{
    ui->quickBox->show();

    setCurrentIndex(1);
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

    if (_currentTimer->type() == Tano::Instant) {
        _currentTimer->setEndTime(QTime::currentTime());
    } else {
        if (_currentTimer->type() == Tano::Daily) {
            Timer *tmptimer = _model->duplicateTimer(_currentTimer);
            tmptimer->setDate(_currentTimer->date().addDays(1));
        } else if (_currentTimer->type() == Tano::Weekly) {
            Timer *tmptimer = _model->duplicateTimer(_currentTimer);
            tmptimer->setDate(_currentTimer->date().addDays(7));
        }  else if (_currentTimer->type() == Tano::Weekdays) {
            Timer *tmptimer = _model->duplicateTimer(_currentTimer);
            if (_currentTimer->date().dayOfWeek() == Qt::Friday)
                tmptimer->setDate(_currentTimer->date().addDays(3));
            else
                tmptimer->setDate(_currentTimer->date().addDays(1));
        }
    }
    _currentTimer->setState(Tano::Finished);
    writeTimers();
    _currentTimer = 0;
}

void Recorder::recordingDelete(Timer *recording)
{
    if (!QFile::remove(recording->file())) {
        int r;
        r = QMessageBox::critical(this, tr("Recorder"),
                                  tr("File cannot be removed.\nHave you already moved or deleted it?"),
                                  QMessageBox::Yes | QMessageBox::No,
                                  QMessageBox::No);

        switch (r)
        {
        case QMessageBox::No:
            return;
            break;
        case QMessageBox::Yes:
        default:
            break;
        }
    }

    _model->deleteTimer(recording);
    writeTimers();
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
    _info->setModel(_model);
    connect(_core, SIGNAL(elapsed(int)), _info, SLOT(time(int)));
    connect(ui->listRecordings, SIGNAL(itemSelected(Timer *)), _info, SLOT(recordingInfo(Timer *)));
    connect(ui->listTimers, SIGNAL(itemSelected(Timer *)), _info, SLOT(timerInfo(Timer *)));
    connect(_info, SIGNAL(deleteRecording(Timer *)), this, SLOT(recordingDelete(Timer *)));
    connect(_info, SIGNAL(deleteTimer(Timer *)), this, SLOT(timerDelete(Timer *)));
    connect(_info, SIGNAL(playRecording(Timer *)), this, SIGNAL(play(Timer *)));
    connect(_info, SIGNAL(saveTimer(Timer *)), this, SLOT(timerSave(Timer *)));

    _trayIcon = icon;
}

void Recorder::timerDelete(Timer *timer)
{
    _model->deleteTimer(timer);
    writeTimers();
}

void Recorder::timerSave(Timer *timer)
{
    Q_UNUSED(timer)

    writeTimers();
}

void Recorder::writeTimers()
{
    _model->writeTimers();
}
