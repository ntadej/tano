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

#include <vlc-qt/Enums.h>

#include "WidgetsSharedExport.h"

class QShortcut;
class QWidgetAction;

class Arguments;
class Channel;
class ChannelSelect;
class DesktopShortcuts;
class NetworkDownload;
class LocaleManager;
class MediaPlayer;
class NetworkUdpxy;
class ScheduleTab;
class SettingsChannel;
class ShowInfoTab;
class OsdFloat;
class PlaylistModel;
class PlaylistTab;
class PlaylistUpdate;
class Recorder;
class Timer;
class TrayIcon;
class UpdateDialog;
class XmltvManager;
class XmltvProgramme;

class TestMain;

namespace Ui
{
    class MainWindow;
}

class TANO_WIDGETS_EXPORT MainWindow : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWindow(Arguments *args);
    ~MainWindow();

public slots:
    void single();
    void dockClicked();

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj,
                     QEvent *event);

private slots:
    void exit();
    void exitLogout();
    void aboutTano();
    void support();

    void currentWidget(QWidget *widget);

    void showSchedule();
    void showScheduleCurrent();
    void showRecorder();
    void showSettings();
    void showSettingsShortcuts();

    void playChannel(Channel* channel);
    void playRecording(Timer* recording);

    void stop();
    void setState(const Vlc::State &state);

    void openPlaylist(bool start = false);
    void openPlaylistComplete();

    void tooltip(const QString &channelNow = "stop");
    void top();
    void lite();
    void tray();
    void showOsd(const QPoint &pos);
    void showVideo(int count = 0);
    void toggleFullscreen(bool enabled);
    void toggleMouse(bool enabled = false);
    void toggleOsdControls(bool enabled = false);
    void preview(bool enabled = false);

    void recordNow(bool start);
    void recordProgramme(XmltvProgramme *programme);

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
    void createShortcuts();

    //Settings
    bool _channelPlayback;
    QString _defaultPlaylist;
    int _desktopWidth;
    int _desktopHeight;
    bool _hasPlaylist;
    bool _hideToTray;
    bool _isLite;
    bool _liteMenu;
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
    LocaleManager *_locale;
    MediaPlayer *_mediaPlayer;
    PlaylistModel *_model;
    PlaylistUpdate *_modelUpdate;
    Recorder *_recorder;
    DesktopShortcuts *_shortcuts;
    UpdateDialog *_update;

    //Playback and channels
    Channel *_channel;
    XmltvManager *_xmltv;
    QString _playlistName;
    QTimer *_previewTimer;
    Timer *_recording;
    NetworkUdpxy *_udpxy;

    //GUI
    PlaylistTab *_playlistTab;
    ScheduleTab *_scheduleTab;
    ShowInfoTab *_showInfoTab;

    OsdFloat *_osdFloat;
    QTimer *_mouseTimer;

    //Menus and actions
    TrayIcon *_trayIcon;
    QMenu *_rightMenu;
    QList<QAction*> _actions;
    QList<QAction*> _actionsFull;
};

#endif // TANO_MAINWINDOW_H_
