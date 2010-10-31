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

#ifndef TANO_PLAYLISTEDIT_H_
#define TANO_PLAYLISTEDIT_H_

#include <QtCore/QTimer>
#include <QtGui/QMainWindow>
#include <QtGui/QTreeWidgetItem>

#include <vlc-qt/Instance.h>
#include <vlc-qt/MediaPlayer.h>

namespace Ui
{
	class PlaylistEdit;
}

class PlaylistEdit : public QMainWindow
{
Q_OBJECT
public:
	PlaylistEdit(const QString &playlist,
				 const WId &video,
				 QWidget *parent = 0);
	~PlaylistEdit();

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *event);

private slots:
	void deleteItem();
	void addItem();
	void addItem(const QString &name,
				 const QString &url);

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

	void refreshPlaylist(const bool &refresh);
	void checkIp();
	void checkCurrentIp();
	void setState(const bool &playing);

private:
	void createConnections();
	void createSettings();

	QString currentIp();

	Ui::PlaylistEdit *ui;

	bool _closeEnabled;
	QString _playlist;

	// Update playlist
	VlcInstance *_instance;
	VlcMediaPlayer *_player;
	int _refresh;
	int _currentIp[4];
	int _currentPort;
	int _currentTimeout;
	bool _currentIpPlaying;
	QTimer *_timer;
};

#endif // TANO_PLAYLISTEDIT_H_
