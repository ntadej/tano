/****************************************************************************
* M3UHandler.h: Reader and handler of modified m3u format
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

#ifndef TANO_M3UHANDLER_H_
#define TANO_M3UHANDLER_H_

#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "../channels/Channel.h"

class M3UHandler
{
public:
	M3UHandler(QTreeWidget *treeWidget, const bool &editable = false);
	virtual ~M3UHandler();

	void processFile(const QString &m3uFile);
	void createChannel();
	void clear();

	Channel *channelRead(QTreeWidgetItem *clickedItem);
	Channel *channelRead(const int &clickedItem);
	QString getName() const {return name;};
	QStringList getCategories() const {return categoryList;};
	QList<int> nums() const {return channelNums;};

private:
	void processList();
	QString processNum(const QString &num);

	bool edit;

	QTreeWidget *treeWidget;
	QTreeWidgetItem *item;
	Channel *channel;

	QIcon channelIcon;

	QString name;
	QStringList m3uLineList;
	QStringList categoryList;

	QList<Channel*> channels;
	QList<int> channelNums;

	QMap<QTreeWidgetItem*, Channel*> map;
	QMap<int, Channel*> nmap;
};

#endif // TANO_M3UHANDLER_H_
