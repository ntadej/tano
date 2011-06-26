/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#include "container/Channel.h"
#include "core/DaemonManager.h"
#include "core/Enums.h"
#include "core/Settings.h"
#include "core/Udpxy.h"
#include "ui/core/FileDialogs.h"
#include "ui/core/TrayIcon.h"
#include "ui/recorder/RecorderController.h"
#include "ui/recorder/TimersEdit.h"

#include "Recorder.h"
#include "ui_Recorder.h"

Recorder::Recorder(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Recorder),
      _name(""),
      _url(""),
      _editor(0),
      _actionRecord(0),
      _trayIcon(0)
{
    ui->setupUi(this);
    ui->browseDirectory->setType(FileDialogs::Directory);

    _controller = new RecorderController(this);
    _daemon = new DaemonManager(this);

    _udpxy = new Udpxy();

	//Init
	connect(ui->buttonRecord, SIGNAL(toggled(bool)), this, SLOT(record(bool)));

    connect(ui->playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(playlist(QTreeWidgetItem *)));

    connect(_controller, SIGNAL(elapsed(int)), this, SLOT(time(int)));
    connect(_controller, SIGNAL(timer(QString, QString)), this, SLOT(timerStart(QString, QString)));
    connect(_controller, SIGNAL(timerStop()), this, SLOT(timerStop()));
}

Recorder::~Recorder()
{
    delete ui;

    delete _controller;
    delete _daemon;
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

void Recorder::createSettings()
{
    Settings *settings = new Settings(this);
    ui->browseDirectory->setResetValue(settings->recorderDirectory());
    ui->browseDirectory->setValue(settings->recorderDirectory());
    delete settings;

    _controller->refreshSettings();
    _udpxy->createSettings();
}

void Recorder::stop()
{
    _controller->stop();
}

void Recorder::openPlaylist(const QString &file)
{
    _playlist = file;
    ui->playlistWidget->open(_playlist);
}

void Recorder::playlist(QTreeWidgetItem *clickedChannel)
{
    Channel *channel = ui->playlistWidget->channelRead(clickedChannel);

	_name = channel->name();
    _url = _udpxy->processUrl(channel->url());

    ui->valueSelected->setText("<b>" + channel->name() + "</b>");
}

void Recorder::record(const bool &status)
{
    if(status) {
        if(ui->browseDirectory->value().isEmpty()) {
            ui->buttonRecord->setChecked(false);
            return;
        } else if(!QDir(ui->browseDirectory->value()).exists()) {
            ui->buttonRecord->setChecked(false);
            QMessageBox::critical(this, tr("Recorder"),
                        tr("Cannot write to %1.")
                        .arg(ui->browseDirectory->value()));
            return;
        } else if(ui->valueSelected->text().isEmpty() && !_controller->isTimer()) {
            ui->buttonRecord->setChecked(false);
            QMessageBox::critical(this, tr("Recorder"),
                        tr("Channel is not selected!"));
            return;
        }

        if(!_controller->isTimer()) {
            _controller->record(_name, _url, ui->browseDirectory->value());
        }

        ui->valueCurrent->setText(_name);
        if(_controller->isTimer())
            ui->valueEndTime->setText(_controller->timerEndTime());
        else
            ui->valueEndTime->setText(tr("No timer - press button to stop."));
        ui->valueFile->setText(_controller->output());

        ui->buttonRecord->setText(tr("Stop recording"));
        if(_actionRecord)
            _actionRecord->setEnabled(true);

        if(_trayIcon) {
            _trayIcon->changeToolTip(Tano::Record, _name);
            if(_controller->isTimer())
                _trayIcon->message(Tano::Record, QStringList() << _name << _controller->output() << ui->valueEndTime->text());
            else
                _trayIcon->message(Tano::Record, QStringList() << _name << _controller->output());
        }
    } else {
        _controller->stop();

        ui->valueCurrent->setText("");
        ui->valueTime->setText("");
        ui->valueEndTime->setText(tr(""));
        ui->valueFile->setText("");

        ui->buttonRecord->setText(tr("Record"));
        if(_actionRecord)
            _actionRecord->setEnabled(false);

        if(_trayIcon) {
            _trayIcon->changeToolTip(Tano::Record);
            _trayIcon->message(Tano::Record, QStringList());
        }
    }
}

void Recorder::recordNow(const QString &name,
                         const QString &url)
{
	_name = name;
    _url = _udpxy->processUrl(url);

    ui->valueSelected->setText("<b>" + name + "</b>");

    if(!_controller->isRecording())
        ui->buttonRecord->toggle();
}

void Recorder::time(const int &time)
{
    ui->valueTime->setText(QTime().addMSecs(time).toString("hh:mm:ss"));

    if(ui->valueCurrent->text().isEmpty()) {
        _controller->timerInfo();
    }
}

void Recorder::setAction(QAction *action)
{
    _actionRecord = action;
    connect(_actionRecord, SIGNAL(triggered()), ui->buttonRecord, SLOT(toggle()));
}

void Recorder::setTrayIcon(TrayIcon *icon)
{
    _trayIcon = icon;
}

bool Recorder::isRecording() const
{
    return _controller->isRecording();
}

void Recorder::showTimersEditor()
{
    if(_editor) {
        if(_editor->isVisible()) {
            _editor->activateWindow();
        } else {
            delete _editor;
            _editor = new TimersEdit(_playlist, this);
            connect(_editor, SIGNAL(updateTimers()), _controller, SLOT(refreshTimers()));
            _editor->show();
        }
    } else {
        _editor = new TimersEdit(_playlist, this);
        connect(_editor, SIGNAL(updateTimers()), _controller, SLOT(refreshTimers()));
        _editor->show();
    }
}

void Recorder::timerStart(const QString &name,
                          const QString &url)
{
    _name = name;
    _url = url;

    ui->buttonRecord->toggle();
}

void Recorder::timerStop()
{
    if(isRecording())
        ui->buttonRecord->toggle();
}
