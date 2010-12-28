/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <info@tano.si>
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

#include "Recorder.h"
#include "ui_Recorder.h"

#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "container/Channel.h"
#include "core/Settings.h"
#include "ui/recorder/TimersEdit.h"

Recorder::Recorder(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::Recorder),
	_channelName(""),
	_channelUrl(""),
	_timeManager(new TimeManager()),
	_actionRecord(0),
	_currentTimer(0)
{
	ui->setupUi(this);

	_core = new RecorderCore(this);

	//Init
	connect(ui->buttonBrowse, SIGNAL(clicked()), this, SLOT(fileBrowse()));
	connect(ui->buttonRecord, SIGNAL(toggled(bool)), this, SLOT(record(bool)));

	connect(ui->playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(playlist(QTreeWidgetItem *)));

	connect(_timeManager, SIGNAL(startTimer(Timer *)), this, SLOT(recordTimer(Timer *)));
	connect(_core, SIGNAL(elapsed(QTime)), this, SLOT(time(QTime)));

	createSettings();
}

Recorder::~Recorder()
{
	delete ui;
	delete _core;
	delete _timeManager;
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
	ui->fileEdit->setText(settings->recorderDirectory());

	_core->setBackend(settings->recorderBackend());

	delete settings;
}

void Recorder::stop()
{
	_core->stop();
}

void Recorder::openPlaylist(const QString &file)
{
	_playlist = file;
	ui->playlistWidget->open(_playlist);
}

void Recorder::playlist(QTreeWidgetItem *clickedChannel)
{
	Channel *channel = ui->playlistWidget->channelRead(clickedChannel);

	_channelName = channel->name();
	_channelUrl = channel->url();

	ui->valueSelected->setText(channel->name());
}

void Recorder::fileBrowse()
{
	QString dir;
	if(ui->fileEdit->text() == "")
		dir = QDir::homePath();
	else
		dir = ui->fileEdit->text();
	QString dfile =
			QFileDialog::getExistingDirectory(this, tr("Open Directory"), dir,
											  QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	if(dfile != "")
		ui->fileEdit->setText(dfile);
}

void Recorder::record(const bool &status)
{
	if(status) {
		if(ui->fileEdit->text() == "") {
			ui->buttonRecord->setChecked(false);
			return;
		} else if(!QDir(ui->fileEdit->text()).exists()) {
			ui->buttonRecord->setChecked(false);
			QMessageBox::critical(this, tr("Recorder"),
						tr("Cannot write to %1.")
						.arg(ui->fileEdit->text()));
			return;
		} else if(ui->valueSelected->text() == "") {
			ui->buttonRecord->setChecked(false);
			QMessageBox::critical(this, tr("Recorder"),
						tr("Channel is not selected!"));
			return;
		}

		_core->record(_channelName, _channelUrl, ui->fileEdit->text());

		ui->valueCurrent->setText(_channelName);
		if(_core->isTimer())
			ui->valueEndTime->setText(_currentTimer->endTime().toString("hh:mm"));
		else
			ui->valueEndTime->setText(tr("No timer - press button to stop."));
		ui->valueFile->setText(_core->output());

		ui->buttonRecord->setText(tr("Stop recording"));
		if(_actionRecord)
			_actionRecord->setEnabled(true);
	} else {
		_core->stop();

		ui->valueCurrent->setText("");
		ui->valueTime->setText("");
		ui->valueEndTime->setText(tr(""));
		ui->valueFile->setText("");

		ui->buttonRecord->setText(tr("Record"));
		if(_actionRecord)
			_actionRecord->setEnabled(false);
	}
}

void Recorder::recordNow(const QString &name,
						 const QString &url)
{
	_channelName = name;
	_channelUrl = url;

	ui->valueSelected->setText(name);

	if(!_core->isRecording())
		ui->buttonRecord->toggle();
}

void Recorder::time(const QTime &time)
{
	ui->valueTime->setText(time.toString("hh:mm:ss"));
}

void Recorder::setAction(QAction *action)
{
	_actionRecord = action;
	connect(_actionRecord, SIGNAL(triggered()), ui->buttonRecord, SLOT(toggle()));
}

void Recorder::recordTimer(Timer *timer)
{
	/*_isTimer = true;
	_currentTimer = timer;
	_channelName = timer->channel();
	_channelUrl = timer->url();
	ui->valueSelected->setText(timer->channel());

	if(!_recording)
		ui->buttonRecord->toggle();*/
}

void Recorder::showTimersEditor()
{
	if(_editor) {
		if(_editor->isVisible()) {
			_editor->activateWindow();
		} else {
			delete _editor;
			_editor = new TimersEdit(_playlist, this);
			_editor->show();
		}
	} else {
		_editor = new TimersEdit(_playlist, this);
		_editor->show();
	}
}
