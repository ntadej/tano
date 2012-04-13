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

#include <QtGui/QCloseEvent>
#include <QtGui/QHideEvent>
#include <QtGui/QShowEvent>
#include <QtGui/QMainWindow>

#include <vlc-qt/Enums.h>

#include "Config.h"

class VlcAudioControl;
class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;
class VlcVideoControl;

class Channel;
class ChannelSelect;
class EpgScheduleFull;
class EpgShow;
class GetFile;
class LocaleManager;
class MenuAspectRatio;
class MenuCropRatio;
class MenuDeinterlacing;
class MenuTrackAudio;
class MenuTrackSubtitles;
class MenuTrackVideo;
class Shortcuts;
class OsdWidget;
class PlaylistEditor;
class PlaylistModel;
class PlaylistUpdate;
class Timer;
class TrayIcon;
class Udpxy;
class XmltvManager;

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
    explicit MainWindow(QWidget *parent = 0);
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

    void openPlaylist(const bool &start = false);
    void openFile();
    void openUrl();

    void tooltip(const QString &channelNow = "stop");
    void showRightMenu(const QPoint &pos);
    void menuOpen();
    void top();
    void lite();
    void tray();
    void showOsd(const QPoint &pos);
    void teletext(const bool &enabled);
    void teletext(const int &page);

    void recordNow();
    void recorder(const bool &enabled);

signals:
    void setVolume(int);

private:
    Ui::MainWindow *ui;
    Qt::WindowFlags _flags;

    //Initialising functions
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
    bool _controlsVisible;
    int _defaultAspectRatio;
    QString _defaultAudioLanguage;
    int _defaultCropRatio;
    int _defaultDeinterlacing;
    QString _defaultPlaylist;
    QString _defaultSubtitleLanguage;
    int _desktopWidth;
    int _desktopHeight;
    bool _hasPlaylist;
    bool _hideToTray;
    bool _infoWidgetVisible;
    bool _isLite;
    bool _osdEnabled;
    int _sessionChannel;
    bool _sessionVolumeEnabled;
    bool _sessionAutoplayEnabled;
    int _sessionVolume;
    bool _udpxyEnabled;
    bool _videoSettings;
    QString _wheelType;

    //Main
    ChannelSelect *_select;
    GetFile *_file;
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
    Udpxy *_udpxy;

    //GUI
    EpgScheduleFull *_schedule;
    EpgShow *_epgShow;
    OsdWidget *_osdMain;
    OsdWidget *_osdFloat;
    PlaylistEditor *_playlistEditor;

    //Menus and actions
    TrayIcon *_trayIcon;
    MenuAspectRatio *_menuAspectRatio;
    MenuCropRatio *_menuCropRatio;
    MenuDeinterlacing *_menuDeinterlacing;
    MenuTrackAudio *_menuTrackAudio;
    MenuTrackSubtitles *_menuTrackSubtitles;
    MenuTrackVideo *_menuTrackVideo;
    QMenu *_rightMenu;
    QMenu *_openMenu;
    QList<QAction*> _actions;
};

#endif // TANO_MAINWINDOW_H_
