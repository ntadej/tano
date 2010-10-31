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

#ifndef TANO_EPGLIST_H_
#define TANO_EPGLIST_H_

#include <QtCore/QMap>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>
#include <QtGui/QTableWidget>

#include "container/EpgDayList.h"

class EpgList : public QTableWidget
{
Q_OBJECT
public:
	EpgList(QWidget *parent = 0);
	~EpgList();

	void clearList();

public slots:
	void setEpg(const EpgDayList &epg);

signals:
	void urlClicked(QString);

protected:
	void mouseReleaseEvent(QMouseEvent *event);

private slots:
	void clicked(QTableWidgetItem *item = 0);

private:
	QAction *_info;
	EpgDayList _list;
	QAction *_record;
	QMenu *_rightMenu;
};

#endif // TANO_EPGLIST_H_
