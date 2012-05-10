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

#include <QtGui/QMouseEvent>

#if defined(Qt5)
    #include <QtWidgets/QAction>
    #include <QtWidgets/QMenu>
#elif defined(Qt4)
    #include <QtGui/QAction>
    #include <QtGui/QMenu>
#endif

#include "container/xmltv/XmltvProgramme.h"
#include "EpgList.h"

EpgList::EpgList(QWidget *parent)
	: QListWidget(parent)
{
	setAlternatingRowColors(true);

	_rightMenu = new QMenu();
	_info = new QAction(QIcon(":/icons/24x24/calendar.png"), tr("Show information"), this);
	_record = new QAction(QIcon(":/icons/24x24/record.png"), tr("Record"), this);
	_rightMenu->addAction(_info);
	//_rightMenu->addAction(_record);

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

void EpgList::setEpg(const QList<XmltvProgramme *> &epg)
{
	clear();
	_list.clear();

	for(int i = 0; i < epg.size(); i++) {
		addItem(QString("%1 - %2").arg(epg[i]->start().toString("hh:mm"), epg[i]->title()));
		_list.insert(item(count() - 1), epg[i]);
		item(count() - 1)->setIcon(QIcon(":/icons/16x16/calendar.png"));
	}
}

void EpgList::clicked(QListWidgetItem *item)
{
	if(item == 0)
		emit itemClicked(_list[currentItem()]);
	else
		emit itemClicked(_list[item]);
}
