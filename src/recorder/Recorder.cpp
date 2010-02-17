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

#include <QtCore/QPluginLoader>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "../Common.h"
#include "../plugins/PluginsLoader.h"
#include "Recorder.h"

#include <QDebug>

Recorder::Recorder(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	//Init
	settings = Common::settings();
	settings->beginGroup("Recorder");
	ui.fileEdit->setText(settings->value("dir",QDir::homePath()+"/Videos").toString());
	settings->endGroup();

	recording = false;

	trayIcon = 0;
	actionRecord = 0;

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(sec()));

	connect(ui.buttonBrowse, SIGNAL(clicked()), this, SLOT(fileBrowse()));
	connect(ui.buttonRecord, SIGNAL(toggled(bool)), this, SLOT(record(bool)));

	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));

	PluginsLoader *loader = new PluginsLoader();
	for(int i=0; i < loader->recorderPlugin().size(); i++)
		if(loader->recorderName()[i] == "FripPlugin")
			plugin = loader->recorder(loader->recorderPlugin()[i]);
	delete loader;
}

Recorder::~Recorder()
{
	delete trayIcon;
	delete timer;
	delete settings;
}

void Recorder::stop()
{
	plugin->stop();
}

void Recorder::openPlaylist(const QString &file)
{
	ui.playlistWidget->open(file);
}

void Recorder::playlist(QTreeWidgetItem* clickedChannel)
{
	channel = ui.playlistWidget->channelRead(clickedChannel);
	if (channel->isCategory() != true) {
		ui.valueSelected->setText(channel->name());
	}
}

void Recorder::fileBrowse()
{
	QString dir;
	if(ui.fileEdit->text() == "")
		dir = QDir::homePath();
	else
		dir = ui.fileEdit->text();
	QString dfile = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
							dir,
							QFileDialog::ShowDirsOnly
							| QFileDialog::DontResolveSymlinks);
	if(dfile != "")
		ui.fileEdit->setText(dfile);
}

void Recorder::record(const bool &status)
{
	if(status) {
		if(ui.fileEdit->text() == "") {
			ui.buttonRecord->setChecked(false);
			return;
		} else if(!QDir(ui.fileEdit->text()).exists()) {
			ui.buttonRecord->setChecked(false);
			QMessageBox::critical(this, tr("Recorder"),
						tr("Cannot write to %1.")
						.arg(ui.fileEdit->text()));
			return;
		} else if(ui.valueSelected->text() == "") {
			ui.buttonRecord->setChecked(false);
			QMessageBox::critical(this, tr("Recorder"),
						tr("Channel is not selected!"));
			return;
		}

		plugin->record(channel->name(), channel->url(), ui.fileEdit->text());

		QString fileName;
		fileName = plugin->output();

		if(trayIcon)
			trayIcon->changeToolTip(channel->name(), "recorder");

		timer->start(1000);
		time = QTime(0,0);

		ui.valueCurrent->setText(channel->name());
		ui.valueTime->setText(time.toString("hh:mm:ss"));
		ui.valueRemaining->setText(tr("No timer - press button to stop."));
		ui.valueFile->setText(fileName);

		ui.buttonRecord->setText(tr("Stop recording"));
		if(actionRecord)
			actionRecord->setEnabled(true);

		if (trayIcon && trayIcon->isVisible()) {
			trayIcon->message(QStringList() << "record" << channel->name() << fileName);
		}

		recording = true;
	} else {
		plugin->stop();
		timer->stop();
		ui.valueCurrent->setText("");
		ui.valueTime->setText("");
		ui.valueRemaining->setText(tr(""));
		ui.valueFile->setText("");

		ui.buttonRecord->setText(tr("Record"));
		if(actionRecord)
			actionRecord->setEnabled(false);

		trayIcon->changeToolTip("stop", "recorder");

		recording = false;
	}
}

void Recorder::recordNow(const int &nmb, const QString &url, const QString &name)
{
	settings->beginGroup("Recorder");
	ui.fileEdit->setText(settings->value("dir",QDir::homePath()+"/Videos").toString());
	settings->endGroup();

	channel = ui.playlistWidget->channelRead(nmb);
	if (channel->isCategory() != true) {
		ui.valueSelected->setText(channel->name());
		if(actionRecord)
			actionRecord->setEnabled(true);
	} else {
		QMessageBox::critical(this, tr("Recorder"),
					tr("Channel is not selected!"));
	}
}

bool Recorder::isRecording()
{
	return recording;
}

void Recorder::sec()
{
	time = time.addSecs(1);
	ui.valueTime->setText(time.toString("hh:mm:ss"));
}

void Recorder::setGlobals(TrayIcon *icon, QAction *action)
{
	trayIcon = icon;
	actionRecord = action;
	connect(actionRecord, SIGNAL(triggered()), ui.buttonRecord, SLOT(toggle()));
}

void Recorder::recordTimer(const Timer *timer)
{

}

void Recorder::stopTimer(const Timer *timer)
{

}
