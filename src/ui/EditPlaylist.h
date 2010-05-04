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
#include <QtGui/QTreeWidgetItem>

#include "core/Print.h"

namespace Ui
{
	class EditPlaylist;
}

class EditPlaylist : public QMainWindow
{
Q_OBJECT
public:
	EditPlaylist(const QString &playlist, QWidget *parent = 0);
	~EditPlaylist();

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *event);

private slots:
	void deleteItem();
	void addItem();

	void save();
	void import();
	void print();

	void exit();

	void editItem(QTreeWidgetItem *item);

	void editChannelNumber();
	void editChannelName(const QString &text);
	void editChannelUrl(const QString &text);
	void editChannelCategories(const QString &text);
	void editChannelLanguage(const QString &text);
	void editChannelEpg(const QString &text);
	void editChannelLogo(const QString &text);

	void moveUp();
	void moveDown();

private:
	void createConnections();
	void createSettings();

	Ui::EditPlaylist *ui;

	bool _closeEnabled;
	QString _playlist;
	QIcon _channelIcon;

	Print *_print;
};

#endif // TANO_EDITPLAYLIST_H_
