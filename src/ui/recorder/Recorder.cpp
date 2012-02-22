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
#include "ui/core/TrayIcon.h"
#include "ui/recorder/TimersEdit.h"

#include "Recorder.h"
#include "ui_Recorder.h"

Recorder::Recorder(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Recorder),
      _actionRecord(0),
      _name(""),
      _url(""),
      _editor(0),
      _trayIcon(0)
{
    ui->setupUi(this);
    ui->browseDirectory->setType(Tano::Directory);

    _core = new RecorderCore(this);

    _udpxy = new Udpxy();

	//Init
	connect(ui->buttonRecord, SIGNAL(toggled(bool)), this, SLOT(record(bool)));

	connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playlist(Channel *)));

    connect(_core, SIGNAL(elapsed(int)), this, SLOT(time(int)));
    connect(_core, SIGNAL(timer(QString, QUrl)), this, SLOT(timerStart(QString, QUrl)));
    connect(_core, SIGNAL(timerStop()), this, SLOT(timerStop()));
}

Recorder::~Recorder()
{
    delete ui;

    delete _core;
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

    _core->settings();
    _udpxy->createSettings();
}

void Recorder::stop()
{
    _core->stop();
}

void Recorder::playlist(Channel *channel)
{
	_name = channel->name();
    _url = _udpxy->processUrl(channel->url());

    ui->valueSelected->setText("<b>" + channel->name() + "</b>");
}

void Recorder::record(const bool &status)
{
    if (status) {
        if (ui->browseDirectory->value().isEmpty()) {
            ui->buttonRecord->setChecked(false);
            return;
        } else if (!QDir(ui->browseDirectory->value()).exists()) {
            ui->buttonRecord->setChecked(false);
            QMessageBox::critical(this, tr("Recorder"),
                        tr("Cannot write to %1.")
                        .arg(ui->browseDirectory->value()));
            return;
        } else if (ui->valueSelected->text().isEmpty() && !_core->isTimer()) {
            ui->buttonRecord->setChecked(false);
            QMessageBox::critical(this, tr("Recorder"),
                        tr("Channel is not selected!"));
            return;
        }

        if (!_core->isTimer()) {
            _core->record(_name, _url, ui->browseDirectory->value());
        }

        ui->valueCurrent->setText(_name);
        if (_core->isTimer())
            ui->valueEndTime->setText(_core->timerEndTime());
        else
            ui->valueEndTime->setText(tr("No timer - press button to stop."));
        ui->valueFile->setText(_core->output());

        ui->buttonRecord->setText(tr("Stop recording"));
        if (_actionRecord)
            _actionRecord->setEnabled(true);

        if (_trayIcon) {
            _trayIcon->changeToolTip(Tano::Record, _name);
            if (_core->isTimer())
                _trayIcon->message(Tano::Record, QStringList() << _name << _core->output() << ui->valueEndTime->text());
            else
                _trayIcon->message(Tano::Record, QStringList() << _name << _core->output());
        }
    } else {
        _core->stop();

        ui->valueCurrent->setText("");
        ui->valueTime->setText("");
        ui->valueEndTime->setText(tr(""));
        ui->valueFile->setText("");

        ui->buttonRecord->setText(tr("Record"));
        if (_actionRecord)
            _actionRecord->setEnabled(false);

        if (_trayIcon) {
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

    if (!_core->isRecording())
        ui->buttonRecord->toggle();
}

void Recorder::time(const int &time)
{
    ui->valueTime->setText(QTime().addMSecs(time).toString("hh:mm:ss"));

    if (ui->valueCurrent->text().isEmpty()) {
        _core->timerInfo();
    }
}

void Recorder::refreshPlaylistModel()
{
    ui->playlistWidget->refreshModel();
}

void Recorder::setAction(QAction *action)
{
    _actionRecord = action;
    connect(_actionRecord, SIGNAL(triggered()), ui->buttonRecord, SLOT(toggle()));
}

void Recorder::setPlaylistModel(PlaylistModel *model)
{
    _model = model;
    ui->playlistWidget->setModel(model);
}

void Recorder::setTrayIcon(TrayIcon *icon)
{
    _trayIcon = icon;
}

bool Recorder::isRecording() const
{
    return _core->isRecording();
}

void Recorder::showTimersEditor()
{
    if (_editor) {
        if (_editor->isVisible()) {
            _editor->activateWindow();
        } else {
            delete _editor;
            _editor = new TimersEdit(_model, this);
            connect(_editor, SIGNAL(updateTimers()), _core, SLOT(refreshTimers()));
            _editor->show();
        }
    } else {
        _editor = new TimersEdit(_model, this);
        connect(_editor, SIGNAL(updateTimers()), _core, SLOT(refreshTimers()));
        _editor->show();
    }
}

void Recorder::timerStart(const QString &name,
                          const QUrl &url)
{
    _name = name;
    _url = url;

    ui->buttonRecord->toggle();
}

void Recorder::timerStop()
{
    if (isRecording())
        ui->buttonRecord->toggle();
}
