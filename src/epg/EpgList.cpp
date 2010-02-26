/****************************************************************************
* EpgList.cpp: EPG Schedule display
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

#include <QtGui/QHeaderView>
#include <QtGui/QTableWidget>

#include "EpgList.h"

EpgList::EpgList(QWidget *parent)
    : QTableWidget(parent)
{
	setColumnCount(1);
	horizontalHeader()->setStretchLastSection(true);
	verticalHeader()->hide();

	_rightMenu = new QMenu();
	_info = new QAction(QIcon(":/icons/images/schedule.png"), tr("Show information"), this);
	_record = new QAction(QIcon(":/icons/images/record.png"), tr("Record"), this);
	_rightMenu->addAction(_info);
	_rightMenu->addAction(_record);

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

void EpgList::setEpg(const QStringList &epg)
{
	clearList();
	setRowCount((epg.size() - 1)/3 + 1);

	QStringList epgHeader;
	epgHeader << epg[0];
	this->setHorizontalHeaderLabels(epgHeader);

	int r = 0;
	for (int i = 1; i < epg.size(); i+=3) {
		ChannelEpg *newEpg = new ChannelEpg(epg[i],epg[i+1],epg[i+2]);
		QTableWidgetItem *newItem = new QTableWidgetItem(QString(epg[i] + " - " + epg[i+2]));
		newItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
		_map.insert(newItem, newEpg);
		_nmap.insert(r,newEpg);
		setItem(r, 0, newItem);
		r++;
	}
}

void EpgList::clearList() {
	for(int i=0; i<rowCount(); i++)
		delete _nmap[i];
	_map.clear();
	_nmap.clear();
	clear();
	setRowCount(0);
	setHorizontalHeaderLabels(QStringList() << tr("No EPG"));
}

void EpgList::clicked(QTableWidgetItem *item) {
	if(item == 0)
		emit urlClicked(_map[currentItem()]->url());
	else
		emit urlClicked(_map[item]->url());
}
