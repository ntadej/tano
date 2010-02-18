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

#include <QVlc/VlcInstance.h>
#include <QVlc/VlcControl.h>

#include <ui_MainWindow.h>

#include "Updates.h"
#include "channels/Channel.h"
#include "control/ChannelSelect.h"
#include "control/Shortcuts.h"
#include "control/Time.h"
#include "epg/Epg.h"
#include "epg/EpgShow.h"
#include "recorder/TimersManager.h"
#include "ui/EditPlaylist.h"
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

	void play(const QString &itemFile = 0);
	void stop();
	void playingState(const int &status);

	void playChannel(QTreeWidgetItem* clickedChannel);
	void playChannel(const int &clickedChannel);

	void openPlaylist(const bool &start = false);
	void openFile();
	void openUrl();

	void showEpg(const int &id, const QStringList &epgValue, const QString &date);
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

	void test();

signals:
	void setVolume(int);

private:
	//Initialising functions
	void createBackend();
	void createCommon();
	void createConnections();
	void createGui();
	void createMenus();
	void createRecorder();
	void createSettings();
	void createSession();
	void createShortcuts();

	//Settings
	QSettings *settings;
	QString _defaultPlaylist;
	QString _defaultSubtitleLanguage;
	int _desktopWidth;
	int _desktopHeight;
	bool _hasPlaylist;
	bool _hideToTray;
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
	ChannelSelect *select;
	Shortcuts *shortcuts;
	Time *time;
	Updates *update;

	//Backend
	VlcInstance *backend;
	VlcControl *controller;

	//GUI
	Ui::MainWindow ui;
	Qt::WindowFlags flags;

	//Playback and channels
	Channel *channel;
	Epg *epg;
	EpgShow *epgShow;
	EditPlaylist *editor;
	QString _fileName;
	QString _playlistName;

	//Menus and actions
	TrayIcon *trayIcon;
	QMenu *rightMenu;
	QMenu *openMenu;
	QActionGroup *ratioGroup;
	QActionGroup *cropGroup;
	QActionGroup *filterGroup;
	QList<QAction*> actions;

	//Recorder
	TimersManager *timers;
};

#endif // TANO_MAINWINDOW_H_
