/****************************************************************************
* Recorder.cpp: Class for recording management
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "Recorder.h"
#include "ui_Recorder.h"

#include <QtCore/QPluginLoader>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "channels/Channel.h"
#include "core/Settings.h"
#include "plugins/PluginsLoader.h"

Recorder::Recorder(QWidget *parent) :
	QWidget(parent), ui(new Ui::Recorder), _recording(false), _isTimer(false),
	_channelName(""), _channelUrl(""),_plugin(0), _trayIcon(0), _actionRecord(0), _currentTimer(0)
{
	ui->setupUi(this);

	//Init
	_timer = new QTimer(this);

	connect(_timer, SIGNAL(timeout()), this, SLOT(sec()));

	connect(ui->buttonBrowse, SIGNAL(clicked()), this, SLOT(fileBrowse()));
	connect(ui->buttonRecord, SIGNAL(toggled(bool)), this, SLOT(record(bool)));

	connect(ui->playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));

	createSettings();
}

Recorder::~Recorder()
{
	delete ui;
	delete _timer;
	delete _plugin;
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

	if(_plugin)
		delete _plugin;

	PluginsLoader *loader = new PluginsLoader();
	for(int i=0; i < loader->recorderPlugin().size(); i++)
		if(loader->recorderName()[i] == settings->recorderPlugin())
			_plugin = loader->recorder(loader->recorderPlugin()[i]);
	delete loader;

	delete settings;
}

void Recorder::stop()
{
	_plugin->stop();
}

void Recorder::openPlaylist(const QString &file)
{
	ui->playlistWidget->open(file);
}

void Recorder::playlist(QTreeWidgetItem* clickedChannel)
{
	Channel *channel = ui->playlistWidget->channelRead(clickedChannel);

	_channelName = channel->name();
	_channelUrl = channel->url();

	ui->valueSelected->setText(channel->name());

	delete channel;
}

void Recorder::fileBrowse()
{
	QString dir;
	if(ui->fileEdit->text() == "")
		dir = QDir::homePath();
	else
		dir = ui->fileEdit->text();
	QString dfile = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
							dir,
							QFileDialog::ShowDirsOnly
							| QFileDialog::DontResolveSymlinks);
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

		_plugin->record(_channelName, _channelUrl, ui->fileEdit->text());

		QString fileName;
		fileName = _plugin->output();

		if(_trayIcon)
			_trayIcon->changeToolTip(_channelName, "recorder");

		_timer->start(1000);
		_time = QTime(0,0);

		ui->valueCurrent->setText(_channelName);
		ui->valueTime->setText(_time.toString("hh:mm:ss"));
		if(_isTimer)
			ui->valueEndTime->setText(_currentTimer->endTime().toString("hh:mm"));
		else
			ui->valueEndTime->setText(tr("No timer - press button to stop."));
		ui->valueFile->setText(fileName);

		ui->buttonRecord->setText(tr("Stop recording"));
		if(_actionRecord)
			_actionRecord->setEnabled(true);

		_trayIcon->message(QStringList() << "record" << _channelName << fileName);

		_recording = true;
	} else {
		_plugin->stop();
		_timer->stop();
		ui->valueCurrent->setText("");
		ui->valueTime->setText("");
		ui->valueEndTime->setText(tr(""));
		ui->valueFile->setText("");

		ui->buttonRecord->setText(tr("Record"));
		if(_actionRecord)
			_actionRecord->setEnabled(false);

		_trayIcon->changeToolTip("stop", "recorder");

		_recording = false;
		_isTimer = false;
	}
}

void Recorder::recordNow(const QString &name, const QString &url)
{
	_channelName = name;
	_channelUrl = url;

	ui->valueSelected->setText(name);

	if(!_recording)
		ui->buttonRecord->toggle();
}

void Recorder::sec()
{
	_time = _time.addSecs(1);
	ui->valueTime->setText(_time.toString("hh:mm:ss"));
}

void Recorder::setGlobals(TrayIcon *icon, QAction *action)
{
	_trayIcon = icon;
	_actionRecord = action;
	connect(_actionRecord, SIGNAL(triggered()), ui->buttonRecord, SLOT(toggle()));
}

void Recorder::recordTimer(Timer *timer)
{
	_isTimer = true;
	_currentTimer = timer;
	_channelName = timer->channel();
	_channelUrl = timer->url();
	ui->valueSelected->setText(timer->channel());

	if(!_recording)
		ui->buttonRecord->toggle();
}

void Recorder::stopTimer(Timer *timer)
{
	if(_currentTimer != timer)
		return;

	_isTimer = false;
	if(_recording)
		ui->buttonRecord->toggle();
	_currentTimer = 0;
}
