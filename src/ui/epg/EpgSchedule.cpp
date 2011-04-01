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

#include <QtCore/QDate>

#include "container/epgold/EpgDayList.h"

#include "EpgSchedule.h"
#include "ui_EpgSchedule.h"

EpgSchedule::EpgSchedule(QWidget *parent)
	: QStackedWidget(parent),
	ui(new Ui::EpgSchedule)
{
	ui->setupUi(this);

	connect(ui->epgToday, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
	connect(ui->epgToday_2, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
	connect(ui->epgToday_3, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
	connect(ui->epgToday_4, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
}

EpgSchedule::~EpgSchedule()
{
	delete ui;
}

void EpgSchedule::changeEvent(QEvent *e)
{
	QStackedWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void EpgSchedule::clear()
{
	ui->epgToday->clearList();
	ui->epgToday_2->clearList();
	ui->epgToday_3->clearList();
	ui->epgToday_4->clearList();
}

void EpgSchedule::setEpg(const EpgDayList &epg,
						 const Tano::Id &id)
{
	if(id != _id)
		return;

	switch (epg.day()) {
		case 0:
			ui->epgTabWidget->setTabText(0, tr("Today"));
			ui->epgToday->setEpg(epg);
			setPage(1);
			break;
		case 1:
			ui->epgTabWidget->setTabText(1, tr("Tomorrow"));
			ui->epgToday_2->setEpg(epg);
			break;
		case 2:
			ui->epgTabWidget->setTabText(2, QDate::currentDate().addDays(epg.day()).toString("dddd"));
			ui->epgToday_3->setEpg(epg);
			break;
		case 3:
			ui->epgTabWidget->setTabText(3, QDate::currentDate().addDays(epg.day()).toString("dddd"));
			ui->epgToday_4->setEpg(epg);
			break;
		default:
			break;
	}
}

void EpgSchedule::setPage(const int &id)
{
	setCurrentIndex(id);
}
