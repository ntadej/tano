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

#include "container/core/Timer.h"
#include "container/xmltv/XmltvProgramme.h"
#include "core/settings/Settings.h"
#include "recorder/RecorderCore.h"
#include "recorder/RecorderTimeManager.h"
#include "recorder/TimersModel.h"
#include "ui/core/TrayIcon.h"
#include "ui/recorder/RecorderInfoWidget.h"
#include "ui/recorder/RecorderNewDialog.h"

#include "Recorder.h"
#include "ui_Recorder.h"

Recorder::Recorder(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Recorder),
      _actionRecord(0),
      _currentTimer(0),
      _trayIcon(0)
{
    ui->setupUi(this);

    _core = new RecorderCore(this);
    _new = new RecorderNewDialog(this);
    _manager = new RecorderTimeManager(this);
    _model = new TimersModel(this);

    _manager->setTimersModel(_model);
    _new->setTimersModel(_model);

    ui->listRecordings->setModel(_model);
    ui->recorderInfo->setModel(_model);

    // Connections
    connect(_core, SIGNAL(timerStop()), this, SLOT(recordStop()));

    connect(_manager, SIGNAL(timer(Timer *)), this, SLOT(recordStart(Timer *)));

    connect(_core, SIGNAL(elapsed(int)), ui->recorderInfo, SLOT(time(int)));
    connect(ui->listRecordings, SIGNAL(recordingSelected(Timer *)), ui->recorderInfo, SLOT(recordingInfo(Timer *)));
    connect(ui->listRecordings, SIGNAL(timerSelected(Timer *)), ui->recorderInfo, SLOT(timerInfo(Timer *)));
    connect(ui->recorderInfo, SIGNAL(deleteRecording(Timer *)), this, SLOT(recordingDelete(Timer *)));
    connect(ui->recorderInfo, SIGNAL(deleteTimer(Timer *)), this, SLOT(timerDelete(Timer *)));
    connect(ui->recorderInfo, SIGNAL(playRecording(Timer *)), this, SIGNAL(play(Timer *)));
    connect(ui->recorderInfo, SIGNAL(saveTimer(Timer *)), this, SLOT(timerSave(Timer *)));
}

Recorder::~Recorder()
{
    delete ui;

    delete _core;
    delete _model;
}

void Recorder::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

QSize Recorder::sizeHint() const
{
    return QSize(100, 100);
}

void Recorder::createSettings()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _directory = settings->recorderDirectory();

    _core->setDefaultOutputPath(_directory);
}

bool Recorder::isRecording() const
{
    return _core->isRecording();
}

Timer *Recorder::newInstantTimer(const QString &channel,
                                 const QString &url)
{
    Timer *timer = _model->createTimer(tr("Quick %1").arg(channel), channel, url, Timer::Instant);

    return timer;
}

void Recorder::newTimer()
{
    _new->newTimer();
    if (_new->exec())
        ui->recorderInfo->timerInfo(_new->timer());
}

void Recorder::newTimerFromSchedule(XmltvProgramme *programme)
{
    _new->newTimerFromSchedule(programme);
    ui->recorderInfo->timerInfo(_new->timer());
}

void Recorder::quickRecord()
{
    _new->newQuick();
    if (_new->exec())
        recordStart(_new->timer());
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
    if (timer->type() == Timer::Instant) {
        timer->setDate(QDate::currentDate());
        timer->setStartTime(QTime::currentTime());
    }

    _core->record(timer);

    if (_core->isTimer())
        ui->recorderInfo->start(timer->name(), timer->channel(), timer->file(), timer->endTime().toString("hh:mm"));
    else
        ui->recorderInfo->start(timer->name(), timer->channel(), timer->file());

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

    ui->recorderInfo->stop();
    if (_actionRecord)
        _actionRecord->setEnabled(false);

    if (_trayIcon) {
        _trayIcon->changeToolTip(Tano::Record);
        _trayIcon->message(Tano::Record, QStringList());
    }

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

void Recorder::refreshPlaylistModel()
{
    _new->refreshPlaylistModel();
}

void Recorder::setMediaInstance(VlcInstance *instance)
{
    _core->setMediaInstance(instance);
}

void Recorder::setPlaylistModel(PlaylistModel *model)
{
    _new->setPlaylistModel(model);
}

void Recorder::setWidgets(QAction *action,
                          TrayIcon *icon)
{
    _actionRecord = action;
    connect(_actionRecord, SIGNAL(triggered()), this, SLOT(recordStop()));
    ui->recorderInfo->setAction(_actionRecord);

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
