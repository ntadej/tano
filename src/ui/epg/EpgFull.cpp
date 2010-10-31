/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "EpgFull.h"
#include "ui_EpgFull.h"

EpgFull::EpgFull(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::EpgFull)
{
	ui->setupUi(this);
	ui->schedule->setIdentifier(Tano::Schedule);

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
	emit requestEpg(ui->playlist->channelRead(item)->epg(), Tano::Schedule);
}

void EpgFull::openPlaylist(const QString &p)
{
	ui->playlist->open(p);
}

void EpgFull::setEpg(const EpgDayList &list,
					 const Tano::Id &identifier)
{
	ui->schedule->setEpg(list, identifier);
}
