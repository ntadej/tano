/****************************************************************************
* MainWindow.h: A MainWindow class for Tano application
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

#ifndef TANO_MAINWINDOW_H_
#define TANO_MAINWINDOW_H_

#include <QtCore/QSettings>
#include <QtGui/QCloseEvent>
#include <QtGui/QMainWindow>

#include <QVlc/AudioControl.h>
#include <QVlc/Instance.h>
#include <QVlc/VideoControl.h>

#include <ui_MainWindow.h>

#include "Updates.h"
#include "channels/Channel.h"
#include "control/ChannelSelect.h"
#include "control/Shortcuts.h"
#include "control/Time.h"
#include "epg/EpgManager.h"
#include "epg/EpgShow.h"
#include "ui/EditPlaylist.h"
#include "ui/EditTimers.h"
#include "ui/TrayIcon.h"

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void exit();
	void aboutTano();
	void aboutPlugins();

	void showSettings();
	void showPlaylistEditor();
	void showTimersEditor();

	void play(const QString &itemFile = 0);
	void stop();
	void playingState(const int &status);

	void playChannel(QTreeWidgetItem* clickedChannel);
	void playChannel(const int &clickedChannel);

	void openPlaylist(const bool &start = false);
	void openFile();
	void openUrl();
	void openSubtitles();

	void showEpg(const QStringList &epgValue, const int &id);
	void processMenu(const QString &type, const QList<QAction*> &list);

	void tooltip(const QString &channelNow = "stop");
	void showRightMenu(const QPoint &pos);
	void menuOpen();
	void top();
	void lite();
	void tray();
	void fullscreen(const bool &on);

	void recordNow();
	void recorder(const bool &enabled);

signals:
	void setVolume(int);

private:
	//Initialising functions
	void createBackend();
	void createConnections();
	void createGui();
	void createMenus();
	void createRecorder();
	void createSettings();
	void createSession();
	void createShortcuts();

	//Settings
	QSettings *_settings;
	bool _controlsVisible;
	QString _defaultPlaylist;
	QString _defaultSubtitleLanguage;
	int _desktopWidth;
	int _desktopHeight;
	bool _hasPlaylist;
	bool _hideToTray;
	bool _infoWidgetVisible;
	bool _isLite;
	bool _osdEnabled;
	bool _recorderEnabled;
	int _sessionChannel;
	bool _sessionEnabled;
	int _sessionVolume;
	bool _updatesOnStart;
	bool _videoSettings;
	QString _wheelType;

	//Main
	ChannelSelect *_select;
	Shortcuts *_shortcuts;
	Time *_time;
	Updates *_update;

	//Backend
	QVlc::AudioControl *_audioController;
	QVlc::Instance *_backend;
	QVlc::VideoControl *_videoController;

	//GUI
	Ui::MainWindow ui;
	Qt::WindowFlags _flags;
	EditPlaylist *_playlistEditor;
	EditTimers *_timersEditor;

	//Playback and channels
	Channel *_channel;
	EpgManager *_epg;
	EpgShow *_epgShow;
	QString _playlistName;

	//Menus and actions
	TrayIcon *_trayIcon;
	QMenu *_rightMenu;
	QMenu *_openMenu;
	QActionGroup *_ratioGroup;
	QActionGroup *_cropGroup;
	QActionGroup *_filterGroup;
	QList<QAction*> _actions;
};

#endif // TANO_MAINWINDOW_H_
