/****************************************************************************
* SettingsPlaylist.h: Common playlist selector widget
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

#ifndef TANO_SETTINGSPLAYLIST_H_
#define TANO_SETTINGSPLAYLIST_H_

#include <QtGui/QWidget>

namespace Ui
{
	class SettingsPlaylist;
}

class SettingsPlaylist : public QWidget
{
	Q_OBJECT
public:
	SettingsPlaylist(QWidget *parent = 0);
	~SettingsPlaylist();

	QString playlist() const;
	void setPlaylist(const QString &playlist);

protected:
	void changeEvent(QEvent *e);

private slots:
	void playlistBrowse();
	void playlistReset();

private:
	Ui::SettingsPlaylist *ui;
};

#endif // TANO_SETTINGSPLAYLIST_H_
