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

#include "Config.h"
#include "core/Common.h"
#include "core/Settings.h"
#include "plugins/PluginsLoader.h"

Mobile::Mobile(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::Mobile), _locale(new LocaleManager())
{
    ui->setupUi(this);

	Settings *settings = new Settings(this);
	QString defaultPlaylist = settings->playlist();
	delete settings;

	_playlist = Common::locateResource(defaultPlaylist);

	connect(ui->buttonContinue, SIGNAL(clicked()), this, SLOT(read()));

	QString version;
#ifdef VERSION
	version = QString(VERSION);
#else
	version = "Unknown";
#endif

	ui->labelVersion->setText(version);

	PluginsLoader *plugins = new PluginsLoader();
	qDebug() << plugins->epgFile().size();
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

void Mobile::read()
{
	ui->playlistWidget->open(_playlist);
	ui->stackedWidget->setCurrentIndex(1);
}
