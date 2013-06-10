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
#include <QUrl>

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

#include <vlc-qt/WidgetVideo.h>
#include <vlc-qt/WidgetVolumeSlider.h>

#include "Config.h"
#include "platform/Features.h"

#include "core/Arguments.h"
#include "core/Common.h"
#include "core/LocaleManager.h"
#include "core/Log.h"
#include "core/Resources.h"
#include "core/network/NetworkDownload.h"
#include "core/network/NetworkHttpAuth.h"
#include "core/network/NetworkUdpxy.h"
#include "core/playlist/PlaylistModel.h"
#include "core/playlist/PlaylistUpdate.h"
#include "core/playlist/containers/Channel.h"
#include "core/settings/Settings.h"
#include "core/settings/SettingsChannel.h"
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
#include "menu/MenuCore.h"
#include "playlist/PlaylistEditor.h"
#include "playlist/PlaylistFilterWidget.h"
#include "settings/SettingsDialog.h"
#include "settings/SettingsDialogShortcuts.h"

#if FEATURE_RECORDER
    #include "core/timers/TimersSql.h"
    #include "core/timers/containers/Timer.h"

    #include "recorder/Recorder.h"
#endif

#if BRANDING
    #include "branding/Branding.h"
#endif

#include "MainWindow.h"
#include "MediaPlayer.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(Arguments *args,
                       const QString &password)
    : QMainWindow(),
      ui(new Ui::MainWindow),
      _init(false),
      _hasPlaylist(false),
      _select(0),
      _file(new NetworkDownload()),
      _locale(new LocaleManager()),
      _model(new PlaylistModel(this)),
      _modelUpdate(new PlaylistUpdate(_model)),
      _channel(0),
      _xmltv(new XmltvManager()),
      _previewTimer(new QTimer(this)),
      _httpAuth(new NetworkHttpAuth(password)),
      _udpxy(new NetworkUdpxy()),
      _schedule(new EpgScheduleFull()),
      _epgShow(new EpgShow()),
      _osdFloat(0),
      _osdInfo(0),
      _osdSchedule(0),
      _osdShow(0),
      _playlistEditor(0)
{
    _arguments = args;

    ui->setupUi(this);

#if FEATURE_UPDATE
    _update = new UpdateDialog(this);
#else
    ui->menuAbout->removeAction(ui->actionUpdate);
#endif

    createDesktopStartup();
    createBackend();
    createMenus();
    createSettings();
    createGui();
    createRecorder();
    createShortcuts();
    createConnections();
    _mediaPlayer->createSession(_hasPlaylist && _model->validate());

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
#if FEATURE_RECORDER
    if (_recorder->isRecording()) {
        ret = QMessageBox::warning(this, Tano::name(),
                      tr("Are you sure you want to exit?\nThis will stop recording in progress."),
                      QMessageBox::Close | QMessageBox::Cancel,
                      QMessageBox::Close);
    } else {
        ret = QMessageBox::Close;
    }
#else
    ret = QMessageBox::Close;
#endif

    switch (ret)
    {
    case QMessageBox::Close:
#if FEATURE_RECORDER
        _recorder->recordStop();
#endif
        if (ui->actionMute->isChecked())
            ui->actionMute->toggle();
        _trayIcon->hide();
        _mediaPlayer->writeSession();
#if PASSWORD
        Tano::Log::logout();
#endif        
	qApp->quit();
        break;
    case QMessageBox::Cancel:
        break;
    default:
        break;
    }
}

void MainWindow::exitLogout()
{
    _httpAuth->logout();
    exit();
}

bool MainWindow::eventFilter(QObject *obj,
                             QEvent *event)
{
    if (obj == this && event->type() == QEvent::Hide) {
        qDebug() << "Event:" << "Hide";

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

        if (_muteOnMinimize) {
            ui->actionMute->setChecked(_muteOnMinimizeCurrent);
        }
    } else if (event->type() == QEvent::MouseMove) {
        toggleMouse(true);

        if (ui->actionFullscreen->isChecked()) {
            QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
            showOsd(mouseEvent->globalPos());
        }

        if (obj == _mediaPlayer->video()) {
            _mouseTimer->start(1000);
        }
    } else if (obj == _mediaPlayer->video() && event->type() == QEvent::MouseButtonDblClick) {
        toggleMouse(true);
        qDebug() << "Event:" << "Double click";
        ui->actionFullscreen->trigger();
    } else if (obj == _mediaPlayer->video() && event->type() == QEvent::MouseButtonPress) {
        toggleMouse(true);
        qDebug() << "Event:" << "Click";
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        switch (mouseEvent->button())
        {
        case Qt::RightButton:
            _rightMenu->exec(mouseEvent->globalPos());
            break;
        case Qt::NoButton:
        default:
            break;
        }
    } else if (obj == _mediaPlayer->video() && event->type() == QEvent::Wheel) {
        toggleMouse(true);
        qDebug() << "Event:" << "Wheel";
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        bool wheel = wheelEvent->delta() > 0;
        if (_wheelType == "volume") {
            _mediaPlayer->osd()->volumeSlider()->volumeControl(wheel);
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

// Init functions
void MainWindow::createGui()
{
    _mouseTimer = new QTimer(this);

    _osdInfo = new OsdFloat(this);
    _osdInfo->setInfo();
    _osdFloat = new OsdFloat(this);
    _osdFloat->resize(_osdFloat->width(), _mediaPlayer->osd()->height());
    _osdFloat->setControls();
    _osdSchedule = new OsdFloat(this);
    _osdSchedule->setSchedule();
    _osdShow = new OsdFloat(this);
    _osdShow->setSchedule();

    ui->dockInfo->setTitleBarWidget(ui->blank);

    _waction = new QWidgetAction(this);
    _playlistMenu->addAction(_waction);
    toggleFilters();

    ui->playlistWidget->playMode();
    ui->playlistWidget->setModel(_model);
    _schedule->setPlaylistModel(_model);

    openPlaylist(true);
    setStopped();
    showVideo();
    ui->toolBarRecorder->hide();
    ui->scheduleWidget->setIdentifier(Tano::Main);

#if !FEATURE_TELETEXT
    ui->menuMedia->removeAction(ui->actionTeletext);
#endif 

    ui->labelPlaylistIcon->setPixmap(QIcon::fromTheme("video-x-generic").pixmap(16));
    ui->labelScheduleIcon->setPixmap(QIcon::fromTheme("x-office-calendar").pixmap(16));

    qDebug() << "Initialised: GUI";
}

void MainWindow::createBackend()
{
    _mediaPlayer = new MediaPlayer(_arguments, this);
    ui->tabs->addStaticWidget(_mediaPlayer);

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

    _wheelType = settings->mouseWheel();
    _rememberGui = settings->rememberGuiSession();
    _filter = settings->filtersVisible();
    if (_init)
        toggleFilters();

    //Playback settings
    _mediaPlayer->createSettings();
    _udpxy->createSettings();
    _muteOnMinimize = settings->muteOnMinimize();

    _init = true;

    qDebug() << "Initialised: Settings";
}

void MainWindow::createDesktopStartup()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _defaultPlaylist = settings->playlist();
    if (!_arguments->value(Argument::Playlist).isEmpty())
        _defaultPlaylist = _arguments->value(Argument::Playlist);

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

#if FEATURE_UPDATE
    _update->checkSilent();
#endif

    if (_rememberGui) {
        resize(_width, _height);
    }

    qDebug() << "Initialised: Startup settings";
}

void MainWindow::createRecorder()
{
#if FEATURE_RECORDER
    _recorder = new Recorder();
    _recorder->setPlaylistModel(_model);
    _recorder->setVisible(false);
    _recorder->setWidgets(ui->actionRecord, _trayIcon);
    _recorder->setMediaInstance(_mediaPlayer->mediaInstance());
    _recorder->createSettings();
    _recorder->refreshPlaylistModel();

    // TODO: Recorder
    //ui->layoutRecorder->addWidget(_recorder);

    _recordNow = false;

    connect(ui->actionRecordQuick, SIGNAL(triggered()), _recorder, SLOT(quickRecord()));
    connect(ui->actionRecordTimer, SIGNAL(triggered()), _recorder, SLOT(newTimer()));
    connect(_recorder, SIGNAL(play(Timer *)), this, SLOT(playRecording(Timer *)));
#else
    ui->menubar->removeAction(ui->menuRecorder->menuAction());
    ui->toolBar->removeAction(ui->actionRecorder);
    removeToolBar(ui->toolBarRecorder);
#endif
}

void MainWindow::createConnections()
{
    connect(ui->actionDonate, SIGNAL(triggered()), this, SLOT(donate()));
    connect(ui->actionSupport, SIGNAL(triggered()), this, SLOT(support()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionLogoutExit, SIGNAL(triggered()), this, SLOT(exitLogout()));
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

    connect(_trayIcon, SIGNAL(restoreClick()), this, SLOT(tray()));
    connect(ui->actionTray, SIGNAL(triggered()), this, SLOT(tray()));

    connect(ui->actionFullscreen, SIGNAL(triggered(bool)), this, SLOT(toggleFullscreen(bool)));

    connect(ui->actionMute, SIGNAL(toggled(bool)), _mediaPlayer->osd(), SLOT(mute(bool)));
    connect(ui->actionVolumeDown, SIGNAL(triggered()), _mediaPlayer->osd(), SLOT(volumeDown()));
    connect(ui->actionVolumeUp, SIGNAL(triggered()), _mediaPlayer->osd(), SLOT(volumeUp()));

#if FEATURE_TELETEXT
    if (_mediaPlayer->teletextEnabled()) {
        connect(ui->actionTeletext, SIGNAL(triggered(bool)), _mediaPlayer->osd(), SLOT(teletext(bool)));
        connect(ui->actionTeletext, SIGNAL(triggered(bool)), _mediaPlayer, SLOT(teletext(bool)));
        connect(_mediaPlayer->osd(), SIGNAL(teletextClicked()), ui->actionTeletext, SLOT(trigger()));
    }
#endif

    connect(_mediaPlayer->osd(), SIGNAL(teletextPage(int)), this, SLOT(teletext(int)));
    connect(_mediaPlayer->osd(), SIGNAL(backClicked()), ui->actionBack, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(muteClicked()), ui->actionMute, SLOT(toggle()));
    connect(_mediaPlayer->osd(), SIGNAL(nextClicked()), ui->actionNext, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(playClicked()), ui->actionPlay, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(recordNowClicked()), ui->actionRecordNow, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(snapshotClicked()), ui->actionSnapshot, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(stopClicked()), ui->actionStop, SLOT(trigger()));

    connect(ui->buttonSchedule, SIGNAL(clicked()), this, SLOT(infoToggleSchedule()));
    connect(ui->buttonScheduleBack, SIGNAL(clicked()), this, SLOT(infoToggleSchedule()));
    connect(ui->buttonPlaylistClose, SIGNAL(clicked()), this, SLOT(infoClose()));
    connect(ui->buttonScheduleClose, SIGNAL(clicked()), this, SLOT(infoClose()));

    connect(_xmltv, SIGNAL(current(QStringList)), _mediaPlayer->osd(), SLOT(setEpg(QStringList)));
    connect(_xmltv, SIGNAL(schedule(XmltvProgrammeModel *, Tano::Id)), ui->scheduleWidget, SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_xmltv, SIGNAL(schedule(XmltvProgrammeModel *, Tano::Id)), _schedule->schedule(), SLOT(setEpg(XmltvProgrammeModel *, Tano::Id)));
    connect(_schedule, SIGNAL(requestEpg(QString, Tano::Id)), _xmltv, SLOT(request(QString, Tano::Id)));
    connect(_schedule, SIGNAL(itemSelected(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(ui->scheduleWidget, SIGNAL(itemSelected(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_xmltv, SIGNAL(programme(XmltvProgramme *)), _epgShow, SLOT(display(XmltvProgramme *)));
    connect(_mediaPlayer->osd(), SIGNAL(openLink(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_epgShow, SIGNAL(requestNext(QString, QString)), _xmltv, SLOT(requestProgrammeNext(QString, QString)));
    connect(_epgShow, SIGNAL(requestPrevious(QString, QString)), _xmltv, SLOT(requestProgrammePrevious(QString, QString)));
    connect(ui->playlistWidget, SIGNAL(scheduleRequested(Channel *)), _schedule, SLOT(openSchedule(Channel *)));

#if FEATURE_UPDATE
    connect(_update, SIGNAL(newUpdate()), this, SLOT(updateAvailable()));
    connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(check()));
#endif

    connect(_file, SIGNAL(file(QFile *)), _mediaPlayer->osd(), SLOT(setLogo(QFile *)));

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

    connect(_epgShow, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(_schedule, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(ui->scheduleWidget, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(_xmltv, SIGNAL(programmeRecord(XmltvProgramme *)), this, SLOT(recordProgramme(XmltvProgramme *)));

    connect(_shortcut, SIGNAL(activated()), this, SLOT(closeOsd()));
    connect(_mouseTimer, SIGNAL(timeout()), this, SLOT(toggleMouse()));

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

    ui->menuAudio->addMenu(_mediaPlayer->menus()[0]);
    foreach(MenuCore *menu, _mediaPlayer->menus())
        ui->menuVideo->addMenu(menu);
    ui->menuVideo->removeAction(_mediaPlayer->menus()[0]->menuAction());

    _playlistMenu = new QMenu();

#if BRANDING
    Tano::Branding::processMenus(this, ui, _rightMenu);
#endif

#if PASSWORD
    ui->menuFile->insertAction(ui->actionExit, ui->actionLogoutExit);
#endif

    qDebug() << "Initialised: Menus";
}

void MainWindow::createShortcuts()
{
    _actions << ui->actionPlay
             << ui->actionStop
             << ui->actionNext
             << ui->actionBack
             << ui->actionFullscreen
             << ui->actionMute
#if FEATURE_TELETEXT
             << ui->actionTeletext
#endif
             << ui->actionVolumeUp
             << ui->actionVolumeDown
#if FEATURE_RECORDER
             << ui->actionRecorder
#endif
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
#if FEATURE_RECORDER
             << ui->actionRecordNow
             << ui->actionSnapshot
#endif
             << ui->actionPreview;

    foreach(MenuCore *menu, _mediaPlayer->menus())
        _actions << menu->actionNext();

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
    QString subject = Tano::name() + " " + tr("Support");
    subject = subject.replace(" ", "%20");
    QDesktopServices::openUrl(QUrl("mailto:" + Tano::email() + "?subject=" + subject));
}


//Media controls
void MainWindow::setPlaying()
{
    // TODO: _osdMain->setPlaying(true);

    ui->actionPlay->setIcon(QIcon::fromTheme("media-playback-pause"));
    ui->actionPlay->setText(tr("Pause"));
    ui->actionPlay->setToolTip(tr("Pause"));
    ui->actionMute->setEnabled(true);
#if FEATURE_TELETEXT
    if (_mediaPlayer->teletextEnabled())
        ui->actionTeletext->setEnabled(true);
#endif
}

void MainWindow::setStopped()
{
    // TODO: _osdMain->setPlaying(false);

    ui->actionPlay->setIcon(QIcon::fromTheme("media-playback-start"));
    ui->actionPlay->setText(tr("Play"));
    ui->actionPlay->setToolTip(tr("Play"));
    ui->actionMute->setEnabled(false);
#if FEATURE_TELETEXT
    ui->actionTeletext->setEnabled(false);
#endif
}

void MainWindow::playChannel(Channel *channel)
{
    _channelPlayback = true;
    _channel = channel;

    QString url = _udpxy->processUrl(_channel->url());
    if (channel->passwordProtected())
        url = _httpAuth->processUrl(url);

    _mediaPlayer->playUrl(url, true);

    Tano::Log::playingChannel(channel->number());

    if (_channel->logo().contains("http")) {
        _file->getFile(_channel->logo());
    } else if (!_channel->logo().isEmpty()) {
        _mediaPlayer->osd()->setLogo(_channel->logo());
    }

    _xmltv->request(_channel->xmltvId(), Tano::Main);
    _mediaPlayer->osd()->setChannel(_channel->number(), _channel->name(), _channel->language());
    tooltip(_channel->name());
    _trayIcon->changeToolTip(Tano::Main, _channel->name());
}

void MainWindow::playRecording(Timer *recording)
{
#if FEATURE_RECORDER
    recorder(false);

    _mediaPlayer->playLocal(recording->file());

    _mediaPlayer->osd()->setRecording(recording->name(), recording->display().replace(recording->name() + " - ",""));
    tooltip(recording->name());
    _trayIcon->changeToolTip(Tano::Main, recording->name());

    delete recording;
#else
    Q_UNUSED(recording);
#endif
}

void MainWindow::stop()
{
    _channelPlayback = false;

    if (_recordNow)
        recordNow(false);

    _mediaPlayer->stop();
    _xmltv->stop();
    ui->scheduleWidget->setPage(0);

    tooltip();
    _trayIcon->changeToolTip(Tano::Main);

    Tano::Log::stopped();
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
        if (_playlistName.isEmpty())
            return;

        QFile f(_playlistName);
        if (!f.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, Tano::name(),
                                tr("Cannot read file %1:\n%2.")
                                .arg(_playlistName)
                                .arg(f.errorString()));
            return;
        }
        f.close();

        _model->open(_playlistName);
    } else {
        _playlistName = Tano::Resources::resource(_defaultPlaylist);
        _modelUpdate->update(_defaultPlaylist);
    }

    _hasPlaylist = true;

    _select = new ChannelSelect(this, _mediaPlayer->osd()->lcd(), _model->numbers());
    connect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
    connect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
    connect(_select, SIGNAL(channelSelect(int)), ui->playlistWidget, SLOT(channelSelected(int)));

    ui->labelPlaylistName->setText("<b>" + _model->name() + "</b>");

    ui->playlistWidget->refreshModel();
    _schedule->refreshPlaylistModel();
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
        _xmltv->requestProgramme(_mediaPlayer->osd()->currentProgramme());
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
    // TODO: Playlist editor
    /*if (_playlistEditor) {
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
    }*/
}

void MainWindow::tooltip(const QString &channelNow)
{
    if (channelNow != "stop")
        setWindowTitle(channelNow + " - " + Tano::name());
    else
        setWindowTitle(Tano::name());
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
    } else {
        _liteMenu = ui->menubar->isVisible();
        _liteToolbar = ui->toolBar->isVisible();

        ui->menubar->setVisible(false);
        ui->toolBar->setVisible(false);
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
    // TODO: Fix fullscreen osd
    /*if (pos.y() > QApplication::desktop()->height()-100) {
        toggleOsdControls(true);
    } else {
        toggleOsdControls(false);
    }

    if (pos.x() > QApplication::desktop()->width()-_osdInfo->width()-50) {
        toggleOsdInfo(true);
    } else {
        toggleOsdInfo(false);
    }*/
}

void MainWindow::showVideo(const int &count)
{
    if (ui->actionRecorder->isChecked())
        return;

    ui->actionFullscreen->setEnabled(count);
    ui->actionRecordNow->setEnabled(count);
    ui->actionSnapshot->setEnabled(count);

    // TODO: osd
    /*_osdMain->setVideoState(count);
    _osdMain->setQuickRecordChecked(ui->actionRecordNow->isChecked());*/
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
        //_osdFloat->setWidget(_osdMain);
        //_osdFloat->resize(_osdFloat->width(), _osdMain->height());
        _osdFloat->setControls();
    } else {
        ui->buttonPlaylistClose->show();
        ui->buttonScheduleClose->show();

        ui->dockContents->layout()->addWidget(ui->stackedWidgetDock);
        // TODO: OSD
        //ui->pagePlayback->layout()->addWidget(_osdMain);

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
    }
}

void MainWindow::toggleOsdInfo(const bool &enabled)
{
    if (ui->actionFullscreen->isChecked()) {
        if (enabled) {
            _osdInfo->floatShow();
        } else {
            _osdInfo->floatHide();
        }
    }
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
    //TODO: quick record
/*#if FEATURE_RECORDER
    if (!start) {
        QString media = _mediaItem->currentLocation();
        _recording->setEndTime(QTime::currentTime());
        _recording->setState(Timer::Finished);
        _mediaPlayer->stop();
        _recorder->database()->updateTimer(_recording);
        delete _recording;

        if (_mediaItem)
            delete _mediaItem;
        _mediaItem = new VlcMedia(media, _mediaInstance);
        play();
    } else {
        _recording = _recorder->database()->createTimer(tr("Instant %1").arg(_channel->name()), _channel->name(), _channel->url(), Timer::Instant);
        _recording->setDate(QDate::currentDate());
        _recording->setStartTime(QTime::currentTime());
        _recording->setState(Timer::Recording);
        _mediaPlayer->stop();
        _recording->setFile(_mediaItem->duplicate(Tano::recordingFileName(tr("Instant"), _channel->name(), _recording->date(), _recording->startTime()), _recorder->directory(), Vlc::TS));
        _mediaPlayer->play();
        _recorder->database()->updateTimer(_recording);
        showVideo(1);
    }

    _recordNow = start;
#else
    Q_UNUSED(start)
#endif*/
}

void MainWindow::recorder(const bool &enabled)
{
#if FEATURE_RECORDER
    if (enabled) {
        _recorder->setVisible(true);
        // TODO: recorder
        //ui->stackedWidget->setCurrentIndex(2);

        ui->dockInfo->setVisible(false);
    } else {
        _recorder->setVisible(false);
        ui->dockInfo->setVisible(true);

        /*if (_mediaPlayer->hasVout()) {
            ui->stackedWidget->setCurrentIndex(1);
        } else {
            ui->stackedWidget->setCurrentIndex(0);
        }*/
    }

    ui->actionRecorder->setChecked(enabled);
    ui->toolBarRecorder->setVisible(enabled);
    ui->actionLite->setEnabled(!enabled);
#else
    Q_UNUSED(enabled)
#endif
}

void MainWindow::recordProgramme(XmltvProgramme *programme)
{
#if FEATURE_RECORDER
    recorder(true);

    _recorder->newTimerFromSchedule(programme);
#else
    Q_UNUSED(programme)
#endif
}

void MainWindow::updateAvailable()
{
    ui->actionUpdate->setText(tr("An update is available!"));
    ui->toolBar->insertAction(ui->actionExit, ui->actionUpdate);
}
