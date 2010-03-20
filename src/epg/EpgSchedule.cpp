/****************************************************************************
* EpgShedule.cpp: EPG schedule GUI class
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

#include <QtCore/QDate>

#include "EpgSchedule.h"

EpgSchedule::EpgSchedule(QWidget *parent)
	: QStackedWidget(parent)
{
	ui.setupUi(this);

	connect(ui.epgToday, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
	connect(ui.epgToday_2, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
	connect(ui.epgToday_3, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
	connect(ui.epgToday_4, SIGNAL(urlClicked(QString)), this, SIGNAL(urlClicked(QString)));
}

EpgSchedule::~EpgSchedule()
{

}

void EpgSchedule::clear()
{
	ui.epgToday->clearList();
	ui.epgToday_2->clearList();
	ui.epgToday_3->clearList();
	ui.epgToday_4->clearList();
}

void EpgSchedule::setEpg(const QStringList &epgValue, const int &id)
{
	switch (id) {
		case 1:
			ui.epgTabWidget->setTabText(0,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday->setEpg(epgValue);
			break;
		case 2:
			ui.epgTabWidget->setTabText(1,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday_2->setEpg(epgValue);
			break;
		case 3:
			ui.epgTabWidget->setTabText(2,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday_3->setEpg(epgValue);
			break;
		case 4:
			ui.epgTabWidget->setTabText(3,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday_4->setEpg(epgValue);
			break;
		default:
			break;
	}
}

void EpgSchedule::setPage(const int &id)
{
	setCurrentIndex(id);
}
