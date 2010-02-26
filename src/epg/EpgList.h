/****************************************************************************
* EpgList.h: EPG Schedule display
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

#ifndef TANO_EPGLIST_H_
#define TANO_EPGLIST_H_

#include <QtCore/QMap>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>
#include <QtGui/QTableWidget>

#include "../channels/ChannelEpg.h"

class EpgList : public QTableWidget
{
Q_OBJECT
public:
	EpgList(QWidget *parent = 0);
	~EpgList();

	void clearList();

public slots:
	void setEpg(const QStringList &epg);

signals:
	void urlClicked(QString);

protected:
	void mouseReleaseEvent(QMouseEvent *event);

private slots:
	void clicked(QTableWidgetItem *item = 0);

private:
	QMap<QTableWidgetItem*, ChannelEpg*> _map;
	QMap<int, ChannelEpg*> _nmap;

	QMenu *_rightMenu;
	QAction *_info;
	QAction *_record;
};

#endif // TANO_EPGLIST_H_
