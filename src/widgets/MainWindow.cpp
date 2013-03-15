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

#include <QtGui/QDesktopServices>
#include <QtGui/QMouseEvent>

#include <QDebug>

#if defined(Qt5)
    #include <QtWidgets/QDesktopWidget>
    #include <QtWidgets/QLCDNumber>
    #include <QtWidgets/QMessageBox>
    #include <QtWidgets/QShortcut>
    #include <QtWidgets/QWidgetAction>
#elif defined(Qt4)
    #include <QtGui/QDesktopWidget>
    #include <QtGui/QLCDNumber>
    #include <QtGui/QMessageBox>
    #include <QtGui/QShortcut>
    #include <QtGui/QWidgetAction>
#endif

#include <vlc-qt/Common.h>
#include <vlc-qt/Config.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/Video.h>

#include <vlc-qt/ControlAudio.h>
#include <vlc-qt/ControlVideo.h>
#include <vlc-qt/WidgetVolumeSlider.h>

#include "Config.h"

#include "core/Arguments.h"
#include "core/Common.h"
#include "core/LocaleManager.h"
#include "core/Resources.h"
#include "core/network/NetworkDownload.h"
#include "core/network/NetworkUdpxy.h"
#include "core/playlist/PlaylistModel.h"
#include "core/playlist/PlaylistUpdate.h"
#include "core/playlist/containers/Channel.h"
#include "core/settings/Settings.h"
#include "core/settings/SettingsChannel.h"
#include "core/timers/containers/Timer.h"
#include "core/xmltv/XmltvManager.h"

#include "common/Backend.h"
#include "common/ChannelSelect.h"
#include "common/DesktopShortcuts.h"
#include "common/FileDialogs.h"
#include "common/OsdFloat.h"
#include "common/OsdWidget.h"
#include "common/TrayIcon.h"
#include "dialogs/AboutDialog.h"
#include "dialogs/DonationDialog.h"
#include "dialogs/UpdateDialog.h"
#include "epg/EpgScheduleFull.h"
#include "epg/EpgShow.h"
#include "menu/MenuAspectRatio.h"
#include "menu/MenuCropRatio.h"
#include "menu/MenuDeinterlacing.h"
#include "menu/MenuScale.h"
#include "menu/MenuTrackAudio.h"
#include "menu/MenuTrackSubtitles.h"
#include "menu/MenuTrackVideo.h"
#include "playlist/PlaylistEditor.h"
#include "playlist/PlaylistFilterWidget.h"
#include "settings/SettingsDialog.h"
#include "settings/SettingsDialogShortcuts.h"

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(Arguments *args)
    : QMainWindow(),
      ui(new Ui::MainWindow),
      _init(false),
      _hasPlaylist(false),
      _select(0),
      _file(new NetworkDownload()),
      _locale(new LocaleManager()),
      _model(new PlaylistModel(this)),
      _modelUpdate(new PlaylistUpdate(_model)),
      _settingsChannel(new SettingsChannel(this)),
      _audioController(0),
      _videoController(0),
      _channel(0),
      _xmltv(new XmltvManager()),
      _previewTimer(new QTimer(this)),
      _startTimer(new QTimer(this)),
      _udpxy(new NetworkUdpxy()),
      _schedule(new EpgScheduleFull()),
      _epgShow(new EpgShow()),
      _osdFloat(0),
      _osdInfo(0),
      _osdSchedule(0),
      _osdShow(0),
      _osdMain(0),
      _playlistEditor(0)
{
    _arguments = args;

    ui->setupUi(this);

#if UPDATE
    _update = new UpdateDialog(this);
#else
    ui->menuAbout->removeAction(ui->actionUpdate);
#endif

    // Mouse double click hack
#if defined(Qt5)
    _double = false;
    _timerDouble = new QTimer();
#endif

    createMenus();
    createDesktopStartup();
    createSettings();
    createBackend();
    createGui();
    createShortcuts();
    createConnections();
    createSession();

    qApp->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    if (_playlistEditor)
        delete _playlistEditor;

    delete ui;
}

void MainWindow::exit()
{
    int ret;
    if (ui->recorder->isRecording()) {
        ret = QMessageBox::warning(this, tr("Tano"),
                      tr("Do you want to exit Tano?\nThis will stop recording in progress."),
                      QMessageBox::Close | QMessageBox::Cancel,
                      QMessageBox::Close);
    } else {
        ret = QMessageBox::Close;
    }

    switch (ret)
    {
    case QMessageBox::Close:
        ui->recorder->recordStop();
        if (ui->actionMute->isChecked())
            ui->actionMute->toggle();
        _trayIcon->hide();
        writeSession();
        qApp->quit();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

bool MainWindow::eventFilter(QObject *obj,
                             QEvent *event)
{
    if (obj == this && event->type() == QEvent::Hide) {
        qDebug() << "Event:" << "Hide";
        _dockControlsVisible = ui->dockControls->isVisible();
        _dockInfoVisible = ui->dockInfo->isVisible();

        if (_muteOnMinimize) {
            _muteOnMinimizeCurrent = ui->actionMute->isChecked();
            ui->actionMute->setChecked(true);
        }
    } else if (obj == this && event->type() == QEvent::Show) {
        qDebug() << "Event:" << "Show";
        if (_rememberGui && _posX && _posY) {
            move(_posX, _posY);
            _posX = 0;
            _posY = 0;
        }

        ui->dockControls->setVisible(_dockControlsVisible);
        ui->dockInfo->setVisible(_dockInfoVisible);

        if (_muteOnMinimize) {
            ui->actionMute->setChecked(_muteOnMinimizeCurrent);
        }
    } else if (event->type() == QEvent::MouseMove) {
        toggleMouse(true);

        if (ui->actionFullscreen->isChecked()) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            showOsd(mouseEvent->globalPos());
        }

        if (obj == ui->videoWidget) {
            _mouseTimer->start(1000);
        }
    } else if (obj == ui->videoWidget && event->type() == QEvent::MouseButtonDblClick) {
        toggleMouse(true);
        qDebug() << "Event:" << "Double click";
        ui->actionFullscreen->trigger();
    } else if (obj == ui->videoWidget && event->type() == QEvent::MouseButtonPress) {
        toggleMouse(true);
        qDebug() << "Event:" << "Click";
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        switch (mouseEvent->button())
        {
        case Qt::RightButton:
            _rightMenu->exec(mouseEvent->globalPos());
            break;
        case Qt::LeftButton:
#if defined(Qt5)
            if (_double) {
                qDebug() << "Event:" << "Double click";
                ui->actionFullscreen->trigger();
            } else {
                _double = true;
                _timerDouble->start(500);
            }
#endif
        case Qt::NoButton:
        default:
            break;
        }
    } else if (obj == ui->videoWidget && event->type() == QEvent::Wheel) {
        toggleMouse(true);
        qDebug() << "Event:" << "Wheel";
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        bool wheel = wheelEvent->delta() > 0;
        if (_wheelType == "volume") {
            _osdMain->volumeSlider()->volumeControl(wheel);
        } else {
            _select->channel(wheel);
        }
    }

    return false;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (_hideToTray) {
        qDebug() << "Event:" << "Close";
        tray();
        event->ignore();
    }
}

// Mouse double click hack
void MainWindow::resetClick()
{
    _double = false;
}

// Init functions
void MainWindow::createGui()
{
    _mouseTimer = new QTimer(this);

    _osdMain = new OsdWidget(this);
    _osdMain->setBackend(_mediaPlayer);
    _osdMain->toggleTeletext(_teletext);
    if (!_teletext) ui->menuMedia->removeAction(ui->actionTeletext);
    _osdInfo = new OsdFloat(this);
    _osdInfo->setInfo();
    _osdFloat = new OsdFloat(this);
    _osdFloat->resize(_osdFloat->width(), _osdMain->height());
    _osdFloat->setControls();
    _osdSchedule = new OsdFloat(this);
    _osdSchedule->setSchedule();
    _osdShow = new OsdFloat(this);
    _osdShow->setSchedule();

    ui->dockInfo->setTitleBarWidget(ui->blank);
    ui->dockControlsContents->layout()->addWidget(_osdMain);
    ui->dockControls->setTitleBarWidget(_osdMain->blank());

    _waction = new QWidgetAction(this);
    _playlistMenu->addAction(_waction);
    toggleFilters();

    ui->playlistWidget->playMode();
    ui->playlistWidget->setModel(_model);
    _schedule->setPlaylistModel(_model);
    ui->recorder->setPlaylistModel(_model);
    ui->recorder->setVisible(false);
    ui->recorder->setWidgets(ui->actionRecord, _trayIcon);

    openPlaylist(true);
    setStopped();
    showVideo();
    ui->pageMain->setStyleSheet("background-color: rgb(0,0,0);");
    ui->toolBarRecorder->hide();
    ui->scheduleWidget->setIdentifier(Tano::Main);

#if !TELETEXT
    _osdMain->toggleTeletext(false);
    ui->menuMedia->removeAction(ui->actionTeletext);
#endif 

    ui->labelPlaylistIcon->setPixmap(QIcon::fromTheme("video-x-generic").pixmap(16));
    ui->labelScheduleIcon->setPixmap(QIcon::fromTheme("x-office-calendar").pixmap(16));

    qDebug() << "Initialised: GUI";
}

void MainWindow::createBackend()
{
    if (!_arguments->value(Argument::Xmltv).isEmpty())
        _xmltv->loadXmltv(_arguments->value(Argument::Xmltv));
    else
        _xmltv->loadXmltv();

    _mediaInstance = new VlcInstance(Tano::Backend::args(_arguments->value(Argument::Aout), _arguments->value(Argument::Vout)), this);
    _mediaItem = 0;
    _mediaPlayer = new VlcMediaPlayer(_mediaInstance);
    _mediaPlayer->setVideoWidget(ui->videoWidget);

    _audioController = new VlcControlAudio(_mediaPlayer, _defaultAudioLanguage, this);
    _videoController = new VlcControlVideo(_mediaPlayer, _defaultSubtitleLanguage, this);

    ui->videoWidget->setMediaPlayer(_mediaPlayer);
    ui->videoWidget->initDefaultSettings();
    ui->recorder->setMediaInstance(_mediaInstance);
    _recordNow = false;

    qDebug() << "Initialised: Backend";
}

void MainWindow::createSettings()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _hideToTray = settings->trayEnabled() ? settings->hideToTray() : false;

    //GUI Settings
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));
    ui->toolBarRecorder->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));

    if (settings->trayEnabled())
        _trayIcon->show();
    else
        _trayIcon->hide();

    _osdEnabled = settings->osd();
    _infoEnabled = settings->info();
    _wheelType = settings->mouseWheel();
    _rememberGui = settings->rememberGuiSession();
    _filter = settings->filtersVisible();
    if (_init)
        toggleFilters();

    //Playback settings
    _defaultAspectRatio = settings->aspectRatio();
    _defaultCropRatio = settings->cropRatio();
    _defaultDeinterlacing = settings->deinterlacing();
    ui->videoWidget->setDefaultAspectRatio(Vlc::Ratio(_defaultAspectRatio));
    ui->videoWidget->setDefaultCropRatio(Vlc::Ratio(_defaultCropRatio));
    ui->videoWidget->setDefaultDeinterlacing(Vlc::Deinterlacing(_defaultDeinterlacing));
    _menuAspectRatio->setDefault(Vlc::Ratio(_defaultAspectRatio));
    _menuCropRatio->setDefault(Vlc::Ratio(_defaultCropRatio));
    _menuDeinterlacing->setDefault(Vlc::Deinterlacing(_defaultDeinterlacing));
    _menuScale->setDefault(Vlc::Scale(0));

    _defaultAudioLanguage = settings->audioLanguage();
    _defaultSubtitleLanguage = settings->subtitleLanguage();
    _videoSettings = settings->rememberVideoSettings();
    _videoSettingsChannel = settings->rememberVideoPerChannel();
    if (_audioController)
        _audioController->setDefaultAudioLanguage(_defaultAudioLanguage);
    if (_videoController)
        _videoController->setDefaultSubtitleLanguage(_defaultSubtitleLanguage);
    _udpxy->createSettings();
    _muteOnMinimize = settings->muteOnMinimize();

    _settingsChannel->setDefaults(_defaultAspectRatio, _defaultCropRatio, _defaultDeinterlacing, _defaultAudioLanguage, _defaultSubtitleLanguage);

    _sessionVolumeEnabled = settings->sessionVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();

    ui->recorder->createSettings();
    _defaultSnapshot = settings->snapshotsDirectory();

    _init = true;

    qDebug() << "Initialised: Settings";
}

void MainWindow::createDesktopStartup()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _defaultPlaylist = settings->playlist();
    if (!_arguments->value(Argument::Playlist).isEmpty())
        _defaultPlaylist = _arguments->value(Argument::Playlist);

    //Session
    _sessionVolumeEnabled = settings->sessionRememberVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();
    _sessionVolume = settings->sessionVolume();
    _sessionChannel = settings->sessionChannel();

    _teletext = settings->teletext();

    _width = settings->width();
    _height = settings->height();
    _posX = settings->posX();
    _posY = settings->posY();

    // GUI
    if (settings->startLite()) {
        ui->actionLite->setChecked(true);
        lite();
    } else
        _isLite = false;

    if (settings->startOnTop()) {
        ui->actionTop->setChecked(true);;
        top();
    }

    ui->dockControls->setVisible(settings->startControls());
    ui->dockInfo->setVisible(settings->startInfo());

    _dockControlsVisible = settings->startControls();
    _dockInfoVisible = settings->startInfo();

    qDebug() << "Initialised: Startup settings";
}

void MainWindow::createConnections()
{
    connect(ui->actionDonate, SIGNAL(triggered()), this, SLOT(donate()));
    connect(ui->actionSupport, SIGNAL(triggered()), this, SLOT(support()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    connect(ui->actionTop, SIGNAL(triggered()), this, SLOT(top()));
    connect(ui->actionLite, SIGNAL(triggered()), this, SLOT(lite()));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
    connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));
    connect(ui->actionOpenToolbar, SIGNAL(triggered()), this, SLOT(showOpenMenu()));

    connect(ui->actionSchedule, SIGNAL(triggered()), this, SLOT(showSchedule()));
    connect(ui->actionScheduleCurrent, SIGNAL(triggered()), this, SLOT(showScheduleCurrent()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(ui->actionSettingsShortcuts, SIGNAL(triggered()), this, SLOT(showSettingsShortcuts()));
    connect(ui->actionEditPlaylist, SIGNAL(triggered()), this, SLOT(showPlaylistEditor()));

    connect(ui->actionPlay, SIGNAL(triggered()), _mediaPlayer, SLOT(togglePause()));
    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stop()));
    connect(ui->actionPreview, SIGNAL(triggered(bool)), this, SLOT(preview(bool)));

    connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playChannel(Channel *)));
    connect(_previewTimer, SIGNAL(timeout()), ui->actionNext, SLOT(trigger()));
    connect(_startTimer, SIGNAL(timeout()), this, SLOT(startSession()));

    connect(_trayIcon, SIGNAL(restoreClick()), this, SLOT(tray()));
    connect(ui->actionTray, SIGNAL(triggered()), this, SLOT(tray()));

    connect(ui->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(toggleFullscreen(bool)));

    connect(ui->actionMute, SIGNAL(toggled(bool)), _osdMain, SLOT(mute(bool)));
    connect(ui->actionVolumeDown, SIGNAL(triggered()), _osdMain, SLOT(volumeDown()));
    connect(ui->actionVolumeUp, SIGNAL(triggered()), _osdMain, SLOT(volumeUp()));

#if TELETEXT
    if (_teletext) {
        connect(ui->actionTeletext, SIGNAL(triggered(bool)), _osdMain, SLOT(teletext(bool)));
        connect(ui->actionTeletext, SIGNAL(triggered(bool)), this, SLOT(teletext(bool)));
        connect(_osdMain, SIGNAL(teletextClicked()), ui->actionTeletext, SLOT(trigger()));
    }
#endif

    connect(_osdMain, SIGNAL(teletextPage(int)), this, SLOT(teletext(int)));
    connect(_osdMain, SIGNAL(backClicked()), ui->actionBack, SLOT(trigger()));
    connect(_osdMain, SIGNAL(muteClicked()), ui->actionMute, SLOT(toggle()));
    connect(_osdMain, SIGNAL(nextClicked()), ui->actionNext, SLOT(trigger()));
    connect(_osdMain, SIGNAL(playClicked()), ui->actionPlay, SLOT(trigger()));
    connect(_osdMain, SIGNAL(recordNowClicked()), ui->actionRecordNow, SLOT(trigger()));
    connect(_osdMain, SIGNAL(snapshotClicked()), ui->actionSnapshot, SLOT(trigger()));
    connect(_osdMain, SIGNAL(stopClicked()), ui->actionStop, SLOT(trigger()));

    connect(ui->actionControls, SIGNAL(triggered(bool)), this, SLOT(toggleOsdControls(bool)));
    connect(ui->dockControls, SIGNAL(visibilityChanged(bool)), this, SLOT(toggleOsdControls(bool)));
    connect(ui->actionInfoPanel, SIGNAL(triggered(bool)), this, SLOT(toggleOsdInfo(bool)));
    connect(ui->dockInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(toggleOsdInfo(bool)));

    connect(ui->buttonSchedule, SIGNAL(clicked()), this, SLOT(infoToggleSchedule()));
    connect(ui->buttonScheduleBack, SIGNAL(clicked()), this, SLOT(infoToggleSchedule()));
    connect(ui->buttonPlaylistClose, SIGNAL(clicked()), this, SLOT(infoClose()));
    connect(ui->buttonScheduleClose, SIGNAL(clicked()), this, SLOT(infoClose()));

    connect(_xmltv, SIGNAL(current(QString, QString)), _osdMain, SLOT(setEpg(QString, QString)));
    connect(_xmltv, SIGNAL(schedule(XmltvProgrammeModel *, Tano::Id)), ui->scheduleWidget, SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_xmltv, SIGNAL(schedule(XmltvProgrammeModel *, Tano::Id)), _schedule->schedule(), SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_schedule, SIGNAL(requestEpg(QString, Tano::Id)), _xmltv, SLOT(request(QString, Tano::Id)));
    connect(_schedule, SIGNAL(itemSelected(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(ui->scheduleWidget, SIGNAL(itemSelected(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_xmltv, SIGNAL(programme(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(_osdMain, SIGNAL(openLink(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_epgShow, SIGNAL(requestNext(QString)), _xmltv, SLOT(requestProgrammeNext(QString)));
    connect(_epgShow, SIGNAL(requestPrevious(QString)), _xmltv, SLOT(requestProgrammePrevious(QString)));
    connect(ui->playlistWidget, SIGNAL(scheduleRequested(Channel *)), _schedule, SLOT(openSchedule(Channel *)));

#if UPDATE
    connect(_update, SIGNAL(newUpdate()), this, SLOT(updateAvailable()));
    connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(check()));
#endif

    connect(_file, SIGNAL(file(QFile *)), _osdMain, SLOT(setLogo(QFile *)));

    connect(_audioController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackAudio, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackSubtitles, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackVideo, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_menuTrackSubtitles, SIGNAL(subtitles(QString)), _videoController, SLOT(loadSubtitle(QString)));
    connect(_mediaPlayer, SIGNAL(playing()), this, SLOT(setPlaying()));
    connect(_mediaPlayer, SIGNAL(paused()), this, SLOT(setStopped()));
    connect(_mediaPlayer, SIGNAL(stopped()), this, SLOT(setStopped()));
    connect(_mediaPlayer, SIGNAL(end()), this, SLOT(setStopped()));
    connect(_mediaPlayer, SIGNAL(error()), this, SLOT(setStopped()));
    connect(_mediaPlayer, SIGNAL(vout(int)), this, SLOT(showVideo(int)));
    connect(_mediaPlayer, SIGNAL(stopped()), this, SLOT(showVideo()));

    connect(ui->actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
    connect(ui->actionRecordNow, SIGNAL(toggled(bool)), this, SLOT(recordNow(bool)));
    connect(ui->actionSnapshot, SIGNAL(triggered()), this, SLOT(takeSnapshot()));
    connect(ui->actionRecordQuick, SIGNAL(triggered()), ui->recorder, SLOT(quickRecord()));
    connect(ui->actionRecordTimer, SIGNAL(triggered()), ui->recorder, SLOT(newTimer()));
    connect(ui->recorder, SIGNAL(play(Timer *)), this, SLOT(playRecording(Timer *)));

    connect(_epgShow, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(_schedule, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(ui->scheduleWidget, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(_xmltv, SIGNAL(programmeRecord(XmltvProgramme *)), this, SLOT(recordProgramme(XmltvProgramme *)));

    connect(_menuAspectRatio, SIGNAL(value(int)), this, SLOT(saveChannelSetting(int)));
    connect(_menuCropRatio, SIGNAL(value(int)), this, SLOT(saveChannelSetting(int)));
    connect(_menuDeinterlacing, SIGNAL(value(int)), this, SLOT(saveChannelSetting(int)));

    connect(_shortcut, SIGNAL(activated()), this, SLOT(closeOsd()));
    connect(_mouseTimer, SIGNAL(timeout()), this, SLOT(toggleMouse()));

    // Mouse double click hack
#if defined(Qt5)
    connect(_timerDouble, SIGNAL(timeout()), this, SLOT(resetClick()));
#endif

    qDebug() << "Initialised: Event connections";
}

void MainWindow::createMenus()
{
    _rightMenu = new QMenu();
    _rightMenu->addAction(ui->actionPlay);
    _rightMenu->addAction(ui->actionStop);
    _rightMenu->addAction(ui->actionBack);
    _rightMenu->addAction(ui->actionNext);
    _rightMenu->addSeparator();
    _rightMenu->addAction(ui->actionTop);
    _rightMenu->addAction(ui->actionLite);
    _rightMenu->addAction(ui->actionFullscreen);
    _rightMenu->addSeparator();
    _rightMenu->addAction(ui->actionInfoPanel);
    _rightMenu->addAction(ui->actionControls);
    _rightMenu->addSeparator();
    _rightMenu->addMenu(ui->menuAudio);
    _rightMenu->addMenu(ui->menuVideo);
    _rightMenu->addSeparator();
    _rightMenu->addAction(ui->actionTray);
    _rightMenu->addAction(ui->actionExit);

    _openMenu = new QMenu();
    _openMenu->addAction(ui->actionOpenFile);
    _openMenu->addAction(ui->actionOpenUrl);
    _openMenu->addAction(ui->actionOpen);

    ui->actionOpenToolbar->setMenu(_openMenu);

    _trayIcon = new TrayIcon(_rightMenu);

    _menuTrackAudio = new MenuTrackAudio(ui->menuAudio);
    ui->menuAudio->addMenu(_menuTrackAudio);
    _menuTrackVideo = new MenuTrackVideo(ui->menuVideo);
    ui->menuVideo->addMenu(_menuTrackVideo);
    _menuTrackSubtitles = new MenuTrackSubtitles(ui->menuVideo);
    ui->menuVideo->addMenu(_menuTrackSubtitles);
    _menuAspectRatio = new MenuAspectRatio(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuAspectRatio);
    _menuCropRatio = new MenuCropRatio(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuCropRatio);
    _menuScale = new MenuScale(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuScale);
    _menuDeinterlacing = new MenuDeinterlacing(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuDeinterlacing);

    _playlistMenu = new QMenu();

    qDebug() << "Initialised: Menus";
}

void MainWindow::createShortcuts()
{
    _actions << ui->actionPlay
             << ui->actionStop
             << ui->actionNext
             << ui->actionBack
             << ui->actionFullscreen
             << ui->actionInfoPanel
             << ui->actionControls
             << ui->actionMute
#if TELETEXT
             << ui->actionTeletext
#endif
             << ui->actionVolumeUp
             << ui->actionVolumeDown
             << ui->actionRecorder
             << ui->actionSchedule
             << ui->actionScheduleCurrent
             << ui->actionOpenFile
             << ui->actionOpenUrl
             << ui->actionOpen
             << ui->actionSettings
             << ui->actionSettingsShortcuts
             << ui->actionTop
             << ui->actionLite
             << ui->actionTray
             << ui->actionRecordNow
             << ui->actionSnapshot
             << ui->actionPreview
             << _menuTrackAudio->actionNext()
             << _menuTrackVideo->actionNext()
             << _menuTrackSubtitles->actionNext()
             << _menuAspectRatio->actionNext()
             << _menuCropRatio->actionNext()
             << _menuScale->actionNext()
             << _menuDeinterlacing->actionNext();

    _actionsFull << ui->actionRecorder
                 << ui->actionOpenFile
                 << ui->actionOpenUrl
                 << ui->actionOpen
                 << ui->actionSettings
                 << ui->actionSettingsShortcuts
                 << ui->actionTop
                 << ui->actionLite
                 << ui->actionTray;

    addActions(_actions);

    _shortcut = new QShortcut(QKeySequence("Esc"), this, 0, 0, Qt::ApplicationShortcut);
    _shortcuts = new DesktopShortcuts(_actions, this);

    qDebug() << "Initialised: Shortcuts";
}

void MainWindow::createSession()
{
    _osdMain->volumeSlider()->setVolume(_sessionVolume);

    if ((_sessionAutoplayEnabled ||
         !_arguments->value(Argument::Channel).isEmpty() ||
         !_arguments->value(Argument::File).isEmpty() ||
         !_arguments->value(Argument::Url).isEmpty()) &&
         _hasPlaylist && _model->validate())
        _startTimer->start(100);

#if UPDATE
    _update->checkSilent();
#endif

    if (_rememberGui) {
        resize(_width, _height);
    }

    qDebug() << "Initialised: Session";
}

void MainWindow::startSession()
{
    if (!isVisible())
        return;

    if (!_arguments->value(Argument::File).isEmpty())
        playLocal(_arguments->value(Argument::File));
    else if (!_arguments->value(Argument::File).isEmpty())
        playUrl(_arguments->value(Argument::Url));
    else if (!_arguments->value(Argument::Channel).isEmpty())
        ui->playlistWidget->channelSelected(_arguments->value(Argument::Channel).toInt());
    else
        ui->playlistWidget->channelSelected(_sessionChannel);

    _startTimer->stop();
}

void MainWindow::writeSession()
{
    QScopedPointer<Settings> settings(new Settings(this));

    if (_sessionVolumeEnabled)
        settings->setSessionVolume(_osdMain->volumeSlider()->volume());
    else
        settings->setSessionVolume(Settings::DEFAULT_SESSION_VOLUME);

    if (_sessionAutoplayEnabled)
        settings->setSessionChannel(_osdMain->lcd()->value());

    if (_rememberGui) {
        settings->setWidth(size().width());
        settings->setHeight(size().height());
        settings->setPosX(x());
        settings->setPosY(y());
        if (!isVisible() || _isLite || ui->actionRecorder->isChecked()) {
            settings->setStartControls(_dockControlsVisible);
            settings->setStartInfo(_dockInfoVisible);
        } else {
            settings->setStartControls(ui->dockControls->isVisible());
            settings->setStartInfo(ui->dockInfo->isVisible());
        }
    }

    settings->writeSettings();

    qDebug() << "Session written";
}

void MainWindow::aboutTano()
{
    AboutDialog about(this);
    about.exec();
}

void MainWindow::donate()
{
    DonationDialog d(this);
    d.exec();
}

void MainWindow::support()
{
    QString subject = tr("Tano Support");
    subject = subject.replace(" ", "%20");
    QDesktopServices::openUrl(QUrl("mailto:" + Settings::SUPPORT_ADDRESS + "?subject=" + subject));
}


//Media controls
void MainWindow::setPlaying()
{
    _osdMain->setPlaying(true);

    ui->actionPlay->setIcon(QIcon::fromTheme("media-playback-pause"));
    ui->actionPlay->setText(tr("Pause"));
    ui->actionPlay->setToolTip(tr("Pause"));
    ui->actionMute->setEnabled(true);
#if TELETEXT
    if (_teletext)
        ui->actionTeletext->setEnabled(true);
#endif
}

void MainWindow::setStopped()
{
    _osdMain->setPlaying(false);

    ui->actionPlay->setIcon(QIcon::fromTheme("media-playback-start"));
    ui->actionPlay->setText(tr("Play"));
    ui->actionPlay->setToolTip(tr("Play"));
    ui->actionMute->setEnabled(false);
#if TELETEXT
    ui->actionTeletext->setEnabled(false);
#endif
}

void MainWindow::play()
{
    _mediaPlayer->open(_mediaItem);

    if (_channelPlayback && _videoSettingsChannel) {
        ui->videoWidget->setCurrentAspectRatio(Vlc::Ratio(_settingsChannel->aspectRatio(_channel->url())));
        ui->videoWidget->setCurrentCropRatio(Vlc::Ratio(_settingsChannel->cropRatio(_channel->url())));
        ui->videoWidget->setCurrentDeinterlacing(Vlc::Deinterlacing(_settingsChannel->deinterlacing(_channel->url())));

        _menuAspectRatio->setDefault(Vlc::Ratio(_settingsChannel->aspectRatio(_channel->url())));
        _menuCropRatio->setDefault(Vlc::Ratio(_settingsChannel->cropRatio(_channel->url())));
        _menuDeinterlacing->setDefault(Vlc::Deinterlacing(_settingsChannel->deinterlacing(_channel->url())));
        _menuScale->setDefault(Vlc::Scale(0));
    }

    if (_videoSettingsChannel)
        ui->videoWidget->enablePreviousSettings();
    else if (_videoSettings)
        ui->videoWidget->enablePreviousSettings();
    else
        ui->videoWidget->enableDefaultSettings();
}

void MainWindow::playChannel(Channel *channel)
{
    _channelPlayback = true;
    _channel = channel;

    playUrl(_udpxy->processUrl(_channel->url()), true);

    if (_channel->logo().contains("http")) {
        _file->getFile(_channel->logo());
    } else if (!_channel->logo().isEmpty()) {
        _osdMain->setLogo(_channel->logo());
    }

    _xmltv->request(_channel->xmltvId(), Tano::Main);
    _osdMain->setChannel(_channel->number(), _channel->name(), _channel->language());
    tooltip(_channel->name());
    _trayIcon->changeToolTip(Tano::Main, _channel->name());
}

void MainWindow::playLocal(const QString &path)
{
    if (path.isEmpty())
        return;

    stop();

    if (_mediaItem)
        delete _mediaItem;
    _mediaItem = new VlcMedia(path, true, _mediaInstance);

    QString t = path;
#if defined(Q_OS_WIN32)
    t.replace("/", "\\");
#endif
    tooltip(t);

    play();
}

void MainWindow::playRecording(Timer *recording)
{
    recorder(false);

    playLocal(recording->file());

    _osdMain->setRecording(recording->name(), recording->display().replace(recording->name() + " - ",""));
    tooltip(recording->name());
    _trayIcon->changeToolTip(Tano::Main, recording->name());
}

void MainWindow::playUrl(const QString &url,
                         const bool &channel)
{
    if (url.isEmpty())
        return;

    stop();

    _channelPlayback = channel;

    if (_mediaItem)
        delete _mediaItem;
    _mediaItem = new VlcMedia(url, _mediaInstance);
    tooltip(url);

    play();
}

void MainWindow::stop()
{
    _channelPlayback = false;

    if (_recordNow)
        recordNow(false);

    _mediaPlayer->stop();

    if (!_videoSettings || _videoSettingsChannel) {
        _menuAspectRatio->setDefault(Vlc::Ratio(_defaultAspectRatio));
        _menuCropRatio->setDefault(Vlc::Ratio(_defaultCropRatio));
        _menuDeinterlacing->setDefault(Vlc::Deinterlacing(_defaultDeinterlacing));
        _menuScale->setDefault(Vlc::Scale(0));
    }

    _xmltv->stop();

    _osdMain->setChannel();

#if TELETEXT
    if (_teletext)
        ui->actionTeletext->setChecked(false);
#endif
    ui->scheduleWidget->setPage(0);

    tooltip();
    _trayIcon->changeToolTip(Tano::Main);

    _audioController->reset();
    _videoController->reset();
}

void MainWindow::saveChannelSetting(const int &value)
{
    if (_mediaPlayer->state() != Vlc::Playing)
        return;

    MenuCore *menu = qobject_cast<MenuCore *>(sender());
    if (!menu)
        return;

    if (menu == _menuAspectRatio)
        _settingsChannel->setAspectRatio(_channel->url(), value);
    else if (menu == _menuCropRatio)
        _settingsChannel->setCropRatio(_channel->url(), value);
    else if (menu == _menuDeinterlacing)
        _settingsChannel->setDeinterlacing(_channel->url(), value);
}

void MainWindow::saveChannelSetting(const QString &value)
{
    Q_UNUSED(value)
}

// Open dialogs
void MainWindow::openPlaylist(const bool &start)
{
    if (_select != 0) {
        disconnect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
        disconnect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
        disconnect(_select, SIGNAL(channelSelect(int)), ui->playlistWidget, SLOT(channelSelected(int)));
        delete _select;
    }

    if (!start) {
        _playlistName = FileDialogs::openPlaylistSimple();
    } else {
        if (!_defaultPlaylist.isEmpty())
            _playlistName = Tano::Resources::resource(_defaultPlaylist);
    }

    if (_playlistName.isEmpty())
        return;

    QFile f(_playlistName);
    if (!f.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(_playlistName)
                            .arg(f.errorString()));
        return;
    }
    f.close();

    if (start)
        _modelUpdate->update(_playlistName);
    else
        _model->open(_playlistName);

    _hasPlaylist = true;

    _select = new ChannelSelect(this, _osdMain->lcd(), _model->numbers());
    connect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
    connect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
    connect(_select, SIGNAL(channelSelect(int)), ui->playlistWidget, SLOT(channelSelected(int)));

    ui->labelPlaylistName->setText("<b>" + _model->name() + "</b>");

    ui->playlistWidget->refreshModel();
    _schedule->refreshPlaylistModel();
    ui->recorder->refreshPlaylistModel();
}
void MainWindow::openFile()
{
    QString file = FileDialogs::openFile();

    if (file.isEmpty())
        return;

    playLocal(file);
}
void MainWindow::openUrl()
{
    QString url = FileDialogs::openUrl();

    if (url.isEmpty())
        return;

    playUrl(url);
}

//GUI
void MainWindow::showSchedule()
{
    if (ui->actionFullscreen->isChecked() && _osdSchedule->isVisible())
        _osdSchedule->floatHide();
    else if (_channelPlayback)
        _schedule->openSchedule(_channel);
    else
        _schedule->openSchedule();
}

void MainWindow::showScheduleCurrent()
{
    if (ui->actionFullscreen->isChecked() && _osdShow->isVisible())
        _osdShow->floatHide();
    else if (_channelPlayback)
        _xmltv->requestProgramme(_osdMain->currentProgramme());
}

void MainWindow::showSettings()
{
    SettingsDialog s(this);
    s.exec();
    _locale->setLocale();
    createSettings();
}

void MainWindow::showSettingsShortcuts()
{
    SettingsDialogShortcuts s(_shortcuts, this);
    s.exec();
}

void MainWindow::showPlaylistEditor()
{
    if (_playlistEditor) {
        if (_playlistEditor->isVisible()) {
            _playlistEditor->activateWindow();
        } else {
            disconnect(_xmltv, SIGNAL(channelsChanged(QHash<QString, QString>)), _playlistEditor, SLOT(setXmltvMap(QHash<QString, QString>)));
            delete _playlistEditor;
            _playlistEditor = new PlaylistEditor(this);
            _playlistEditor->setMediaInstance(_mediaInstance);
            _playlistEditor->setXmltvMap(_xmltv->channels());
            _playlistEditor->open(_playlistName);
            _playlistEditor->show();
        }
    } else {
        _playlistEditor = new PlaylistEditor(this);
        _playlistEditor->setMediaInstance(_mediaInstance);
        connect(_xmltv, SIGNAL(channelsChanged(QHash<QString, QString>)), _playlistEditor, SLOT(setXmltvMap(QHash<QString, QString>)));
        _playlistEditor->setXmltvMap(_xmltv->channels());
        _playlistEditor->open(_playlistName);
        _playlistEditor->show();
    }
}

void MainWindow::tooltip(const QString &channelNow)
{
    if (channelNow != "stop")
        setWindowTitle(channelNow + " - " + tr("Tano"));
    else
        setWindowTitle(tr("Tano"));
}

void MainWindow::showOpenMenu()
{
    ui->actionOpenToolbar->menu()->exec(QCursor::pos());
}

void MainWindow::top()
{
    Qt::WindowFlags top = _flags;
    top |= Qt::WindowStaysOnTopHint;
    if (ui->actionTop->isChecked())
        setWindowFlags(top);
    else
        setWindowFlags(_flags);

    show();
}

void MainWindow::lite()
{
    if (_isLite) {
        ui->menubar->setVisible(_liteMenu);
        ui->toolBar->setVisible(_liteToolbar);
        ui->dockInfo->setVisible(_dockInfoVisible);
        ui->dockControls->setVisible(_dockControlsVisible);
    } else {
        _liteMenu = ui->menubar->isVisible();
        _liteToolbar = ui->toolBar->isVisible();
        _dockInfoVisible = ui->dockInfo->isVisible();
        _dockControlsVisible = ui->dockControls->isVisible();

        ui->menubar->setVisible(false);
        ui->dockInfo->setVisible(false);
        ui->toolBar->setVisible(false);
        ui->dockControls->setVisible(false);
    }
    _isLite = !_isLite;
}

void MainWindow::tray()
{
    if (!_trayIcon->isVisible())
        return;

    if (isHidden()) {
        ui->actionTray->setText(tr("Hide to tray"));
        show();
    } else {
        ui->actionTray->setText(tr("Restore"));
        hide();
    }
}

void MainWindow::closeOsd()
{
    if (ui->actionFullscreen->isChecked()) {
        if (_osdShow->isVisible())
            _osdShow->floatHide();
        else if (_osdSchedule->isVisible())
            _osdSchedule->floatHide();
    }
}

void MainWindow::showOsd(const QPoint &pos)
{
    if (_osdEnabled && pos.y() > QApplication::desktop()->height()-100) {
        toggleOsdControls(true);
    } else {
        toggleOsdControls(false);
    }

    if (_infoEnabled && pos.x() > QApplication::desktop()->width()-_osdInfo->width()-50) {
        toggleOsdInfo(true);
    } else {
        toggleOsdInfo(false);
    }
}

void MainWindow::showVideo(const int &count)
{
    if (ui->actionRecorder->isChecked())
        return;

    if (count) {
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }

    ui->actionFullscreen->setEnabled(count);
    ui->actionRecordNow->setEnabled(count);
    ui->actionSnapshot->setEnabled(count);

    _osdMain->setVideoState(count);
    _osdMain->setQuickRecordChecked(ui->actionRecordNow->isChecked());
}

void MainWindow::teletext(const bool &enabled)
{
    if (enabled) {
        _mediaPlayer->video()->setTeletextPage(100);
        _osdMain->setTeletextPage(100);
    } else {
        _mediaPlayer->video()->setTeletextPage(99);
    }
}

void MainWindow::teletext(const int &page)
{
    if (page == _mediaPlayer->video()->teletextPage())
        return;

    _mediaPlayer->video()->setTeletextPage(page);
}

// Dock
void MainWindow::infoClose()
{
    ui->dockInfo->close();
}

void MainWindow::infoToggleSchedule()
{
    if (ui->stackedWidgetDock->currentIndex()) {
        ui->stackedWidgetDock->setCurrentIndex(0);
    } else {
        ui->stackedWidgetDock->setCurrentIndex(1);
    }
}

void MainWindow::toggleFilters(const bool &enabled)
{
    if (enabled || _filter) {
        ui->buttonPlaylistSearch->hide();
        ui->playlistWidget->filterReset();
    } else if (!_filter) {
        ui->buttonPlaylistSearch->show();
        ui->playlistWidget->filter()->show();
        _playlistMenu->removeAction(_waction);
        _waction->setDefaultWidget(ui->playlistWidget->filter());
        _playlistMenu->addAction(_waction);
        ui->buttonPlaylistSearch->setMenu(_playlistMenu);
    }
}

void MainWindow::toggleFullscreen(const bool &enabled)
{
    _osdInfo->setVisible(enabled);
    _osdFloat->setVisible(enabled);
    _osdSchedule->setVisible(enabled);
    _osdShow->setVisible(enabled);

    foreach (QAction *action, _actionsFull)
        action->setDisabled(enabled);

    _schedule->setFullscreen(enabled, _osdSchedule);
    _osdSchedule->floatHide();
    _epgShow->setFullscreen(enabled, _osdShow);
    _osdShow->floatHide();

    if (enabled) {
        if (!ui->actionLite->isChecked())
            lite();

        setWindowState(windowState() | Qt::WindowFullScreen);

        ui->buttonPlaylistClose->hide();
        ui->buttonScheduleClose->hide();

        _osdInfo->setWidget(ui->stackedWidgetDock);
        _osdFloat->setWidget(_osdMain);
        _osdFloat->resize(_osdFloat->width(), _osdMain->height());
        _osdFloat->setControls();
    } else {
        ui->buttonPlaylistClose->show();
        ui->buttonScheduleClose->show();

        ui->dockContents->layout()->addWidget(ui->stackedWidgetDock);
        ui->dockControlsContents->layout()->addWidget(_osdMain);

        setWindowState(windowState() & ~Qt::WindowFullScreen);

        if (!ui->actionLite->isChecked())
            lite();
    }

    toggleMouse(!enabled);
    toggleFilters(enabled);
}

void MainWindow::toggleMouse(const bool &enabled)
{
    if (enabled) {
        qApp->restoreOverrideCursor();
        _mouseTimer->stop();
    } else {
        qApp->setOverrideCursor(Qt::BlankCursor);
    }
}

void MainWindow::toggleOsdControls(const bool &enabled)
{
    if (ui->actionFullscreen->isChecked()) {
        if (enabled) {
            _osdFloat->floatShow();
        } else {
            _osdFloat->floatHide();
        }
    } else {
        ui->dockControls->setVisible(enabled);
    }

    ui->actionControls->setChecked(enabled);
}

void MainWindow::toggleOsdInfo(const bool &enabled)
{
    if (ui->actionFullscreen->isChecked()) {
        if (enabled) {
            _osdInfo->floatShow();
        } else {
            _osdInfo->floatHide();
        }
    } else {
        ui->dockInfo->setVisible(enabled);
    }

    ui->actionInfoPanel->setChecked(enabled);
}

void MainWindow::preview(const bool &enabled)
{
    if (enabled) {
        ui->actionNext->trigger();
        _previewTimer->start(8000);
    } else {
        _previewTimer->stop();
    }
}

// Recorder
void MainWindow::recordNow(const bool &start)
{
    if (!start) {
        QString media = _mediaItem->currentLocation();
        _recording->setEndTime(QTime::currentTime());
        _recording->setState(Timer::Finished);
        _mediaPlayer->stop();
        ui->recorder->writeTimers();
        _recording = 0;

        if (_mediaItem)
            delete _mediaItem;
        _mediaItem = new VlcMedia(media, _mediaInstance);
        play();
    } else {
        _recording = ui->recorder->newInstantTimer(_channel->name(), _channel->url());
        _recording->setDate(QDate::currentDate());
        _recording->setStartTime(QTime::currentTime());
        _recording->setState(Timer::Recording);
        _mediaPlayer->stop();
        _recording->setFile(_mediaItem->duplicate(Tano::recordingFileName(tr("Instant"), _channel->name(), _recording->date(), _recording->startTime()), ui->recorder->directory(), Vlc::TS));
        _mediaPlayer->play();
        showVideo(1);
    }

    _recordNow = start;
}

void MainWindow::takeSnapshot()
{
    _mediaPlayer->video()->takeSnapshot(_defaultSnapshot);
}

void MainWindow::recorder(const bool &enabled)
{
    if (enabled) {
        ui->recorder->setVisible(true);
        ui->stackedWidget->setCurrentIndex(2);

        _dockControlsVisible = ui->dockControls->isVisible();
        _dockInfoVisible = ui->dockInfo->isVisible();
        ui->dockInfo->setVisible(false);
        ui->dockControls->setVisible(false);
    } else {
        ui->recorder->setVisible(false);
        ui->dockInfo->setVisible(_dockInfoVisible);
        ui->dockControls->setVisible(_dockControlsVisible);

        if (_mediaPlayer->hasVout()) {
            ui->stackedWidget->setCurrentIndex(1);
        } else {
            ui->stackedWidget->setCurrentIndex(0);
        }
    }

    ui->actionRecorder->setChecked(enabled);
    ui->toolBarRecorder->setVisible(enabled);
    ui->actionLite->setEnabled(!enabled);
}

void MainWindow::recordProgramme(XmltvProgramme *programme)
{
    recorder(true);

    ui->recorder->newTimerFromSchedule(programme);
}

void MainWindow::updateAvailable()
{
    ui->toolBar->insertAction(ui->actionExit, ui->actionUpdate);
}
