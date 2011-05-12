/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QDate>

#include "container/xmltv/XmltvChannel.h"
#include "container/xmltv/XmltvProgramme.h"

#include "EpgScheduleChannel.h"
#include "ui_EpgScheduleChannel.h"

EpgScheduleChannel::EpgScheduleChannel(QWidget *parent)
	: QStackedWidget(parent),
	ui(new Ui::EpgScheduleChannel)
{
	ui->setupUi(this);

	connect(ui->day0, SIGNAL(itemClicked(XmltvProgramme*)), this, SIGNAL(itemClicked(XmltvProgramme*)));
	connect(ui->day1, SIGNAL(itemClicked(XmltvProgramme*)), this, SIGNAL(itemClicked(XmltvProgramme*)));
	connect(ui->day2, SIGNAL(itemClicked(XmltvProgramme*)), this, SIGNAL(itemClicked(XmltvProgramme*)));
	connect(ui->day3, SIGNAL(itemClicked(XmltvProgramme*)), this, SIGNAL(itemClicked(XmltvProgramme*)));
	connect(ui->day4, SIGNAL(itemClicked(XmltvProgramme*)), this, SIGNAL(itemClicked(XmltvProgramme*)));
}

EpgScheduleChannel::~EpgScheduleChannel()
{
	delete ui;
}

void EpgScheduleChannel::changeEvent(QEvent *e)
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

void EpgScheduleChannel::setEpg(XmltvChannel *epg,
								const Tano::Id &id)
{
	if(id != _id)
		return;

	QList<XmltvProgramme *> day[5];
	for(int i = 0; i < epg->programme().size(); i++) {
		for(int k = 0; k < 5; k++) {
			if(epg->programme()[i]->start().date() == QDate::currentDate().addDays(k)) {
				day[k] << epg->programme()[i];
			}
		}
	}

	ui->day0->setEpg(day[0]);
	ui->day1->setEpg(day[1]);
	ui->day2->setEpg(day[2]);
	ui->day3->setEpg(day[3]);
	ui->day4->setEpg(day[4]);

	for(int k = 4; k >= 0; k--) {
		ui->epgTabWidget->setTabText(k, QDate::currentDate().addDays(k).toString("d.M."));

		if(day[k].isEmpty()) {
			ui->epgTabWidget->removeTab(k);
		}
	}

	setPage(1);
}

void EpgScheduleChannel::setPage(const int &id)
{
	setCurrentIndex(id);
}
