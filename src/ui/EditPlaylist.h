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

#include <QtCore/QTimer>
#include <QtGui/QMainWindow>
#include <QtGui/QTreeWidgetItem>

#include <vlc-qt/Instance.h>
#include <vlc-qt/MediaPlayer.h>

namespace Ui
{
	class EditPlaylist;
}

class EditPlaylist : public QMainWindow
{
Q_OBJECT
public:
	EditPlaylist(const QString &playlist, const WId &video, QWidget *parent = 0);
	~EditPlaylist();

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *event);

private slots:
	void deleteItem();
	void addItem();
	void addItem(const QString &name, const QString &url);

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

	Ui::EditPlaylist *ui;

	bool _closeEnabled;
	QString _playlist;
	QIcon _channelIcon;

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

#endif // TANO_EDITPLAYLIST_H_
