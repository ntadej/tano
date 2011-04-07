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

#ifndef TANO_PLAYLISTSELECT_H_
#define TANO_PLAYLISTSELECT_H_

#include <QtCore/QTextCodec>
#include <QtGui/QAbstractButton>
#include <QtGui/QWidget>

namespace Ui {
	class PlaylistSelect;
}

class PlaylistFile;
class PlaylistListHandler;

class PlaylistSelect : public QWidget
{
Q_OBJECT
public:
	PlaylistSelect(QWidget *parent = 0);
	~PlaylistSelect();

	QString playlist() const { return _playlist; }
	bool setPlaylist(const QString &playlist) const;

protected:
	void changeEvent(QEvent *e);

private slots:
	void list(const QString &id);
	void listP(const QString &id);
	void readList(const QString &list);

private:
	Ui::PlaylistSelect *ui;

	QTextCodec *_codec;
	PlaylistListHandler *_handler;
	QString _playlist;

	QStringList _playlistCountries;
	QList<PlaylistFile> _playlistList;
};

#endif // TANO_PLAYLISTSELECT_H_
