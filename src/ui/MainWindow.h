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
#include "epg/EpgManager.h"
#include "ui/core/TrayIcon.h"
#include "ui/dialogs/UpdateDialog.h"
#include "ui/epg/EpgFull.h"
#include "ui/epg/EpgShow.h"
#include "ui/menu/MenuAspectRatio.h"
#include "ui/menu/MenuCrop.h"
#include "ui/menu/MenuDeinterlacing.h"
#include "ui/menu/MenuTrackAudio.h"
#include "ui/menu/MenuTrackSubtitles.h"
#include "ui/menu/MenuTrackVideo.h"
#include "ui/playlist/PlaylistEdit.h"

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

protected:
	void changeEvent(QEvent *e);
	void closeEvent(QCloseEvent *event);
	void hideEvent(QHideEvent *event);
	void showEvent(QShowEvent *event);

private slots:
	void exit();
	void aboutTano();

	void showSchedule();
	void showSettings();
	void showPlaylistEditor();

	void play(const QString &itemFile = 0);
	void stop();
	void setPlayingState(const bool &playing,
						 const bool &buffering = false);

	void playChannel(QTreeWidgetItem* clickedChannel);
	void playChannel(const int &clickedChannel);

	void openPlaylist(const bool &start = false);
	void openFile();
	void openUrl();

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
	QString _defaultAudioLanguage;
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

	//Playback and channels
	Channel *_channel;
	EpgManager *_epg;
	EpgShow *_epgShow;
	QString _playlistName;

	//Menus and actions
	TrayIcon *_trayIcon;
	MenuAspectRatio *_menuAspectRatio;
	MenuCrop *_menuCrop;
	MenuDeinterlacing *_menuDeinterlacing;
	MenuTrackAudio *_menuTrackAudio;
	MenuTrackSubtitles *_menuTrackSubtitles;
	MenuTrackVideo *_menuTrackVideo;
	QMenu *_rightMenu;
	QMenu *_openMenu;
	QList<QAction*> _actions;
};

#endif // TANO_MAINWINDOW_H_
