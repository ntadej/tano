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
#include "core/Enums.h"
#include "core/Settings.h"
#include "core/Udpxy.h"
#include "playlist/PlaylistModel.h"
#include "recorder/RecorderCore.h"
#include "recorder/RecorderTimeManager.h"
#include "recorder/TimersModel.h"
#include "ui/core/TrayIcon.h"
#include "ui/recorder/RecorderInfoWidget.h"
#include "ui/recorder/TimersEditor.h"

#include "Recorder.h"
#include "ui_Recorder.h"

Recorder::Recorder(QWidget *parent)
    : QStackedWidget(parent),
      ui(new Ui::Recorder),
      _actionRecord(0),
      _name(""),
      _url(""),
      _editor(0),
      _info(0),
      _trayIcon(0)
{
    ui->setupUi(this);

    _core = new RecorderCore(this);
    _manager = new RecorderTimeManager(this);
    _model = new TimersModel(this);
    _udpxy = new Udpxy();

    _manager->setTimersModel(_model);

    // Connections
    connect(ui->buttonQuickCancel, SIGNAL(clicked()), this, SLOT(quickRecordCancel()));
    connect(ui->buttonQuickRecord, SIGNAL(clicked()), this, SLOT(quickRecordStart()));

	connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playlist(Channel *)));

    connect(_core, SIGNAL(timer(QString, QString)), this, SLOT(timerStart(QString, QString)));
    connect(_core, SIGNAL(timerStop()), this, SLOT(timerStop()));

    connect(_manager, SIGNAL(timer(Timer *)), _core, SLOT(record(Timer *)));
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
    _name = _currentChannel->name();
    _url = _udpxy->processUrl(_currentChannel->url());

    recordStart();

    setCurrentIndex(0);
}

void Recorder::recordNow(const QString &name,
                         const QString &url)
{
	_name = name;
    _url = _udpxy->processUrl(url);

    ui->valueSelectedQuick->setText("<b>" + name + "</b>");

    if (isRecording())
        recordStart();
}

void Recorder::recordStart()
{
    if (!QDir(_directory).exists()) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Cannot write to %1.")
                    .arg(_directory));
        return;
    } else if (ui->valueSelectedQuick->text().isEmpty() && !_core->isTimer()) {
        QMessageBox::critical(this, tr("Recorder"),
                    tr("Channel is not selected!"));
        return;
    }

    if (!_core->isTimer())
        _core->record(_name, _url, _directory);

    _info->start(_name, _core->output(), _core->timerEndTime());
    if (_actionRecord)
        _actionRecord->setEnabled(true);

    if (_trayIcon) {
        _trayIcon->changeToolTip(Tano::Record, _name);
        if (_core->isTimer())
            _trayIcon->message(Tano::Record, QStringList() << _name << _core->output() << _core->timerEndTime());
        else
            _trayIcon->message(Tano::Record, QStringList() << _name << _core->output());
    }
}


void Recorder::recordStop()
{
    _core->stop();

    _info->stop();
    if (_actionRecord)
        _actionRecord->setEnabled(false);

    if (_trayIcon) {
        _trayIcon->changeToolTip(Tano::Record);
        _trayIcon->message(Tano::Record, QStringList());
    }
}

void Recorder::refreshPlaylistModel()
{
    ui->playlistWidget->refreshModel();
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

    _trayIcon = icon;
}

void Recorder::showTimersEditor()
{
    if (_editor) {
        if (_editor->isVisible()) {
            _editor->activateWindow();
        } else {
            delete _editor;
            _editor = new TimersEditor(this);
            _editor->setModels(_model, _playlistModel);
            _editor->show();
        }
    } else {
        _editor = new TimersEditor(this);
        _editor->setModels(_model, _playlistModel);
        _editor->show();
    }
}

void Recorder::timerStart(const QString &name,
                          const QString &url)
{
    _name = name;
    _url = url;

    recordStart();
}

void Recorder::timerStop()
{
    if (isRecording())
        recordStop();
}
