/****************************************************************************
* EpgFull.cpp: Independent schedule widget
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

#include "EpgFull.h"
#include "ui_EpgFull.h"

const QString EpgFull::IDENTIFIER = "schedule";

EpgFull::EpgFull(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::EpgFull)
{
	ui->setupUi(this);

	connect(ui->playlist, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(channel(QTreeWidgetItem*)));
	connect(ui->schedule, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
}

EpgFull::~EpgFull()
{
	delete ui;
}

void EpgFull::changeEvent(QEvent *e)
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

void EpgFull::channel(QTreeWidgetItem *item)
{
	ui->schedule->setPage(0);
	emit requestEpg(ui->playlist->channelRead(item)->epg(), EpgFull::IDENTIFIER);
}

void EpgFull::openPlaylist(const QString &p)
{
	ui->playlist->open(p);
}

void EpgFull::loadEpg(const QStringList &list,
					   const int &day,
					   const QString &identifier)
{
	if(identifier != EpgFull::IDENTIFIER)
		return;

	ui->schedule->setEpg(list, day);
	ui->schedule->setPage(1);
}
