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

#include <QtCore/QDebug>
#include <QtGui/QApplication>
#include <QtGui/QBitmap>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMessageBox>
#include <QtGui/QSplashScreen>

#include <vlc-qt/Common.h>
#include <vlc-qt/Config.h>
#include <vlc-qt/AudioControl.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/VideoControl.h>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "container/core/Channel.h"
#include "core/ChannelSelect.h"
#include "core/Common.h"
#include "core/LocaleManager.h"
#include "core/Settings.h"
#include "core/Shortcuts.h"
#include "core/Udpxy.h"
#include "epg/XmltvManager.h"
#include "playlist/PlaylistModel.h"
#include "ui/core/FileDialogs.h"
#include "ui/core/TrayIcon.h"
#include "ui/dialogs/AboutDialog.h"
#include "ui/dialogs/DonationDialog.h"
#include "ui/dialogs/UpdateDialog.h"
#include "ui/epg/EpgScheduleFull.h"
#include "ui/epg/EpgShow.h"
#include "ui/menu/MenuAspectRatio.h"
#include "ui/menu/MenuCrop.h"
#include "ui/menu/MenuDeinterlacing.h"
#include "ui/menu/MenuTrackAudio.h"
#include "ui/menu/MenuTrackSubtitles.h"
#include "ui/menu/MenuTrackVideo.h"
#include "ui/playlist/PlaylistEdit.h"
#include "ui/settings/SettingsEdit.h"

MainWindow::MainWindow(QWidget *parent)    :
    QMainWindow(parent), ui(new Ui::MainWindow), _select(0), _locale(new LocaleManager()), _model(new PlaylistModel(this)), _update(new UpdateDialog()),
    _audioController(0), _mediaInstance(0), _mediaPlayer(0), _videoController(0), _udpxy(new Udpxy()),
    _playlistEditor(0), _xmltv(new XmltvManager()), _epgShow(new EpgShow()), _schedule(new EpgScheduleFull())
{
    QPixmap pixmap(":/images/splash.png");
    Settings *settings = new Settings(this);
    QSplashScreen *splash = new QSplashScreen(pixmap);
    splash->setMask(pixmap.mask());
    if(settings->splash())
        splash->show();
    delete settings;

    ui->setupUi(this);

    createSettingsStartup();
    createSettings();
    createBackend();
    createGui();
    createMenus();
    createShortcuts();
    createSession();
    createConnections();

    splash->close();
    delete splash;
}

MainWindow::~MainWindow() { }

void MainWindow::exit()
{
    int ret;
    if(ui->recorder->isRecording()) {
        ret = QMessageBox::warning(this, tr("Tano"),
                      tr("Do you want to exit Tano?\nThis will stop recording in progress."),
                      QMessageBox::Close | QMessageBox::Cancel,
                      QMessageBox::Close);
    } else {
        ret = QMessageBox::Close;
    }

    switch (ret) {
        case QMessageBox::Close:
            ui->recorder->stop();
            writeSession();
            qApp->quit();
            break;
        case QMessageBox::Cancel:
            break;
        default:
            break;
    }
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
    if(_hideToTray) {
        tray();
        event->ignore();
    }
}
void MainWindow::hideEvent(QHideEvent *event)
{
    _controlsVisible = ui->osdWidget->isVisible();
    _infoWidgetVisible = ui->infoWidget->isVisible();
}
void MainWindow::showEvent(QShowEvent *event)
{
    ui->osdWidget->setVisible(_controlsVisible);
    ui->infoWidget->setVisible(_infoWidgetVisible);
}

// Init functions
void MainWindow::createGui()
{
    ui->playlistWidget->setModel(_model);
    _schedule->setPlaylistModel(_model);
    ui->recorder->setPlaylistModel(_model);
    ui->recorder->setAction(ui->actionRecord);
    ui->recorder->setTrayIcon(_trayIcon);

    openPlaylist(true);
    setPlayingState(false);
    ui->pageMain->setStyleSheet("background-color: rgb(0,0,0);");
    ui->statusBar->addPermanentWidget(ui->timeWidget);
    ui->statusBar->addPermanentWidget(ui->buttonUpdate);
    ui->buttonUpdate->hide();
    ui->scheduleWidget->setIdentifier(Tano::Main);


    _menuTrackAudio = new MenuTrackAudio(ui->menuAudio);
    ui->menuAudio->addMenu(_menuTrackAudio);
    _menuTrackVideo = new MenuTrackVideo(ui->menuVideo);
    ui->menuVideo->addMenu(_menuTrackVideo);
    _menuTrackSubtitles = new MenuTrackSubtitles(ui->menuVideo);
    ui->menuVideo->addMenu(_menuTrackSubtitles);
    _menuAspectRatio = new MenuAspectRatio(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuAspectRatio);
    _menuCrop = new MenuCrop(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuCrop);
    _menuDeinterlacing = new MenuDeinterlacing(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuDeinterlacing);
}

void MainWindow::createBackend()
{
    _mediaInstance = new VlcInstance(Tano::vlcQtArgs(), this);
    _mediaPlayer = new VlcMediaPlayer(ui->videoWidget->widgetId(), this);

    _audioController = new VlcAudioControl(_defaultAudioLanguage);
    _videoController = new VlcVideoControl(_defaultSubtitleLanguage);

    ui->seekWidget->setAutoHide(true);
}

void MainWindow::createSettings()
{
    Settings *settings = new Settings(this);
    _xmltv->setSource(Tano::EpgType(settings->epgType()), settings->location());
    _hideToTray = settings->hideToTray();

    //GUI Settings
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));

    _osdEnabled = settings->osd();
    ui->actionPlaylistFullscreen->setEnabled(_osdEnabled);
    ui->actionPlaylistFullscreen->setChecked(settings->osdPlaylist());
    showPlaylistFullscreen(settings->osdPlaylist());
    _wheelType = settings->mouseWheel();
    mouseWheel();

    //Playback settings
    _defaultAudioLanguage = settings->audioLanguage();
    _defaultSubtitleLanguage = settings->subtitleLanguage();
    _videoSettings = settings->rememberVideoSettings();
    if(_audioController)
        _audioController->setDefaultAudioLanguage(_defaultAudioLanguage);
    if(_videoController)
        _videoController->setDefaultSubtitleLanguage(_defaultSubtitleLanguage);
    _udpxy->createSettings();

    _sessionVolumeEnabled = settings->sessionVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();

    ui->recorder->createSettings();

    delete settings;
}

void MainWindow::createSettingsStartup()
{
    Settings *settings = new Settings(this);
    _desktopWidth = QApplication::desktop()->width();
    _desktopHeight = QApplication::desktop()->height();

    _defaultPlaylist = settings->playlist();

    //Session
    _sessionVolumeEnabled = settings->sessionVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();
    _sessionVolume = settings->volume();
    _sessionChannel = settings->channel();

    // GUI
    if(settings->startLite()) {
        ui->actionLite->setChecked(true);
        lite();
    } else
        _isLite = false;

    if(settings->startOnTop()) {
        ui->actionTop->setChecked(true);;
        top();
    }

    ui->osdWidget->setVisible(settings->startControls());
    ui->infoWidget->setVisible(settings->startInfo());

    _controlsVisible = settings->startControls();
    _infoWidgetVisible = settings->startInfo();

    delete settings;
}

void MainWindow::createConnections()
{
    connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(check()));
    connect(ui->actionDonate, SIGNAL(triggered()), this, SLOT(donate()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    connect(ui->actionTop, SIGNAL(triggered()), this, SLOT(top()));
    connect(ui->actionLite, SIGNAL(triggered()), this, SLOT(lite()));
    connect(ui->actionFullscreen, SIGNAL(toggled(bool)), this, SLOT(fullscreen(bool)));
    connect(ui->actionPlaylistFullscreen, SIGNAL(toggled(bool)), this, SLOT(showPlaylistFullscreen(bool)));

    connect(ui->actionOpenToolbar, SIGNAL(triggered()), this, SLOT(menuOpen()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
    connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

    connect(ui->actionSchedule, SIGNAL(triggered()), this, SLOT(showSchedule()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(ui->actionEditPlaylist, SIGNAL(triggered()), this, SLOT(showPlaylistEditor()));

    connect(ui->actionPlay, SIGNAL(triggered()), _mediaPlayer, SLOT(pause()));
    connect(ui->actionStop, SIGNAL(triggered()), _mediaPlayer, SLOT(stop()));
    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stop()));

    connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playChannel(Channel *)));

    connect(_trayIcon, SIGNAL(restoreClick()), this, SLOT(tray()));
    connect(ui->actionTray, SIGNAL(triggered()), this, SLOT(tray()));

    connect(ui->videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(showRightMenu(QPoint)));
    connect(ui->videoWidget, SIGNAL(mouseShow(QPoint)), this, SLOT(showOsd(QPoint)));
    connect(ui->videoWidget, SIGNAL(mouseShow(QPoint)), ui->osdWidget, SLOT(show()));
    connect(ui->videoWidget, SIGNAL(mouseHide()), ui->osdWidget, SLOT(hide()));
    connect(ui->videoWidget, SIGNAL(mouseShow(QPoint)), ui->infoWidget, SLOT(show()));
    connect(ui->videoWidget, SIGNAL(mouseHide()), ui->infoWidget, SLOT(hide()));

    connect(_xmltv, SIGNAL(epgCurrent(QString, QString)), ui->infoBarWidget, SLOT(setEpg(QString, QString)));
    connect(_xmltv, SIGNAL(epgSchedule(XmltvProgrammeModel *, Tano::Id)), ui->scheduleWidget, SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_xmltv, SIGNAL(epgSchedule(XmltvProgrammeModel *, Tano::Id)), _schedule->schedule(), SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_schedule, SIGNAL(requestEpg(QString, Tano::Id)), _xmltv, SLOT(request(QString, Tano::Id)));
    connect(_schedule, SIGNAL(itemSelected(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(_xmltv, SIGNAL(epgProgramme(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(ui->scheduleWidget, SIGNAL(itemSelected(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(ui->infoBarWidget, SIGNAL(open(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_epgShow, SIGNAL(requestNext(XmltvProgramme *)), _xmltv, SLOT(requestProgrammeNext(XmltvProgramme*)));
    connect(_epgShow, SIGNAL(requestPrevious(XmltvProgramme *)), _xmltv, SLOT(requestProgrammePrevious(XmltvProgramme*)));

    connect(_update, SIGNAL(newUpdate()), ui->buttonUpdate, SLOT(show()));
    connect(ui->buttonUpdate, SIGNAL(clicked()), _update, SLOT(check()));

    connect(_rightMenu, SIGNAL(aboutToHide()), ui->videoWidget, SLOT(enableMouseHide()));
    connect(_rightMenu, SIGNAL(aboutToShow()), ui->videoWidget, SLOT(disableMouseHide()));

    connect(_audioController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackAudio, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackSubtitles, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackVideo, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_menuTrackSubtitles, SIGNAL(subtitles(QString)), _videoController, SLOT(loadSubtitle(QString)));
    connect(_mediaPlayer, SIGNAL(playing(bool, bool)), this, SLOT(setPlayingState(bool, bool)));
    connect(_mediaPlayer, SIGNAL(hasAudio(bool)), ui->menuAudio, SLOT(setEnabled(bool)));
    connect(_mediaPlayer, SIGNAL(hasVideo(bool)), ui->menuVideo, SLOT(setEnabled(bool)));

    connect(ui->actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
    connect(ui->actionRecordNow, SIGNAL(triggered()), this, SLOT(recordNow()));
    connect(ui->actionTimers, SIGNAL(triggered()), ui->recorder, SLOT(showTimersEditor()));
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
    _rightMenu->addMenu(ui->menuAudio);
    _rightMenu->addMenu(ui->menuVideo);
    _rightMenu->addSeparator();
    _rightMenu->addAction(ui->actionTray);
    _rightMenu->addAction(ui->actionExit);

    _openMenu = new QMenu();
    _openMenu->addAction(ui->actionOpenFile);
    _openMenu->addAction(ui->actionOpenUrl);
    _openMenu->addAction(ui->actionOpen);

    _trayIcon = new TrayIcon(_rightMenu);
    _trayIcon->show();
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
             << ui->actionTeletext
             << ui->actionVolumeUp
             << ui->actionVolumeDown
             << ui->actionRecorder
             << ui->actionOpenFile
             << ui->actionOpenUrl
             << ui->actionOpen
             << ui->actionEditPlaylist
             << ui->actionSettings
             << ui->actionTop
             << ui->actionLite
             << ui->actionTray
             << ui->actionPlaylistFullscreen
             << _menuTrackAudio->actionNext()
             << _menuTrackVideo->actionNext()
             << _menuTrackSubtitles->actionNext()
             << _menuAspectRatio->actionNext()
             << _menuCrop->actionNext()
             << _menuDeinterlacing->actionNext();

    _shortcuts = new Shortcuts(_actions, this);
}

void MainWindow::createSession()
{
    ui->volumeSlider->setVolume(_sessionVolume);

    if(_sessionAutoplayEnabled && _hasPlaylist && _model->validate())
        ui->playlistWidget->channelSelected(_sessionChannel);

    _update->checkSilent();
}

void MainWindow::writeSession()
{
    Settings *settings = new Settings(this);
    if(_sessionVolumeEnabled)
        settings->setVolume(ui->volumeSlider->volume());
    else
        settings->setVolume(Settings::DEFAULT_VOLUME);
    if(_sessionAutoplayEnabled)
        settings->setChannel(ui->channelNumber->value());
    settings->writeSettings();
    delete settings;
}

void MainWindow::mouseWheel()
{
    if(_wheelType == "volume") {
        if(_select)
            disconnect(ui->videoWidget, SIGNAL(wheel(bool)), _select, SLOT(channel(bool)));
        connect(ui->videoWidget, SIGNAL(wheel(bool)), ui->volumeSlider, SLOT(volumeControl(bool)));
    } else if(_select) {
        disconnect(ui->videoWidget, SIGNAL(wheel(bool)), ui->volumeSlider, SLOT(volumeControl(bool)));
        connect(ui->videoWidget, SIGNAL(wheel(bool)), _select, SLOT(channel(bool)));
    }
}

void MainWindow::aboutTano()
{
    AboutDialog about(Tano::Player, this);
    about.exec();
}

void MainWindow::donate()
{
    DonationDialog d(this);
    d.exec();
}


//Media controls
void MainWindow::playChannel(Channel *channel)
{
    _channel = channel;
    play();
}

void MainWindow::setPlayingState(const bool &playing,
                                 const bool &buffering)
{
    if(playing) {
        ui->actionPlay->setIcon(QIcon(":/icons/24x24/media-playback-pause.png"));
        ui->buttonPlay->setIcon(QIcon(":/icons/48x48/media-playback-pause.png"));
        ui->actionPlay->setText(tr("Pause"));
        ui->actionPlay->setToolTip(tr("Pause"));
        ui->buttonPlay->setToolTip(tr("Pause"));
        ui->buttonPlay->setStatusTip(tr("Pause"));
        ui->actionMute->setEnabled(true);
        ui->buttonMute->setEnabled(true);
        ui->actionTeletext->setEnabled(true);
        ui->teletextWidget->setEnabled(true);
    } else {
        ui->actionPlay->setIcon(QIcon(":/icons/24x24/media-playback-start.png"));
        ui->buttonPlay->setIcon(QIcon(":/icons/48x48/media-playback-start.png"));
        ui->actionPlay->setText(tr("Play"));
        ui->actionPlay->setToolTip(tr("Play"));
        ui->buttonPlay->setToolTip(tr("Play"));
        ui->buttonPlay->setStatusTip(tr("Play"));
        ui->actionMute->setEnabled(false);
        ui->buttonMute->setEnabled(false);
        ui->actionTeletext->setEnabled(false);
        ui->teletextWidget->setEnabled(false);
    }

    if(buffering) {
        ui->statusBar->showMessage(tr("Buffering..."));
    } else {
        ui->statusBar->clearMessage();
    }
}

void MainWindow::play(const QString &itemFile)
{
    this->stop();

    if(itemFile.isNull()) {
        ui->infoBarWidget->setInfo(_channel->name(), _channel->language());
        //ui->infoBarWidget->setLogo(_channel->logo());

        _xmltv->request(_channel->epg(), Tano::Main);
        ui->channelNumber->display(_channel->number());

        _mediaPlayer->open(_udpxy->processUrl(_channel->url()));
        tooltip(_channel->name());
        _trayIcon->changeToolTip(Tano::Main, _channel->name());
    } else {
        ui->infoWidget->hide();
        _mediaPlayer->open(itemFile);
        tooltip(itemFile);
    }

    if(_videoSettings)
        ui->videoWidget->setPreviousSettings();
}

void MainWindow::stop()
{
    if(!_videoSettings) {
        _menuAspectRatio->original()->trigger();
        _menuCrop->original()->trigger();
    }

    _xmltv->stop();

    ui->infoBarWidget->clear();
    ui->actionTeletext->setChecked(false);

    ui->scheduleWidget->setPage(0);

    tooltip();
    _trayIcon->changeToolTip(Tano::Main);

    _audioController->reset();
    _videoController->reset();
}

// Open dialogs
void MainWindow::openPlaylist(const bool &start)
{
    if(_select != 0) {
        disconnect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
        disconnect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
        disconnect(_select, SIGNAL(channelSelect(int)), ui->playlistWidget, SLOT(channelSelected(int)));
        delete _select;
    }

    if (!start) {
        _playlistName = FileDialogs::openPlaylistSimple();
    } else {
        if(!_defaultPlaylist.isEmpty())
            _playlistName = Tano::locateResource(_defaultPlaylist);
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

    _model->open(_playlistName);

    _hasPlaylist = true;

    _select = new ChannelSelect(this, ui->channelNumber, _model->numbers());
    connect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
    connect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
    connect(_select, SIGNAL(channelSelect(int)), ui->playlistWidget, SLOT(channelSelected(int)));
    mouseWheel();

    ui->channelToolBox->setItemText(0, _model->name());

    ui->playlistWidget->refreshModel();
    _schedule->refreshPlaylistModel();
    ui->recorder->refreshPlaylistModel();
}
void MainWindow::openFile()
{
    QString file = FileDialogs::openFile();

    if (file.isEmpty())
        return;

    play(file);
}
void MainWindow::openUrl()
{
    QString file = FileDialogs::openUrl();

    if (file.isEmpty())
        return;

    play(file);
}

//GUI
void MainWindow::showSchedule()
{
    _schedule->show();
}

void MainWindow::showSettings()
{
    SettingsEdit s(_shortcuts, this);
    s.exec();
    _locale->setLocale();
    createSettings();
}

void MainWindow::showPlaylistEditor()
{
    if(_playlistEditor) {
        if(_playlistEditor->isVisible()) {
            _playlistEditor->activateWindow();
        } else {
            delete _playlistEditor;
            _playlistEditor = new PlaylistEdit(ui->videoWidget->widgetId());
            _playlistEditor->open(_playlistName);
            _playlistEditor->show();
        }
    } else {
        _playlistEditor = new PlaylistEdit(ui->videoWidget->widgetId());
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

void MainWindow::showRightMenu(const QPoint &pos)
{
    _rightMenu->exec(pos);
}

void MainWindow::menuOpen()
{
    _openMenu->exec(QCursor::pos());
}

void MainWindow::top()
{
    Qt::WindowFlags top = _flags;
    top |= Qt::WindowStaysOnTopHint;
    if(ui->actionTop->isChecked())
        this->setWindowFlags(top);
    else
        this->setWindowFlags(_flags);

    this->show();
}

void MainWindow::lite()
{
    ui->infoWidget->setVisible(_isLite);
    ui->toolBar->setVisible(_isLite);
    ui->osdWidget->setVisible(_isLite);
    _isLite = !_isLite;
}

void MainWindow::tray()
{
    if (!_trayIcon->isVisible())
        return;

    if(this->isHidden()) {
        ui->actionTray->setText(tr("Hide to tray"));
        show();
    } else {
        ui->actionTray->setText(tr("Restore"));
        hide();
    }
}

void MainWindow::fullscreen(const bool &on)
{
    if(!_osdEnabled)
        return;

    if(on) {
        ui->osdWidget->resize(2*_desktopWidth/3, ui->osdWidget->height());
        ui->osdWidget->setFloating(true);
        ui->osdWidget->move(_desktopWidth/6, _desktopHeight - ui->osdWidget->height());
        ui->osdWidget->show();
        ui->osdWidget->setWindowFlags(Qt::ToolTip);

        if(_playlistFullscreen) {
            ui->infoWidget->resize(ui->infoWidget->width(), _desktopHeight - 3*ui->osdWidget->height());
            ui->infoWidget->setFloating(true);
            ui->infoWidget->move(_desktopWidth - ui->infoWidget->width(), ui->osdWidget->height());
            ui->infoWidget->show();
            ui->infoWidget->setWindowFlags(Qt::ToolTip);
        }
    } else {
        ui->osdWidget->setFloating(false);
        ui->osdWidget->show();

        ui->infoWidget->setFloating(false);
        ui->infoWidget->show();
    }
}

void MainWindow::showOsd(const QPoint &pos)
{
    if((pos.x() < ui->osdWidget->pos().x()+ui->osdWidget->width()) &&
       (pos.x() > ui->osdWidget->pos().x()) &&
       (pos.y() < ui->osdWidget->pos().y()+ui->osdWidget->height()) &&
       (pos.y() > ui->osdWidget->pos().y())) {
        ui->videoWidget->disableMouseHide();
    } else if(_playlistFullscreen &&
              ((pos.x() < ui->infoWidget->pos().x()+ui->infoWidget->width()) &&
              (pos.x() > ui->infoWidget->pos().x()) &&
              (pos.y() < ui->infoWidget->pos().y()+ui->infoWidget->height()) &&
              (pos.y() > ui->infoWidget->pos().y()))) {
        ui->videoWidget->disableMouseHide();
    } else {
        ui->videoWidget->enableMouseHide();
    }
}

void MainWindow::showPlaylistFullscreen(const bool &on)
{
    _playlistFullscreen = on;

    if(ui->actionFullscreen->isChecked() && on) {
        ui->infoWidget->resize(ui->infoWidget->width(), _desktopHeight - 3*ui->osdWidget->height());
        ui->infoWidget->setFloating(true);
        ui->infoWidget->move(_desktopWidth - ui->infoWidget->width(), ui->osdWidget->height());
        ui->infoWidget->show();
        ui->infoWidget->setWindowFlags(Qt::ToolTip);
    } else {
        ui->infoWidget->setFloating(false);
        //ui->infoWidget->show();
    }
}

// Recorder
void MainWindow::recordNow()
{
    ui->recorder->recordNow(_channel->name(), _channel->url());
}

void MainWindow::recorder(const bool &enabled)
{
    if(enabled) {
        ui->stackedWidget->setCurrentIndex(1);
        ui->infoWidget->setVisible(false);
        ui->osdWidget->setVisible(false);
    } else {
        ui->stackedWidget->setCurrentIndex(0);
        ui->infoWidget->setVisible(true);
        ui->osdWidget->setVisible(true);
    }
}
