/****************************************************************************
* SelectPlaylist.h: Common playlist selector widget
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

#ifndef TANO_SELECTPLAYLIST_H_
#define TANO_SELECTPLAYLIST_H_

#include <QtGui/QWidget>

namespace Ui
{
	class SelectPlaylist;
}

class SelectPlaylist : public QWidget
{
	Q_OBJECT
public:
	SelectPlaylist(QWidget *parent = 0);
	~SelectPlaylist();

	QString playlist() const;
	void setPlaylist(const QString &playlist);

protected:
	void changeEvent(QEvent *e);

private slots:
	void playlistBrowse();
	void playlistReset();

private:
	Ui::SelectPlaylist *ui;
};

#endif // TANO_SELECTPLAYLIST_H_
