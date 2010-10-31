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

#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>

#include "container/EpgItem.h"

#include "EpgList.h"

EpgList::EpgList(QWidget *parent)
    : QTableWidget(parent)
{
	setColumnCount(1);
	horizontalHeader()->setStretchLastSection(true);
	verticalHeader()->hide();

	_rightMenu = new QMenu();
	_info = new QAction(QIcon(":/icons/128x128/schedule.png"), tr("Show information"), this);
	_record = new QAction(QIcon(":/icons/22x22/record.png"), tr("Record"), this);
	_rightMenu->addAction(_info);
	//_rightMenu->addAction(_record);

	clearList();

	connect(_info, SIGNAL(triggered()), this, SLOT(clicked()));
}

EpgList::~EpgList()
{
	delete _info;
	delete _record;
	delete _rightMenu;
}

void EpgList::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::RightButton) {
		event->ignore();
		_rightMenu->exec(event->globalPos());
	} else {
		clicked(currentItem());
	}
}

void EpgList::setEpg(const EpgDayList &epg)
{
	clearList();
	_list = epg;

	setRowCount(epg.size());

	QStringList epgHeader;
	epgHeader << epg.channel() + ", " + epg.date().addDays(epg.day()).toString("dd.MM.yyyy");
	this->setHorizontalHeaderLabels(epgHeader);

	for (int i = 0; i < epg.size(); i++) {
		QTableWidgetItem *newItem = new QTableWidgetItem(QString(epg[i].time().toString("hh:mm") + " - " + epg[i].title()));
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		setItem(i, 0, newItem);
	}
}

void EpgList::clearList() {
	clear();
	setRowCount(0);
	setHorizontalHeaderLabels(QStringList() << tr("No EPG"));
}

void EpgList::clicked(QTableWidgetItem *item) {
	if(item == 0)
		emit urlClicked(_list[row(currentItem())].url());
	else
		emit urlClicked(_list[row(item)].url());
}
