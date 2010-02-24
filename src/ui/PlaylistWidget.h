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

#include "../channels/Channel.h"
#include "../xml/M3UHandler.h"

#include <ui_PlaylistWidget.h>

class PlaylistWidget : public QWidget
{
Q_OBJECT
public:
	PlaylistWidget(QWidget *parent = 0);
	~PlaylistWidget();

	void clear();
	void open(const QString &file);
	void save(const QString &name, const QString &file);

	QTreeWidgetItem* createItem();
	void deleteItem();

	QString name() const {return _handler->name();};
	QString fileName() const {return _fileName;};
	QList<int> nums() const {return _handler->nums();};
	QTreeWidget *treeWidget() {return ui.treeWidget;};

	void import(const QString &file);
	int processNum(QTreeWidgetItem *channel, const int &num);
	void disableCategories();

	Channel *channelRead(QTreeWidgetItem* clickedChannel);
	Channel *channelRead(const int &clickedChannel);

signals:
	void itemClicked(QTreeWidgetItem*, const int);

private slots:
	void processPlaylist();

private:
	Ui::PlaylistWidget ui;

	M3UHandler *_handler;
	QString _fileName;
};

#endif // TANO_PLAYLISTWIDGET_H_
