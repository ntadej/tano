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

#ifndef TANO_PLAYLISTIMPORTWEB_H_
#define TANO_PLAYLISTIMPORTWEB_H_

#include <QtCore/QTextCodec>
#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

namespace Ui {
	class PlaylistImportWeb;
}

class GetFile;
class WebPlaylistHandler;

class PlaylistImportWeb : public QDialog
{
Q_OBJECT
public:
	PlaylistImportWeb(QWidget *parent = 0);
	~PlaylistImportWeb();

	void download();
	void save();

	QString playlist() const { return _playlist; }
	bool refresh() const { return _refresh; }

protected:
	void changeEvent(QEvent *e);

private slots:
	void action(QAbstractButton *button);
	void finish(const QString &playlist);
	void getList(const int &id);
	void readList(const QString &list);

private:
	Ui::PlaylistImportWeb *ui;

	QTextCodec *_codec;
	WebPlaylistHandler *_handler;
	GetFile *_file;
	QString _playlist;

	QStringList _playlistName;
	QStringList _playlistUrl;

	bool _refresh;
	bool _save;
};

#endif // TANO_PLAYLISTIMPORTWEB_H_
