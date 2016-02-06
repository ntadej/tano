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
#include <QtCore/QEvent>
#include <QtCore/QFile>
#include <QtWidgets/QAction>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>

#include "network/NetworkUdpxy.h"
#include "channels/containers/Channel.h"
#include "channels/models/ChannelsModel.h"
#include "settings/Settings.h"
#include "timers/TimersTimeManager.h"
#include "timers/containers/Timer.h"
#include "timers/models/TimersModel.h"
#include "xmltv/containers/XmltvProgramme.h"

#include "recorder/RecorderCore.h"

#include "application/Notifications.h"
#include "common/TrayIcon.h"
#include "recorder/RecorderInfoWidget.h"
#include "recorder/RecorderNewDialog.h"
#include "recorder/RecorderTimersWidget.h"

#include "Recorder.h"

Recorder::Recorder(QWidget *parent)
    : MiniSplitter(parent),
      _actionRecord(0),
      _currentTimer(0)
{
    _core = new RecorderCore(this);
    _manager = new TimersTimeManager(this);
    _model = new TimersModel(this);
    _udpxy = new NetworkUdpxy();

    _manager->setTimersModel(_model);

    _timers = new RecorderTimersWidget(parent);
    _timers->setModel(_model);
    addWidget(_timers);
    _info = new RecorderInfoWidget(parent);
    _info->setModel(_model);
    _info->setVisible(false);
    addWidget(_info);

    setStretchFactor(0, 1);
    setStretchFactor(1, 0);

    // Connections
    connect(_core, SIGNAL(timerStop()), this, SLOT(recordStop()));

    connect(_manager, SIGNAL(timer(Timer *)), this, SLOT(recordStart(Timer *)));

    connect(_core, SIGNAL(elapsed(int)), _info, SLOT(time(int)));
    connect(_timers, SIGNAL(recordingSelected(Timer *)), _info, SLOT(recordingInfo(Timer *)));
    connect(_timers, SIGNAL(timerSelected(Timer *)), _info, SLOT(timerInfo(Timer *)));
    connect(_timers, SIGNAL(requestNew()), this, SLOT(newTimer()));
    connect(_timers, SIGNAL(requestQuick()), this, SLOT(quickRecord()));
    connect(_info, SIGNAL(deleteRecording(Timer *)), this, SLOT(recordingDelete(Timer *)));
    connect(_info, SIGNAL(deleteTimer(Timer *)), this, SLOT(timerDelete(Timer *)));
    connect(_info, SIGNAL(playRecording(Timer *)), this, SIGNAL(play(Timer *)));
    connect(_info, SIGNAL(saveTimer(Timer *)), this, SLOT(timerSave(Timer *)));
}

Recorder::~Recorder() { }

void Recorder::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            break;
        default:
            break;
    }
}

void Recorder::createSettings()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _directory = settings->recordingsDirectory();

    _core->setDefaultOutputPath(_directory);

    _udpxy->createSettings();
}

void Recorder::currentWidget(QWidget *widget)
{
    _info->setVisible(widget == this);
}

bool Recorder::isRecording() const
{
    return _core->isRecording();
}

Timer *Recorder::newInstantTimer(Channel *channel)
{
    Timer *timer = _model->createTimer(tr("Instant %1").arg(channel->name()), channel->id(), Timer::Instant);

    return timer;
}

void Recorder::newTimer()
{
    RecorderNewDialog n(false, _model, _channels, parentWidget());
    if (n.exec())
        _info->timerInfo(n.timer());
}

void Recorder::newTimerFromSchedule(XmltvProgramme *programme)
{
    RecorderNewDialog n(false, _model, _channels, parentWidget());
    _info->timerInfo(n.newTimerFromSchedule(programme));
}

void Recorder::quickRecord()
{
    RecorderNewDialog n(true, _model, _channels, parentWidget());
    if (n.exec())
        recordStart(n.timer());
}

void Recorder::recordStart(Timer *timer)
{
    if (timer->channelId().isEmpty()) {
        QMessageBox::critical(this, tr("Recorder"),
                              tr("Recording does not have a channel associated!"));
        delete timer;
        return;
    }

    Channel *c = _channels->find(timer->channelId());

    if (!c) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Recording does not have a valid channel associated!"));
        delete timer;
        return;
    } else if (!QDir(_directory).exists()) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Cannot write to %1.")
                    .arg(_directory));
        return;
    }

    _currentTimer = timer;
    if (timer->type() == Timer::Instant) {
        timer->setDate(QDate::currentDate());
        timer->setStartTime(QTime::currentTime());
    }

    _core->record(timer, c->name(), _udpxy->processUrl(c->url()));

    if (_core->isTimer())
        _info->start(timer->name(), c->name(), timer->endTime().toString("hh:mm"));
    else
        _info->start(timer->name(), c->name());

    if (_actionRecord)
        _actionRecord->setEnabled(true);

    if (_core->isTimer())
        notifications->notify(tr("Recording"), tr("Starting %1 on %2 until %3.").arg(timer->name(), c->name(), timer->startTime().toString("hh:mm")));
    else
        notifications->notify(tr("Recording"), tr("Starting %1 on %2.").arg(timer->name(), c->name()));
}


void Recorder::recordStop()
{
    if(!_core->isRecording())
        return;

    _core->stop();

    _info->stop();
    if (_actionRecord)
        _actionRecord->setEnabled(false);

    if (_currentTimer->type() == Timer::Instant) {
        _currentTimer->setEndTime(QTime::currentTime());
    } else {
        if (_currentTimer->type() == Timer::Daily) {
            Timer *tmptimer = _model->duplicateTimer(_currentTimer);
            tmptimer->setDate(_currentTimer->date().addDays(1));
        } else if (_currentTimer->type() == Timer::Weekly) {
            Timer *tmptimer = _model->duplicateTimer(_currentTimer);
            tmptimer->setDate(_currentTimer->date().addDays(7));
        }  else if (_currentTimer->type() == Timer::Weekdays) {
            Timer *tmptimer = _model->duplicateTimer(_currentTimer);
            if (_currentTimer->date().dayOfWeek() == Qt::Friday)
                tmptimer->setDate(_currentTimer->date().addDays(3));
            else
                tmptimer->setDate(_currentTimer->date().addDays(1));
        }
    }
    _currentTimer->setState(Timer::Finished);
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

void Recorder::setMediaInstance(VlcInstance *instance)
{
    _core->setMediaInstance(instance);
}

void Recorder::setChannelsModel(ChannelsModel *model)
{
    _channels = model;
    _info->setChannelsModel(model);
}

void Recorder::setWidgets(QAction *action)
{
    _actionRecord = action;
    connect(_actionRecord, SIGNAL(triggered()), this, SLOT(recordStop()));
    _info->setAction(_actionRecord);
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
