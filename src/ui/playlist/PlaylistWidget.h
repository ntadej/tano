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

#ifndef TANO_PLAYLISTWIDGET_H_
#define TANO_PLAYLISTWIDGET_H_

#include <QtGui/QWidget>

#include "container/Channel.h"
#include "core/Common.h"
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
	Tano::EpgType epgType() const { return _handler->epgType(); }
	QStringList epg() const { return _handler->epg(); }
	QString fileName() const { return _fileName; }
	QList<int> nums() const { return _handler->nums(); }
	QTreeWidget *treeWidget();

	void import(const QString &file);
	int processNum(QTreeWidgetItem *channel,
				   const int &num) const { return _handler->processNewNum(channel, num); }
	void moveUp(QTreeWidgetItem *channel) { _handler->moveUp(channel); }
	void moveDown(QTreeWidgetItem *channel) { _handler->moveDown(channel); }
	void editMode();

	Channel *channelRead(QTreeWidgetItem* clickedChannel) {	return _handler->channelRead(clickedChannel); }
	Channel *channelRead(const int &clickedChannel) { return _handler->channelRead(clickedChannel); }

	bool validate() const { return _handler->validate(); }

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
