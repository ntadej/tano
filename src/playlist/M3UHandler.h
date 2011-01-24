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

#ifndef TANO_M3UHANDLER_H_
#define TANO_M3UHANDLER_H_

#include <QtCore/QMap>
#include <QtGui/QIcon>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

class Channel;

class M3UHandler
{
public:
	M3UHandler(QTreeWidget *treeWidget);
	~M3UHandler();

	void processFile(const QString &m3uFile);
	void importOldFormat(const QString &tanoFile);

	QTreeWidgetItem *createChannel(const QString &name = 0,
								   const QString &url = 0);
	void deleteChannel(QTreeWidgetItem *channel);
	void clear();

	Channel *channelRead(QTreeWidgetItem *item) { return _map[item]; }
	Channel *channelRead(const int &item) { return _nmap[item]; }

	QString name() const { return _name; }
	QStringList categories() const { return _categoryList; }
	QStringList languages() const { return _languageList; }
	QStringList epg() const { return _epgList; }
	QList<int> nums() const { return _channelNums; }
	QMap<QTreeWidgetItem *, Channel *> channelMap() const { return _map; }

	int processNewNum(QTreeWidgetItem *channel,
					  const int &num);
	void moveUp(QTreeWidgetItem *channel);
	void moveDown(QTreeWidgetItem *channel);

	bool validate() const;

private:
	void processList();
	QString processNum(const QString &num);

	QTreeWidget *_treeWidget;
	QTreeWidgetItem *_item;
	Channel *_channel;

	QIcon _channelIcon;

	QString _name;
	QStringList _m3uLineList;
	QStringList _categoryList;
	QStringList _languageList;
	QStringList _epgList;

	QList<Channel *> _channels;
	QList<int> _channelNums;

	QMap<QTreeWidgetItem *, Channel *> _map;
	QMap<int, Channel *> _nmap;
};

#endif // TANO_M3UHANDLER_H_
