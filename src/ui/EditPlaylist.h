/****************************************************************************
* EditPlaylist.h: Playlist editor
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

#ifndef TANO_EDITPLAYLIST_H_
#define TANO_EDITPLAYLIST_H_

#include <QtGui/QMainWindow>

#include <ui_EditPlaylist.h>

class EditPlaylist : public QMainWindow
{
Q_OBJECT
public:
	EditPlaylist(const QString &playlist, QWidget *parent = 0);
	~EditPlaylist();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void deleteItem();
	void addItem();

	void save();
	void import();

	void exit();

	void editItem(QTreeWidgetItem *item);

	void editChannelNumber();
	void editChannelName(const QString &text);
	void editChannelUrl(const QString &text);
	void editChannelCategories(const QString &text);
	void editChannelLanguage(const QString &text);
	void editChannelEpg(const QString &text);

private:
	void createConnections();
	void createSettings();

	Ui::EditPlaylist ui;

	bool _closeEnabled;
	QString _playlist;
	QIcon _channelIcon;
};

#endif // TANO_EDITPLAYLIST_H_
