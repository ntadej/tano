/****************************************************************************
* PlaylistWidget.h: Playlist display with search and categories support
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

#ifndef TANO_PLAYLISTWIDGET_H_
#define TANO_PLAYLISTWIDGET_H_

#include <QtGui/QWidget>

#include "channels/Channel.h"
#include "xml/M3UHandler.h"

namespace Ui
{
	class PlaylistWidget;
}

class PlaylistWidget : public QWidget
{
Q_OBJECT
public:
	PlaylistWidget(QWidget *parent = 0);
	~PlaylistWidget();

	void clear();
	void open(const QString &file);
	void save(const QString &name,
			  const QString &epg,
			  const QString &file);

	QTreeWidgetItem* createItem(const QString &name = 0,
								const QString &url = 0);
	void deleteItem();

	QString name() const { return _handler->name(); }
	QString epgPlugin() const { return _handler->epgPlugin(); }
	QStringList epg() const { return _handler->epg(); }
	QString fileName() const { return _fileName; }
	QList<int> nums() const { return _handler->nums(); }
	QTreeWidget *treeWidget();

	void import(const QString &file);
	int processNum(QTreeWidgetItem *channel,
				   const int &num) const { return _handler->processNewNum(channel, num); }
	void moveUp(QTreeWidgetItem *channel) { _handler->moveUp(channel); }
	void moveDown(QTreeWidgetItem *channel) { _handler->moveDown(channel); }
	void disableCategories();

	Channel *channelRead(QTreeWidgetItem* clickedChannel) {	return _handler->channelRead(clickedChannel); }
	Channel *channelRead(const int &clickedChannel) { return _handler->channelRead(clickedChannel); }

protected:
	void changeEvent(QEvent *e);

signals:
	void itemClicked(QTreeWidgetItem*,
					 const int);

private slots:
	void processPlaylist();

private:
	Ui::PlaylistWidget *ui;

	M3UHandler *_handler;
	QString _fileName;
};

#endif // TANO_PLAYLISTWIDGET_H_
