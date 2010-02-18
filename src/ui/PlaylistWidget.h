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

	QTreeWidgetItem* createItem();
	void deleteItem();

	QString name() const {return handler->getName();};
	QString fileName() const {return _fileName;};
	QList<int> nums() const {return handler->nums();};
	QTreeWidget *treeWidget() {return ui.treeWidget;};

	void import(const QString &file);

public slots:
	Channel *channelRead(QTreeWidgetItem* clickedChannel);
	Channel *channelRead(const int &clickedChannel);

signals:
	void itemClicked(QTreeWidgetItem*, const int);

private slots:
	void processCategories(const QString &cat);
	void processSearch(const QString &search);

private:
	Ui::PlaylistWidget ui;

	M3UHandler *handler;
	QString _fileName;
};

#endif // TANO_PLAYLISTWIDGET_H_
