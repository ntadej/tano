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

#ifndef TANO_PLAYLISTWIDGET_H_
#define TANO_PLAYLISTWIDGET_H_

#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

class Channel;
class PlaylistHandler;

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
	void open(const QString &file,
			  const bool &refresh = false);
	void save(const QString &name,
			  const QString &file);
	void exportM3UClean(const QString &file);
	void exportCSV(const QString &file);
	void importCSV(const QString &file,
				   const QString &separator,
				   const bool &header,
				   const QList<int> &columns);
	void exportJs(const QString &file);
	void importJs(const QString &file);
	void importTanoOld(const QString &file);
	void exportTvheadend(const QString &location,
						 const QString &interface,
						 const QString &xmltv);

	QTreeWidgetItem* createItem(const QString &name = 0,
								const QString &url = 0);
	void deleteItem();

	QString name() const;
	QStringList epg() const;
	QString fileName() const { return _fileName; }
	QList<int> nums() const;
	QTreeWidget *treeWidget();

	int processNum(QTreeWidgetItem *channel,
				   const int &num) const;
	void moveUp(QTreeWidgetItem *channel);
	void moveDown(QTreeWidgetItem *channel);
	void editMode();

	Channel *channelRead(QTreeWidgetItem* clickedChannel);
	Channel *channelRead(const int &clickedChannel);

	bool validate() const;

protected:
	void changeEvent(QEvent *e);

signals:
	void itemClicked(QTreeWidgetItem*,
					 const int);

private slots:
	void processPlaylist();

private:
	Ui::PlaylistWidget *ui;

	PlaylistHandler *_handler;
	QString _fileName;
};

#endif // TANO_PLAYLISTWIDGET_H_
