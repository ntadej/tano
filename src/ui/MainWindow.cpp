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

#if defined(Qt5)
    #include <QtWidgets/QLCDNumber>
    #include <QtWidgets/QMessageBox>
#elif defined(Qt4)
    #include <QtGui/QLCDNumber>
    #include <QtGui/QMessageBox>
#endif

#include <vlc-qt/Common.h>
#include <vlc-qt/Config.h>
#include <vlc-qt/AudioControl.h>
#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>
#include <vlc-qt/Video.h>
#include <vlc-qt/VideoControl.h>
#include <vlc-qt/VolumeSlider.h>

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "container/core/Channel.h"
#include "container/core/Timer.h"
#include "core/ChannelSelect.h"
#include "core/Common.h"
#include "core/GetFile.h"
#include "core/LocaleManager.h"
#include "core/Settings.h"
#include "core/Shortcuts.h"
#include "core/Udpxy.h"
#include "playlist/PlaylistModel.h"
#include "playlist/PlaylistUpdate.h"
#include "ui/core/FileDialogs.h"
#include "ui/core/OsdWidget.h"
#include "ui/core/TrayIcon.h"
#include "ui/dialogs/AboutDialog.h"
#include "ui/dialogs/DonationDialog.h"
#include "ui/epg/EpgScheduleFull.h"
#include "ui/epg/EpgShow.h"
#include "ui/menu/MenuAspectRatio.h"
#include "ui/menu/MenuCropRatio.h"
#include "ui/menu/MenuDeinterlacing.h"
#include "ui/menu/MenuTrackAudio.h"
#include "ui/menu/MenuTrackSubtitles.h"
#include "ui/menu/MenuTrackVideo.h"
#include "ui/playlist/PlaylistEditor.h"
#include "ui/settings/SettingsDialog.h"
#include "ui/settings/SettingsDialogShortcuts.h"
#include "xmltv/XmltvManager.h"

#if UPDATE
    #include "update/UpdateDialog.h"
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow),
      _select(0),
      _file(new GetFile()),
      _locale(new LocaleManager()),
      _model(new PlaylistModel(this)),
      _modelUpdate(new PlaylistUpdate(_model)),
      _audioController(0),
      _videoController(0),
      _xmltv(new XmltvManager()),
      _udpxy(new Udpxy()),
      _schedule(new EpgScheduleFull()),
      _epgShow(new EpgShow()),
      _osdMain(0),
      _osdFloat(0),
      _playlistEditor(0)
{
    ui->setupUi(this);

#if UPDATE
    _update = new UpdateDialog(this);
#else
    ui->menuAbout->removeAction(ui->actionUpdate);
    ui->buttonUpdate->hide();
#endif

    createMenus();
    createSettingsStartup();
    createSettings();
    createBackend();
    createGui();
    createShortcuts();
    createConnections();
    createSession();
}

MainWindow::~MainWindow() { }

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
        tray();
        event->ignore();
    }
}
void MainWindow::hideEvent(QHideEvent *event)
{
    Q_UNUSED(event)

    _controlsVisible = ui->osdWidget->isVisible();
    _infoWidgetVisible = ui->infoWidget->isVisible();
}
void MainWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event)

    ui->osdWidget->setVisible(_controlsVisible);
    ui->infoWidget->setVisible(_infoWidgetVisible);
}

// Init functions
void MainWindow::createGui()
{
    _osdMain = new OsdWidget(this);
    _osdMain->setBackend(_mediaPlayer);
    ui->osdContents->layout()->addWidget(_osdMain);
    ui->osdWidget->setTitleBarWidget(_osdMain->blank());
    _osdFloat = new OsdWidget();
    _osdFloat->resize(_osdFloat->width(), _osdMain->height());
    _osdFloat->enableFloat();
    _osdFloat->setBackend(_mediaPlayer);

    ui->playlistWidget->setModel(_model);
    _schedule->setPlaylistModel(_model);
    ui->recorder->setPlaylistModel(_model);
    ui->recorder->setWidgets(ui->actionRecord, ui->recorderInfo, _trayIcon);

    openPlaylist(true);
    setPlayingState(Vlc::Idle);
    ui->pageMain->setStyleSheet("background-color: rgb(0,0,0);");
    ui->statusBar->addPermanentWidget(ui->timeWidget);
    ui->statusBar->addPermanentWidget(ui->buttonUpdate);
    ui->toolBarRecorder->hide();
    ui->buttonUpdate->hide();
    ui->scheduleWidget->setIdentifier(Tano::Main);
}

void MainWindow::createBackend()
{
    _xmltv->loadXmltv();

    _mediaInstance = new VlcInstance(Tano::vlcQtArgs(), this);
    _mediaItem = 0;
    _mediaPlayer = new VlcMediaPlayer(_mediaInstance);
    _mediaPlayer->setVideoWidget(ui->videoWidget);

    _audioController = new VlcAudioControl(_mediaPlayer, _defaultAudioLanguage, this);
    _videoController = new VlcVideoControl(_mediaPlayer, _defaultSubtitleLanguage, this);

    ui->videoWidget->setMediaPlayer(_mediaPlayer);
    ui->videoWidget->initDefaultSettings();
    ui->recorder->setMediaInstance(_mediaInstance);
    _recordNow = false;
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
    _wheelType = settings->mouseWheel();
    mouseWheel();

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

    _defaultAudioLanguage = settings->audioLanguage();
    _defaultSubtitleLanguage = settings->subtitleLanguage();
    _videoSettings = settings->rememberVideoSettings();
    if (_audioController)
        _audioController->setDefaultAudioLanguage(_defaultAudioLanguage);
    if (_videoController)
        _videoController->setDefaultSubtitleLanguage(_defaultSubtitleLanguage);
    _udpxy->createSettings();

    _sessionVolumeEnabled = settings->sessionVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();

    ui->recorder->createSettings();
}

void MainWindow::createSettingsStartup()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _defaultPlaylist = settings->playlist();

    //Session
    _sessionVolumeEnabled = settings->sessionVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();
    _sessionVolume = settings->volume();
    _sessionChannel = settings->channel();

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

    ui->osdWidget->setVisible(settings->startControls());
    ui->infoWidget->setVisible(settings->startInfo());

    _controlsVisible = settings->startControls();
    _infoWidgetVisible = settings->startInfo();
}

void MainWindow::createConnections()
{
    connect(ui->actionDonate, SIGNAL(triggered()), this, SLOT(donate()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    connect(ui->actionTop, SIGNAL(triggered()), this, SLOT(top()));
    connect(ui->actionLite, SIGNAL(triggered()), this, SLOT(lite()));

    connect(ui->actionOpenToolbar, SIGNAL(triggered()), this, SLOT(menuOpen()));
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
    connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

    connect(ui->actionSchedule, SIGNAL(triggered()), this, SLOT(showSchedule()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(ui->actionSettingsShortcuts, SIGNAL(triggered()), this, SLOT(showSettingsShortcuts()));
    connect(ui->actionEditPlaylist, SIGNAL(triggered()), this, SLOT(showPlaylistEditor()));

    connect(ui->actionPlay, SIGNAL(triggered()), _mediaPlayer, SLOT(pause()));
    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stop()));

    connect(ui->playlistWidget, SIGNAL(itemSelected(Channel *)), this, SLOT(playChannel(Channel *)));

    connect(_trayIcon, SIGNAL(restoreClick()), this, SLOT(tray()));
    connect(ui->actionTray, SIGNAL(triggered()), this, SLOT(tray()));

    connect(ui->videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(showRightMenu(QPoint)));
    connect(ui->videoWidget, SIGNAL(mouseShow(QPoint)), this, SLOT(showOsd(QPoint)));
    connect(ui->videoWidget, SIGNAL(mouseShow(QPoint)), _osdFloat, SLOT(floatShow()));
    connect(ui->videoWidget, SIGNAL(mouseHide()), _osdFloat, SLOT(floatHide()));
    connect(ui->actionFullscreen, SIGNAL(triggered(bool)), _osdFloat, SLOT(setVisible(bool)));

    connect(ui->actionMute, SIGNAL(triggered(bool)), _osdMain, SLOT(mute(bool)));
    connect(ui->actionTeletext, SIGNAL(triggered(bool)), _osdMain, SLOT(teletext(bool)));
    connect(ui->actionVolumeDown, SIGNAL(triggered()), _osdMain, SLOT(volumeDown()));
    connect(ui->actionVolumeUp, SIGNAL(triggered()), _osdMain, SLOT(volumeUp()));

    connect(ui->actionMute, SIGNAL(triggered(bool)), _osdFloat, SLOT(mute(bool)));
    connect(ui->actionTeletext, SIGNAL(triggered(bool)), _osdFloat, SLOT(teletext(bool)));
    connect(ui->actionTeletext, SIGNAL(triggered(bool)), this, SLOT(teletext(bool)));

    connect(_osdMain->volumeSlider(), SIGNAL(newVolume(int)), _osdFloat->volumeSlider(), SLOT(setVolume(int)));
    connect(_osdFloat->volumeSlider(), SIGNAL(newVolume(int)), _osdMain->volumeSlider(), SLOT(setVolume(int)));
    connect(_osdMain, SIGNAL(teletextPage(int)), _osdFloat, SLOT(setTeletextPage(int)));
    connect(_osdFloat, SIGNAL(teletextPage(int)), _osdMain, SLOT(setTeletextPage(int)));
    connect(_osdMain, SIGNAL(teletextPage(int)), this, SLOT(teletext(int)));
    connect(_osdFloat, SIGNAL(teletextPage(int)), this, SLOT(teletext(int)));

    connect(_osdMain, SIGNAL(backClicked()), ui->actionBack, SLOT(trigger()));
    connect(_osdMain, SIGNAL(muteClicked()), ui->actionMute, SLOT(trigger()));
    connect(_osdMain, SIGNAL(nextClicked()), ui->actionNext, SLOT(trigger()));
    connect(_osdMain, SIGNAL(playClicked()), ui->actionPlay, SLOT(trigger()));
    connect(_osdMain, SIGNAL(recordNowClicked(bool)), ui->actionRecordNow, SLOT(setChecked(bool)));
    connect(_osdMain, SIGNAL(stopClicked()), ui->actionStop, SLOT(trigger()));
    connect(_osdMain, SIGNAL(teletextClicked()), ui->actionTeletext, SLOT(trigger()));
    connect(_osdFloat, SIGNAL(backClicked()), ui->actionBack, SLOT(trigger()));
    connect(_osdFloat, SIGNAL(muteClicked()), ui->actionMute, SLOT(trigger()));
    connect(_osdFloat, SIGNAL(nextClicked()), ui->actionNext, SLOT(trigger()));
    connect(_osdFloat, SIGNAL(playClicked()), ui->actionPlay, SLOT(trigger()));
    connect(_osdFloat, SIGNAL(recordNowClicked(bool)), ui->actionRecordNow, SLOT(setChecked(bool)));
    connect(_osdFloat, SIGNAL(stopClicked()), ui->actionStop, SLOT(trigger()));
    connect(_osdFloat, SIGNAL(teletextClicked()), ui->actionTeletext, SLOT(trigger()));

    connect(_xmltv, SIGNAL(current(QString, QString)), _osdMain, SLOT(setEpg(QString, QString)));
    connect(_xmltv, SIGNAL(current(QString, QString)), _osdFloat, SLOT(setEpg(QString, QString)));
    connect(_xmltv, SIGNAL(schedule(XmltvProgrammeModel *, Tano::Id)), ui->scheduleWidget, SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_xmltv, SIGNAL(schedule(XmltvProgrammeModel *, Tano::Id)), _schedule->schedule(), SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_schedule, SIGNAL(requestEpg(QString, Tano::Id)), _xmltv, SLOT(request(QString, Tano::Id)));
    connect(_schedule, SIGNAL(itemSelected(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(_xmltv, SIGNAL(programme(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(ui->scheduleWidget, SIGNAL(itemSelected(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(_osdMain, SIGNAL(openLink(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_osdFloat, SIGNAL(openLink(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_epgShow, SIGNAL(requestNext(XmltvProgramme *)), _xmltv, SLOT(requestProgrammeNext(XmltvProgramme*)));
    connect(_epgShow, SIGNAL(requestPrevious(XmltvProgramme *)), _xmltv, SLOT(requestProgrammePrevious(XmltvProgramme*)));

#if UPDATE
    connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(check()));
    connect(_update, SIGNAL(newUpdate()), ui->buttonUpdate, SLOT(show()));
    connect(ui->buttonUpdate, SIGNAL(clicked()), _update, SLOT(check()));
#endif

    connect(_file, SIGNAL(file(QString)), _osdMain, SLOT(setLogo(QString)));
    connect(_file, SIGNAL(file(QString)), _osdFloat, SLOT(setLogo(QString)));

    connect(_rightMenu, SIGNAL(aboutToHide()), ui->videoWidget, SLOT(enableMouseHide()));
    connect(_rightMenu, SIGNAL(aboutToShow()), ui->videoWidget, SLOT(disableMouseHide()));

    connect(_audioController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackAudio, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackSubtitles, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackVideo, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_menuTrackSubtitles, SIGNAL(subtitles(QString)), _videoController, SLOT(loadSubtitle(QString)));
    connect(_mediaPlayer, SIGNAL(currentState(Vlc::State)), this, SLOT(setPlayingState(Vlc::State)));
    connect(_mediaPlayer, SIGNAL(hasAudio(bool)), ui->menuAudio, SLOT(setEnabled(bool)));
    connect(_mediaPlayer, SIGNAL(hasVideo(bool)), ui->menuVideo, SLOT(setEnabled(bool)));
    connect(_mediaPlayer, SIGNAL(hasVideo(bool)), this, SLOT(showVideo(bool)));

    connect(ui->actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
    connect(ui->actionRecordNow, SIGNAL(toggled(bool)), this, SLOT(recordNow(bool)));
    connect(ui->actionRecordQuick, SIGNAL(triggered()), ui->recorder, SLOT(quickRecord()));
    connect(ui->actionRecordTimer, SIGNAL(triggered()), ui->recorder, SLOT(newTimer()));
    connect(ui->recorder, SIGNAL(play(Timer *)), this, SLOT(playRecording(Timer *)));
    connect(_epgShow, SIGNAL(requestRecord(XmltvProgramme *)), this, SLOT(recordProgramme(XmltvProgramme *)));
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
    _menuDeinterlacing = new MenuDeinterlacing(ui->videoWidget, ui->menuVideo);
    ui->menuVideo->addMenu(_menuDeinterlacing);
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
             << ui->actionSettingsShortcuts
             << ui->actionTop
             << ui->actionLite
             << ui->actionTray
             << _menuTrackAudio->actionNext()
             << _menuTrackVideo->actionNext()
             << _menuTrackSubtitles->actionNext()
             << _menuAspectRatio->actionNext()
             << _menuCropRatio->actionNext()
             << _menuDeinterlacing->actionNext();

    _shortcuts = new Shortcuts(_actions, this);
}

void MainWindow::createSession()
{
    _osdMain->volumeSlider()->setVolume(_sessionVolume);
    _osdFloat->volumeSlider()->setVolume(_sessionVolume);

    if (_sessionAutoplayEnabled && _hasPlaylist && _model->validate())
        ui->playlistWidget->channelSelected(_sessionChannel);

#if UPDATE
    _update->checkSilent();
#endif
}

void MainWindow::writeSession()
{
    QScopedPointer<Settings> settings(new Settings(this));
    if (_sessionVolumeEnabled)
        settings->setVolume(_osdMain->volumeSlider()->volume());
    else
        settings->setVolume(Settings::DEFAULT_VOLUME);
    if (_sessionAutoplayEnabled)
        settings->setChannel(_osdMain->lcd()->value());
    settings->writeSettings();
}

void MainWindow::mouseWheel()
{
    if (_wheelType == "volume") {
        if (_select)
            disconnect(ui->videoWidget, SIGNAL(wheel(bool)), _select, SLOT(channel(bool)));
        if (_osdMain)
            connect(ui->videoWidget, SIGNAL(wheel(bool)), _osdMain->volumeSlider(), SLOT(volumeControl(bool)));
    } else {
        if (_osdMain)
            disconnect(ui->videoWidget, SIGNAL(wheel(bool)), _osdMain->volumeSlider(), SLOT(volumeControl(bool)));
        if (_select)
            connect(ui->videoWidget, SIGNAL(wheel(bool)), _select, SLOT(channel(bool)));
    }
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


//Media controls
void MainWindow::setPlayingState(const Vlc::State &state)
{
    _osdMain->setPlayingState(state);
    _osdFloat->setPlayingState(state);

    switch (state)
    {
    case Vlc::Playing:
        ui->actionPlay->setIcon(QIcon(":/icons/24x24/media-playback-pause.png"));
        ui->actionPlay->setText(tr("Pause"));
        ui->actionPlay->setToolTip(tr("Pause"));
        ui->actionMute->setEnabled(true);
        ui->actionTeletext->setEnabled(true);
        break;
    default:
        ui->actionPlay->setIcon(QIcon(":/icons/24x24/media-playback-start.png"));
        ui->actionPlay->setText(tr("Play"));
        ui->actionPlay->setToolTip(tr("Play"));
        ui->actionMute->setEnabled(false);
        ui->actionTeletext->setEnabled(false);
    }

    if (state == Vlc::Buffering) {
        ui->statusBar->showMessage(tr("Buffering..."));
    } else {
        ui->statusBar->clearMessage();
    }
}

void MainWindow::play()
{
    _mediaPlayer->open(_mediaItem);

    if (_videoSettings)
        ui->videoWidget->enablePreviousSettings();
    else
        ui->videoWidget->enableDefaultSettings();
}

void MainWindow::playChannel(Channel *channel)
{
    _channel = channel;

    playUrl(_udpxy->processUrl(_channel->url()));

    if (_channel->logo().contains("http")) {
        _file->getFile(_channel->logo());
    } else if (!_channel->logo().isEmpty()) {
        _osdMain->setLogo(_channel->logo());
        _osdFloat->setLogo(_channel->logo());
    }

    _xmltv->request(_channel->xmltvId(), Tano::Main);
    _osdMain->setChannel(_channel->number(), _channel->name(), _channel->language());
    _osdFloat->setChannel(_channel->number(), _channel->name(), _channel->language());
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
    _osdFloat->setRecording(recording->name(), recording->display().replace(recording->name() + " - ",""));
    tooltip(recording->name());
    _trayIcon->changeToolTip(Tano::Main, recording->name());
}

void MainWindow::playUrl(const QString &url)
{
    if (url.isEmpty())
        return;

    stop();

    if (_mediaItem)
        delete _mediaItem;
    _mediaItem = new VlcMedia(url, _mediaInstance);
    tooltip(url);

    _osdMain->setQuickRecordEnabled(true);
    _osdFloat->setQuickRecordEnabled(true);

    play();
}

void MainWindow::stop()
{
    if (_recordNow)
        recordNow(false);

    _mediaPlayer->stop();

    if (!_videoSettings) {
        _menuAspectRatio->setDefault(Vlc::Ratio(_defaultAspectRatio));
        _menuCropRatio->setDefault(Vlc::Ratio(_defaultCropRatio));
        _menuDeinterlacing->setDefault(Vlc::Deinterlacing(_defaultDeinterlacing));
    }

    _xmltv->stop();

    _osdMain->setChannel();
    _osdFloat->setChannel();
    _osdMain->setQuickRecordEnabled(false);
    _osdFloat->setQuickRecordEnabled(false);
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

    if (start)
        _modelUpdate->update(_playlistName);
    else
        _model->open(_playlistName);

    _hasPlaylist = true;

    _select = new ChannelSelect(this, _osdMain->lcd(), _model->numbers());
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
    _schedule->show();
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
            delete _playlistEditor;
            _playlistEditor = new PlaylistEditor(this);
            _playlistEditor->setMediaInstance(_mediaInstance);
            _playlistEditor->open(_playlistName);
            _playlistEditor->show();
        }
    } else {
        _playlistEditor = new PlaylistEditor(this);
        _playlistEditor->setMediaInstance(_mediaInstance);
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
    if (ui->actionTop->isChecked())
        setWindowFlags(top);
    else
        setWindowFlags(_flags);

    show();
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

    if (isHidden()) {
        ui->actionTray->setText(tr("Hide to tray"));
        show();
    } else {
        ui->actionTray->setText(tr("Restore"));
        hide();
    }
}

void MainWindow::showOsd(const QPoint &pos)
{
    if ((pos.x() < _osdFloat->pos().x()+_osdFloat->width()) &&
       (pos.x() > _osdFloat->pos().x()) &&
       (pos.y() < _osdFloat->pos().y()+_osdFloat->height()) &&
       (pos.y() > _osdFloat->pos().y())) {
        ui->videoWidget->disableMouseHide();
    } else {
        ui->videoWidget->enableMouseHide();
    }
}

void MainWindow::showVideo(const bool &enabled)
{
    if (ui->actionRecorder->isChecked())
        return;

    if (enabled) {
        ui->stackedWidget->setCurrentIndex(1);
    } else {
        ui->stackedWidget->setCurrentIndex(0);
    }
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

// Recorder
void MainWindow::recordNow(const bool &start)
{
    _osdMain->setQuickRecordChecked(start);
    _osdFloat->setQuickRecordChecked(start);

    if (!start) {
        QString media = _mediaItem->currentLocation();
        _recording->setEndTime(QTime::currentTime());
        _recording->setState(Tano::Finished);
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
        _recording->setState(Tano::Recording);
        _mediaPlayer->stop();
        _recording->setFile(_mediaItem->duplicate(Tano::recordingFileName(tr("Quick"), _channel->name(), _recording->date(), _recording->startTime()), ui->recorder->directory(), Vlc::TS));
        _mediaPlayer->play();
    }

    _recordNow = start;
}

void MainWindow::recorder(const bool &enabled)
{
    if (enabled) {
        ui->actionRecorder->setChecked(true);
        ui->stackedWidget->setCurrentIndex(2);
        ui->toolBarRecorder->setVisible(true);
        ui->infoContent->setCurrentIndex(1);
        ui->osdWidget->setVisible(false);
    } else {
        ui->actionRecorder->setChecked(false);
        ui->stackedWidget->setCurrentIndex(0);
        ui->toolBarRecorder->setVisible(false);
        ui->infoContent->setCurrentIndex(0);
        ui->osdWidget->setVisible(true);
    }
}

void MainWindow::recordProgramme(XmltvProgramme *programme)
{
    recorder(true);

    ui->recorder->newTimerFromSchedule(programme);
}
