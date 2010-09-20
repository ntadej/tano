/****************************************************************************
* Mobile.cpp: Main Widget (Tano Mobile)
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

#include "Mobile.h"
#include "ui_Mobile.h"

#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtGui/QFileDialog>

#include "About.h"
#include "Config.h"
#include "EditSettings.h"
#include "core/Common.h"
#include "core/Settings.h"
#include "plugins/PluginsLoader.h"

Mobile::Mobile(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Mobile),
	_locale(new LocaleManager())
{
    ui->setupUi(this);

#ifdef VERSION
	_version = QString(VERSION);
#else
	_version = "Unknown";
#endif

	Settings *settings = new Settings(this);
	_playlist = Common::locateResource(settings->playlist());
	delete settings;

	connect(ui->buttonContinue, SIGNAL(clicked()), this, SLOT(read()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(settings()));

	PluginsLoader *plugins = new PluginsLoader();
	qDebug() << plugins->epgFile().size();

	menuBar()->addAction(ui->actionOpen);
	menuBar()->addAction(ui->actionAbout);
	menuBar()->addAction(ui->actionSettings);

	qDebug() << "Tano Mobile started!";
}

Mobile::~Mobile()
{
    delete ui;
}

void Mobile::changeEvent(QEvent *e)
{
	QMainWindow::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void Mobile::about()
{
	About about(_version, this);
	about.exec();
}

void Mobile::openPlaylist()
{
	_playlist =
		QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
						QDir::homePath(),
						tr("Tano TV Channel list Files(*.m3u)"));

	if (_playlist.isEmpty())
		return;

	ui->playlistWidget->open(_playlist);

	//_epg->setEpg(ui->playlistWidget->epg(), ui->playlistWidget->epgPlugin());
	//_epgShow->loadPlugin(ui->playlistWidget->epgPlugin());
}

void Mobile::read()
{
	ui->playlistWidget->open(_playlist);
	ui->stackedWidget->setCurrentIndex(1);
}

void Mobile::settings()
{
	EditSettings s(_version, this);
	s.exec();
	_locale->setLocale();
}
