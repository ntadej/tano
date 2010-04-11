/****************************************************************************
* Schedule.cpp: Schedule widget
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

#include "Schedule.h"
#include "ui_Schedule.h"

Schedule::Schedule(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Schedule),
	_epg(new EpgManager())
{
	ui->setupUi(this);

	connect(_epg, SIGNAL(epg(QStringList, int)), this, SLOT(loadEpg(QStringList, int)));
	connect(ui->playlist, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(channel(QTreeWidgetItem*)));
	connect(ui->schedule, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
}

Schedule::~Schedule()
{
	delete ui;
	delete _epg;
}

void Schedule::changeEvent(QEvent *e)
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

void Schedule::channel(QTreeWidgetItem *item)
{
	ui->schedule->setPage(0);
	_epg->request(ui->playlist->channelRead(item)->epg());
}

void Schedule::openPlaylist(const QString &p)
{
	ui->playlist->open(p);
}

void Schedule::loadEpg(const QStringList &list, const int &day)
{
	ui->schedule->setEpg(list, day);
	ui->schedule->setPage(1);
}
