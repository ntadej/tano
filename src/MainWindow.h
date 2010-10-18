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

#include <QtGui/QCloseEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMainWindow>

#include <vlc-qt/AudioControl.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/VideoControl.h>

#include "container/Channel.h"
#include "core/ChannelSelect.h"
#include "core/LocaleManager.h"
#include "core/Shortcuts.h"
#include "core/Time.h"
#include "epg/EpgManager.h"
#include "ui/core/TrayIcon.h"
#include "ui/dialogs/UpdateDialog.h"
#include "ui/epg/EpgFull.h"
#include "ui/epg/EpgShow.h"
#include "ui/playlist/PlaylistEdit.h"
#include "ui/recorder/TimersEdit.h"

namespace Ui
{
	class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	static const QString IDENTIFIER;

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *event);
	void hideEvent(QHideEvent *event);
	void showEvent(QShowEvent *event);

private slots:
	void exit();
	void aboutTano();
	void aboutPlugins();

	void showSchedule();
	void showSettings();
	void showPlaylistEditor();
	void showTimersEditor();

	void play(const QString &itemFile = 0);
	void stop();
	void setState(const bool &playing,
				  const bool &audio = false,
				  const bool &video = false);

	void playChannel(QTreeWidgetItem* clickedChannel);
	void playChannel(const int &clickedChannel);

	void openPlaylist(const bool &start = false);
	void openFile();
	void openUrl();
	void openSubtitles();

	void showEpg(const QStringList &epgValue,
				 const int &id,
				 const QString &identifier = IDENTIFIER);
	void processMenu(const QString &type,
					 const QList<QAction*> &list);

	void tooltip(const QString &channelNow = "stop");
	void showRightMenu(const QPoint &pos);
	void menuOpen();
	void top();
	void lite();
	void tray();
	void fullscreen(const bool &on);
	void showOsd(const QPoint &pos);

	void recordNow();
	void recorder(const bool &enabled);

	void nextCrop();
	void nextDeinterlace();
	void nextRatio();

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
	void createSettingsStartup();
	void createSession();
	void createShortcuts();
	void mouseWheel();
	void writeSession();

	//Settings
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
	bool _sessionVolumeEnabled;
	bool _sessionAutoplayEnabled;
	int _sessionVolume;
	bool _videoSettings;
	QString _wheelType;

	//Main
	ChannelSelect *_select;
	LocaleManager *_locale;
	Shortcuts *_shortcuts;
	Time *_time;
	UpdateDialog *_update;

	//Backend
	VlcAudioControl *_audioController;
	VlcInstance *_mediaInstance;
	VlcMediaPlayer *_mediaPlayer;
	VlcVideoControl *_videoController;

	//GUI
	Ui::MainWindow *ui;
	Qt::WindowFlags _flags;
	EpgFull *_schedule;
	PlaylistEdit *_playlistEditor;
	TimersEdit *_timersEditor;

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
