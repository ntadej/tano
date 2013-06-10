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

#if defined(Qt5)
    #include <QtWidgets/QMainWindow>
#elif defined(Qt4)
    #include <QtGui/QMainWindow>
#endif

#include "WidgetsSharedExport.h"

class QShortcut;
class QWidgetAction;

class Arguments;
class Channel;
class ChannelSelect;
class DesktopShortcuts;
class EpgScheduleFull;
class EpgShow;
class NetworkDownload;
class LocaleManager;
class MediaPlayer;
class NetworkHttpAuth;
class NetworkUdpxy;
class SettingsChannel;
class OsdFloat;
class PlaylistEditor;
class PlaylistModel;
class PlaylistUpdate;
class Recorder;
class Timer;
class TrayIcon;
class UpdateDialog;
class XmltvManager;
class XmltvProgramme;

namespace Ui
{
    class MainWindow;
}

class TANO_WIDGETS_EXPORT MainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWindow(Arguments *args,
                        const QString &password = "");
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj,
                     QEvent *event);

private slots:
    void exit();
    void exitLogout();
    void aboutTano();
    void donate();
    void support();
    void updateAvailable();

    void showSchedule();
    void showScheduleCurrent();
    void showSettings();
    void showSettingsShortcuts();
    void showPlaylistEditor();

    void playChannel(Channel* channel);
    void playRecording(Timer* recording);

    void stop();
    void setPlaying();
    void setStopped();

    void openPlaylist(const bool &start = false);

    void tooltip(const QString &channelNow = "stop");
    void showOpenMenu();
    void top();
    void lite();
    void tray();
    void closeOsd();
    void showOsd(const QPoint &pos);
    void showVideo(const int &count = 0);
    void toggleFilters(const bool &enabled = false);
    void toggleFullscreen(const bool &enabled);
    void toggleMouse(const bool &enabled = false);
    void toggleOsdControls(const bool &enabled = false);
    void toggleOsdInfo(const bool &enabled = false);
    void preview(const bool &enabled = false);

    void infoClose();
    void infoToggleSchedule();

    void recordNow(const bool &start);
    void recordProgramme(XmltvProgramme *programme);
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
    void createDesktopStartup();
    void createRecorder();
    void createShortcuts();

    //Settings
    bool _init;
    bool _channelPlayback;
    QString _defaultPlaylist;
    int _desktopWidth;
    int _desktopHeight;
    bool _filter;
    bool _hasPlaylist;
    bool _hideToTray;
    bool _isLite;
    bool _liteMenu;
    bool _liteToolbar;
    bool _muteOnMinimize;
    bool _muteOnMinimizeCurrent;
    bool _recordNow;
    bool _rememberGui;
    int _width;
    int _height;
    int _posX;
    int _posY;
    bool _udpxyEnabled;
    QString _wheelType;

    //Main
    Arguments *_arguments;
    ChannelSelect *_select;
    NetworkDownload *_file;
    LocaleManager *_locale;
    MediaPlayer *_mediaPlayer;
    PlaylistModel *_model;
    PlaylistUpdate *_modelUpdate;
    Recorder *_recorder;
    QShortcut *_shortcut;
    DesktopShortcuts *_shortcuts;
    UpdateDialog *_update;

    //Playback and channels
    Channel *_channel;
    XmltvManager *_xmltv;
    QString _playlistName;
    QTimer *_previewTimer;
    Timer *_recording;
    NetworkHttpAuth *_httpAuth;
    NetworkUdpxy *_udpxy;

    //GUI
    EpgScheduleFull *_schedule;
    EpgShow *_epgShow;
    OsdFloat *_osdFloat;
    OsdFloat *_osdInfo;
    OsdFloat *_osdSchedule;
    OsdFloat *_osdShow;
    PlaylistEditor *_playlistEditor;
    QTimer *_mouseTimer;

    //Menus and actions
    TrayIcon *_trayIcon;
    QMenu *_rightMenu;
    QMenu *_openMenu;
    QMenu *_playlistMenu;
    QList<QAction*> _actions;
    QList<QAction*> _actionsFull;
    QWidgetAction *_waction;
};

#endif // TANO_MAINWINDOW_H_
