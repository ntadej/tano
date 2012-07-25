/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QTimer>
#include <QtGui/QCloseEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QShowEvent>

#if defined(Qt5)
    #include <QtWidgets/QMainWindow>
#elif defined(Qt4)
    #include <QtGui/QMainWindow>
#endif

#include <vlc-qt/Enums.h>

#include "Config.h"

class VlcAudioControl;
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class VlcVideoControl;

class Arguments;
class Channel;
class ChannelSelect;
class EpgScheduleFull;
class EpgShow;
class NetworkDownload;
class LocaleManager;
class MenuAspectRatio;
class MenuCropRatio;
class MenuDeinterlacing;
class MenuScale;
class MenuTrackAudio;
class MenuTrackSubtitles;
class MenuTrackVideo;
class Shortcuts;
class OsdFloat;
class OsdWidget;
class PlaylistEditor;
class PlaylistModel;
class PlaylistUpdate;
class Timer;
class TrayIcon;
class Udpxy;
class XmltvManager;
class XmltvProgramme;

#if UPDATE
class UpdateDialog;
#endif

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWindow(Arguments *args);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    void hideEvent(QHideEvent *event);
    void showEvent(QShowEvent *event);

private slots:
    void exit();
    void aboutTano();
    void donate();
    void support();
    void updateAvailable();

    void showSchedule();
    void showSettings();
    void showSettingsShortcuts();
    void showPlaylistEditor();

    void playChannel(Channel* channel);
    void playLocal(const QString &path);
    void playRecording(Timer* recording);
    void playUrl(const QString &url);
    void stop();
    void setPlayingState(const Vlc::State &state);
    void startSession();

    void openPlaylist(const bool &start = false);
    void openFile();
    void openUrl();

    void tooltip(const QString &channelNow = "stop");
    void showOpenMenu();
    void showRightMenu(const QPoint &pos);
    void top();
    void lite();
    void tray();
    void showOsd(const QPoint &pos);
    void showVideo(const bool &enabled);
    void teletext(const bool &enabled);
    void teletext(const int &page);
    void toggleFullscreen(const bool &enabled);
    void toggleOsdControls();
    void toggleOsdControls(const bool &enabled);
    void toggleOsdInfo();
    void toggleOsdInfo(const bool &enabled);

    void infoClose();
    void infoToggleSchedule();

    void recordNow(const bool &start);
    void recordProgramme(XmltvProgramme *programme);
    void recorder(const bool &enabled);
    void takeSnapshot();

signals:
    void setVolume(int);

private:
    Ui::MainWindow *ui;
    Qt::WindowFlags _flags;

    //Initialising functions
    void createArguments();
    void createBackend();
    void createConnections();
    void createGui();
    void createMenus();
    void createSettings();
    void createSettingsStartup();
    void createSession();
    void createShortcuts();
    void mouseWheel();
    void writeSession();

    // Functions
    void play();

    //Settings
    bool _dockControlsVisible;
    int _defaultAspectRatio;
    QString _defaultAudioLanguage;
    int _defaultCropRatio;
    int _defaultDeinterlacing;
    QString _defaultPlaylist;
    QString _defaultSnapshot;
    QString _defaultSubtitleLanguage;
    int _desktopWidth;
    int _desktopHeight;
    bool _hasPlaylist;
    bool _hideToTray;
    bool _dockInfoVisible;
    bool _isLite;
    bool _osdEnabled;
    bool _infoEnabled;
    bool _muteOnMinimize;
    bool _recordNow;
    bool _rememberSize;
    int _mainWidth;
    int _mainHeight;
    int _sessionChannel;
    bool _sessionVolumeEnabled;
    bool _sessionAutoplayEnabled;
    int _sessionVolume;
    bool _teletext;
    bool _udpxyEnabled;
    bool _videoSettings;
    QString _wheelType;

    //Main
    Arguments *_arguments;
    ChannelSelect *_select;
    NetworkDownload *_file;
    LocaleManager *_locale;
    PlaylistModel *_model;
    PlaylistUpdate *_modelUpdate;
    Shortcuts *_shortcuts;

#if UPDATE
    UpdateDialog *_update;
#endif

    //Backend
    VlcAudioControl *_audioController;
    VlcInstance *_mediaInstance;
    VlcMedia *_mediaItem;
    VlcMediaPlayer *_mediaPlayer;
    VlcVideoControl *_videoController;

    //Playback and channels
    Channel *_channel;
    XmltvManager *_xmltv;
    QString _playlistName;
    Timer *_recording;
    QTimer *_startTimer;
    Udpxy *_udpxy;

    //GUI
    EpgScheduleFull *_schedule;
    EpgShow *_epgShow;
    OsdFloat *_osdFloat;
    OsdFloat *_osdInfo;
    OsdWidget *_osdMain;
    PlaylistEditor *_playlistEditor;

    //Menus and actions
    TrayIcon *_trayIcon;
    MenuAspectRatio *_menuAspectRatio;
    MenuCropRatio *_menuCropRatio;
    MenuDeinterlacing *_menuDeinterlacing;
    MenuScale *_menuScale;
    MenuTrackAudio *_menuTrackAudio;
    MenuTrackSubtitles *_menuTrackSubtitles;
    MenuTrackVideo *_menuTrackVideo;
    QMenu *_rightMenu;
    QMenu *_openMenu;
    QMenu *_playlistMenu;
    QList<QAction*> _actions;
};

#endif // TANO_MAINWINDOW_H_
