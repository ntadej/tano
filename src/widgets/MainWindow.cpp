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
    #include <QtGui/QStatusBar>
    #include <QtGui/QWidgetAction>
#endif

#include <vlc-qt/WidgetVideo.h>

#include "Config.h"

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
#include "core/platform/Features.h"
#include "core/settings/Settings.h"
#include "core/settings/SettingsChannel.h"
#include "core/xmltv/XmltvManager.h"

#include "common/Backend.h"
#include "common/ChannelSelect.h"
#include "common/DesktopShortcuts.h"
#include "common/FileDialogs.h"
#include "common/OsdFloat.h"
#include "common/OsdWidget.h"
#include "common/PlaylistDisplayWidget.h"
#include "common/TrayIcon.h"
#include "dialogs/AboutDialog.h"
#include "dialogs/DonationDialog.h"
#include "dialogs/UpdateDialog.h"
#include "main/MediaPlayer.h"
#include "main/PlaylistTab.h"
#include "main/ScheduleTab.h"
#include "main/ShowInfoTab.h"
#include "menu/MenuCore.h"
#include "settings/SettingsDialog.h"
#include "settings/SettingsDialogShortcuts.h"
#include "style/VolumeSlider.h"

#if FEATURE_RECORDER
    #include "core/timers/TimersSql.h"
    #include "core/timers/containers/Timer.h"

    #include "recorder/Recorder.h"
#endif

#if BRANDING
    #include "branding/Branding.h"
#endif

#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(Arguments *args,
                       const QString &password)
    : QMainWindow(),
      ui(new Ui::MainWindow),
      _hasPlaylist(false),
      _select(0),
      _locale(new LocaleManager()),
      _model(new PlaylistModel(this)),
      _modelUpdate(new PlaylistUpdate(_model)),
      _channel(0),
      _xmltv(new XmltvManager()),
      _previewTimer(new QTimer(this)),
      _httpAuth(new NetworkHttpAuth(password)),
      _udpxy(new NetworkUdpxy()),
      _osdFloat(0)
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

    return QMainWindow::eventFilter(obj, event);
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

    _playlistTab = new PlaylistTab(this);
    _playlistTab->playlist()->setModel(_model);
    ui->tabs->insertTab(0, _playlistTab, QIcon::fromTheme("video-x-generic"), tr("Channels"));
    ui->tabs->setTabEnabled(0, true);

    _scheduleTab = new ScheduleTab(this);
    _scheduleTab->playlist()->setModel(_model);
    ui->tabs->insertTab(1, _scheduleTab, QIcon::fromTheme("x-office-calendar"), tr("Schedule"));
    ui->tabs->setTabEnabled(1, true);

    _showInfoTab = new ShowInfoTab(this);
    ui->tabs->insertTab(2, _showInfoTab, QIcon::fromTheme("dialog-info"), tr("Show Info"));
    ui->tabs->setTabEnabled(2, true);

    ui->tabs->setCurrentIndex(0);

    //ui->tabs->addCornerWidget(new FancyColorButton(this));

    _osdFloat = new OsdFloat(this);
    _osdFloat->resize(_osdFloat->width(), _mediaPlayer->osd()->height());
    _osdFloat->setControls();

    openPlaylist(true);
    setState(Vlc::Idle);

#if !FEATURE_TELETEXT
    ui->menuMedia->removeAction(ui->actionTeletext);
#endif 

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
    if (settings->trayEnabled())
        _trayIcon->show();
    else
        _trayIcon->hide();

    _wheelType = settings->mouseWheel();
    _rememberGui = settings->rememberGuiSession();

    //Playback settings
    _mediaPlayer->createSettings();
    _udpxy->createSettings();
    _muteOnMinimize = settings->muteOnMinimize();

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
    connect(_playlistTab, SIGNAL(changeTo(QWidget *)), ui->tabs, SLOT(setCurrentWidget(QWidget *)));
    connect(_scheduleTab, SIGNAL(changeTo(QWidget *)), ui->tabs, SLOT(setCurrentWidget(QWidget *)));
    connect(_showInfoTab, SIGNAL(changeTo(QWidget *)), ui->tabs, SLOT(setCurrentWidget(QWidget *)));

    connect(ui->actionDonate, SIGNAL(triggered()), this, SLOT(donate()));
    connect(ui->actionSupport, SIGNAL(triggered()), this, SLOT(support()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
    connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionLogoutExit, SIGNAL(triggered()), this, SLOT(exitLogout()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exit()));

    connect(ui->actionTop, SIGNAL(triggered()), this, SLOT(top()));
    connect(ui->actionLite, SIGNAL(triggered()), this, SLOT(lite()));

    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
    connect(ui->actionOpenFile, SIGNAL(triggered()), _mediaPlayer, SLOT(openFile()));
    connect(ui->actionOpenUrl, SIGNAL(triggered()), _mediaPlayer, SLOT(openUrl()));

    connect(ui->actionSchedule, SIGNAL(triggered()), this, SLOT(showSchedule()));
    connect(ui->actionScheduleCurrent, SIGNAL(triggered()), this, SLOT(showScheduleCurrent()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
    connect(ui->actionSettingsShortcuts, SIGNAL(triggered()), this, SLOT(showSettingsShortcuts()));

    connect(ui->actionPlay, SIGNAL(triggered()), _mediaPlayer, SLOT(togglePause()));
    connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stop()));
    connect(ui->actionPreview, SIGNAL(triggered(bool)), this, SLOT(preview(bool)));

    connect(_playlistTab->playlist(), SIGNAL(itemSelected(Channel *)), this, SLOT(playChannel(Channel *)));
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

    connect(_mediaPlayer->osd(), SIGNAL(backClicked()), ui->actionBack, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(muteClicked()), ui->actionMute, SLOT(toggle()));
    connect(_mediaPlayer->osd(), SIGNAL(nextClicked()), ui->actionNext, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(playClicked()), ui->actionPlay, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(recordNowClicked()), ui->actionRecordNow, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(snapshotClicked()), ui->actionSnapshot, SLOT(trigger()));
    connect(_mediaPlayer->osd(), SIGNAL(stopClicked()), ui->actionStop, SLOT(trigger()));

    connect(_xmltv, SIGNAL(current(QStringList)), _mediaPlayer->osd(), SLOT(setEpg(QStringList)));
    connect(_xmltv, SIGNAL(schedule(QString, XmltvProgrammeModel *)), _scheduleTab, SLOT(setEpg(QString, XmltvProgrammeModel *)));
    connect(_scheduleTab, SIGNAL(requestEpg(QString)), _xmltv, SLOT(request(QString)));
    connect(_scheduleTab, SIGNAL(itemSelected(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_xmltv, SIGNAL(programme(XmltvProgramme *)), _showInfoTab, SLOT(display(XmltvProgramme *)));
    connect(_mediaPlayer->osd(), SIGNAL(openLink(QString)), _xmltv, SLOT(requestProgramme(QString)));
    connect(_showInfoTab, SIGNAL(requestNext(QString, QString)), _xmltv, SLOT(requestProgrammeNext(QString, QString)));
    connect(_showInfoTab, SIGNAL(requestPrevious(QString, QString)), _xmltv, SLOT(requestProgrammePrevious(QString, QString)));
    connect(_playlistTab->playlist(), SIGNAL(scheduleRequested(Channel *)), _scheduleTab, SLOT(channel(Channel *)));

#if FEATURE_UPDATE
    connect(_update, SIGNAL(newUpdate()), this, SLOT(updateAvailable()));
    connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(check()));
#endif

    connect(_mediaPlayer, SIGNAL(stateChanged(Vlc::State)), this, SLOT(setState(Vlc::State)));
    connect(_mediaPlayer, SIGNAL(vout(int)), this, SLOT(showVideo(int)));

    connect(ui->actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
    connect(ui->actionRecordNow, SIGNAL(toggled(bool)), this, SLOT(recordNow(bool)));
    connect(ui->actionSnapshot, SIGNAL(triggered()), _mediaPlayer, SLOT(takeSnapshot()));

    connect(_showInfoTab, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(_scheduleTab, SIGNAL(requestRecord(QString)), _xmltv, SLOT(requestProgrammeRecord(QString)));
    connect(_xmltv, SIGNAL(programmeRecord(XmltvProgramme *)), this, SLOT(recordProgramme(XmltvProgramme *)));

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

    _trayIcon = new TrayIcon(_rightMenu);

    ui->menuAudio->addMenu(_mediaPlayer->menus()[0]);
    foreach(MenuCore *menu, _mediaPlayer->menus())
        ui->menuVideo->addMenu(menu);
    ui->menuVideo->removeAction(_mediaPlayer->menus()[0]->menuAction());

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
void MainWindow::setState(const Vlc::State &state)
{
    switch(state)
    {
    case Vlc::Playing:
        ui->actionPlay->setIcon(QIcon::fromTheme("media-playback-pause"));
        ui->actionPlay->setText(tr("Pause"));
        ui->actionPlay->setToolTip(tr("Pause"));
        ui->actionMute->setEnabled(true);
#if FEATURE_TELETEXT
        if (_mediaPlayer->teletextEnabled())
            ui->actionTeletext->setEnabled(true);
#endif
        break;
    case Vlc::Idle:
    case Vlc::Opening:
    case Vlc::Buffering:
    case Vlc::Paused:
    case Vlc::Stopped:
    case Vlc::Ended:
    case Vlc::Error:
        showVideo();
        ui->actionPlay->setIcon(QIcon::fromTheme("media-playback-start"));
        ui->actionPlay->setText(tr("Play"));
        ui->actionPlay->setToolTip(tr("Play"));
        ui->actionMute->setEnabled(false);
#if FEATURE_TELETEXT
        ui->actionTeletext->setEnabled(false);
#endif
    default:
        break;
    }
}

void MainWindow::showVideo(const int &count)
{
    ui->actionFullscreen->setEnabled(count);
    ui->actionRecordNow->setEnabled(count);
    ui->actionSnapshot->setEnabled(count);

    _mediaPlayer->osd()->setQuickRecordChecked(ui->actionRecordNow->isChecked());
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

    _xmltv->request(_channel->xmltvId(), true);
    _mediaPlayer->osd()->setChannel(_channel->number(), _channel->name(), _channel->language());
    _mediaPlayer->osd()->setLogo(_channel->logo());
    tooltip(_channel->name());
    _trayIcon->changeToolTip(0, _channel->name());
}

void MainWindow::playRecording(Timer *recording)
{
#if FEATURE_RECORDER
    recorder(false);

    _mediaPlayer->playLocal(recording->file());

    _mediaPlayer->osd()->setRecording(recording->name(), recording->display().replace(recording->name() + " - ",""));
    tooltip(recording->name());
    _trayIcon->changeToolTip(0, recording->name());

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

    tooltip();
    _trayIcon->changeToolTip(0);

    Tano::Log::stopped();
}

// Open dialogs
void MainWindow::openPlaylist(const bool &start)
{
    if (_select != 0) {
        disconnect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
        disconnect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
        disconnect(_select, SIGNAL(channelSelect(int)), _playlistTab->playlist(), SLOT(channelSelected(int)));
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
    connect(_select, SIGNAL(channelSelect(int)), _playlistTab->playlist(), SLOT(channelSelected(int)));

    _playlistTab->setPlaylistName(_model->name());
    _playlistTab->setFilters(_model->categories(), _model->languages());
}


//GUI
void MainWindow::showSchedule()
{
    ui->tabs->setCurrentWidget(_scheduleTab);
}

void MainWindow::showScheduleCurrent()
{
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

void MainWindow::tooltip(const QString &channelNow)
{
    if (channelNow != "stop")
        setWindowTitle(channelNow + " - " + Tano::name());
    else
        setWindowTitle(Tano::name());
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
        ui->tabs->setLiteMode(false);
    } else {
        _liteMenu = ui->menubar->isVisible();

        ui->menubar->setVisible(false);
        ui->tabs->setLiteMode(true);
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

void MainWindow::showOsd(const QPoint &pos)
{
    if (pos.y() > QApplication::desktop()->height()-100) {
        toggleOsdControls(true);
    } else {
        toggleOsdControls(false);
    }
}

// Dock
void MainWindow::toggleFullscreen(const bool &enabled)
{
    _osdFloat->setVisible(enabled);

    foreach (QAction *action, _actionsFull)
        action->setDisabled(enabled);

    if (enabled) {
        if (!ui->actionLite->isChecked())
            lite();

        setWindowState(windowState() | Qt::WindowFullScreen);

        _osdFloat->setWidget(_mediaPlayer->osd());
        _osdFloat->resize(_osdFloat->width(), _mediaPlayer->osd()->height());
        _osdFloat->setControls();
    } else {
        _mediaPlayer->restoreOsd();

        setWindowState(windowState() & ~Qt::WindowFullScreen);

        if (!ui->actionLite->isChecked())
            lite();
    }

    toggleMouse(!enabled);
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
    } else {
        _recorder->setVisible(false);

        /*if (_mediaPlayer->hasVout()) {
            ui->stackedWidget->setCurrentIndex(1);
        } else {
            ui->stackedWidget->setCurrentIndex(0);
        }*/
    }

    ui->actionRecorder->setChecked(enabled);
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
    ui->tabs->statusBar()->showMessage(tr("An update is available!"));
}
