/****************************************************************************
* MainWindow.cpp: A MainWindow class for Tano application
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QBitmap>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QSplashScreen>

#include "Config.h"
#include <vlc-qt/Config.h>

#include "core/Common.h"
#include "core/Settings.h"
#include "plugins/PluginsManager.h"
#include "ui/EditSettings.h"

const QString MainWindow::IDENTIFIER = "main";

MainWindow::MainWindow(QWidget *parent)	:
	QMainWindow(parent), ui(new Ui::MainWindow), _select(0), _locale(new LocaleManager()),
	_time(new Time()), _update(new UpdateManager()),
	_audioController(0), _mediaInstance(0), _mediaPlayer(0), _videoController(0),
	_playlistEditor(0), _timersEditor(0), _epg(new EpgManager()), _epgShow(new EpgShow()), _schedule(new Schedule())
{
	QPixmap pixmap(":/icons/images/splash.png");
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
	createRecorder();
	createConnections();

	splash->close();
	delete splash;
}

MainWindow::~MainWindow()
{

}

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
	openPlaylist(true);
	setState(false);
	ui->pageMain->setStyleSheet("background-color: rgb(0,0,0);");
	ui->statusBar->addPermanentWidget(ui->timeWidget);
}

void MainWindow::createBackend()
{
	_mediaInstance = new VlcInstance(Common::libvlcArgs(), this);
	_mediaPlayer = new VlcMediaPlayer(ui->videoWidget->widgetId(), this);

	_audioController = new VlcAudioControl();
	_videoController = new VlcVideoControl(_defaultSubtitleLanguage);

	ui->menuDeinterlacing->setEnabled(true);
}

void MainWindow::createSettings()
{
	Settings *settings = new Settings(this);
	_hideToTray = settings->hideToTray();

	//GUI Settings
	ui->toolBar->setToolButtonStyle(Qt::ToolButtonStyle(settings->toolbarLook()));

	_osdEnabled = settings->osd();
	_wheelType = settings->mouseWheel();
	mouseWheel();

	//Playback settings
	_defaultSubtitleLanguage = settings->subtitleLanguage();
	_videoSettings = settings->rememberVideoSettings();
	if(_videoController)
		_videoController->setDefaultSubtitleLanguage(_defaultSubtitleLanguage);

	//Recorder settings
	_recorderEnabled = settings->recorderEnabled();
	_sessionVolumeEnabled = settings->sessionVolume();
	_sessionAutoplayEnabled = settings->sessionAutoplay();

	delete settings;
}

void MainWindow::createSettingsStartup()
{
	Settings *settings = new Settings(this);
	_desktopWidth = QApplication::desktop()->width();
	_desktopHeight = QApplication::desktop()->height();

	_defaultPlaylist = settings->playlist();
	_updatesOnStart = settings->updatesCheck();

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
	connect(ui->actionUpdate, SIGNAL(triggered()), _update, SLOT(getUpdates()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui->actionAboutPlugins, SIGNAL(triggered()), this, SLOT(aboutPlugins()));
	connect(ui->actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui->actionClose, SIGNAL(triggered()), this, SLOT(exit()));

	connect(ui->actionTop, SIGNAL(triggered()), this, SLOT(top()));
	connect(ui->actionLite, SIGNAL(triggered()), this, SLOT(lite()));
	connect(ui->actionFullscreen, SIGNAL(toggled(bool)), this, SLOT(fullscreen(bool)));

	connect(ui->actionOpenToolbar, SIGNAL(triggered()), this, SLOT(menuOpen()));
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui->actionOpenSubtitles, SIGNAL(triggered()), this, SLOT(openSubtitles()));
	connect(ui->actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

	connect(ui->actionSchedule, SIGNAL(triggered()), this, SLOT(showSchedule()));
	connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
	connect(ui->actionEditPlaylist, SIGNAL(triggered()), this, SLOT(showPlaylistEditor()));

	connect(ui->actionPlay, SIGNAL(triggered()), _mediaPlayer, SLOT(pause()));
	connect(ui->actionStop, SIGNAL(triggered()), _mediaPlayer, SLOT(stop()));
	connect(ui->actionStop, SIGNAL(triggered()), this, SLOT(stop()));

	connect(ui->playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playChannel(QTreeWidgetItem*)));

	connect(_trayIcon, SIGNAL(restoreClick()), this, SLOT(tray()));
	connect(ui->actionTray, SIGNAL(triggered()), this, SLOT(tray()));

	connect(ui->videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(showRightMenu(QPoint)));
	connect(ui->videoWidget, SIGNAL(mouseShow(QPoint)), this, SLOT(showOsd(QPoint)));
	connect(ui->videoWidget, SIGNAL(mouseShow(QPoint)), ui->osdWidget, SLOT(show()));
	connect(ui->videoWidget, SIGNAL(mouseHide()), ui->osdWidget, SLOT(hide()));

	connect(_epg, SIGNAL(epg(QStringList, int, QString)), this, SLOT(showEpg(QStringList, int, QString)));
	connect(_epg, SIGNAL(epg(QStringList, int, QString)), _schedule, SLOT(loadEpg(QStringList, int, QString)));
	connect(_schedule, SIGNAL(requestEpg(QString, QString)), _epg, SLOT(request(QString, QString)));
	connect(_schedule, SIGNAL(urlClicked(QString)), _epgShow, SLOT(open(QString)));
	connect(ui->scheduleWidget, SIGNAL(urlClicked(QString)), _epgShow, SLOT(open(QString)));
	connect(ui->infoBarWidget, SIGNAL(open(QString)), _epgShow, SLOT(open(QString)));
	connect(_update, SIGNAL(updates(QStringList)), _trayIcon, SLOT(message(QStringList)));

	connect(_rightMenu, SIGNAL(aboutToHide()), ui->videoWidget, SLOT(enableMouseHide()));
	connect(_rightMenu, SIGNAL(aboutToShow()), ui->videoWidget, SLOT(disableMouseHide()));

	connect(_audioController, SIGNAL(actions(QString, QList<QAction*>)), this, SLOT(processMenu(QString, QList<QAction*>)));
	connect(_videoController, SIGNAL(actions(QString, QList<QAction*>)), this, SLOT(processMenu(QString, QList<QAction*>)));
	connect(_mediaPlayer, SIGNAL(state(bool, bool, bool)), this, SLOT(setState(bool, bool, bool)));

	connect(ui->actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
	connect(ui->actionRecordNow, SIGNAL(triggered()), this, SLOT(recordNow()));
	connect(ui->actionTimers, SIGNAL(triggered()), this, SLOT(showTimersEditor()));

	connect(_time, SIGNAL(startTimer(Timer*)), ui->recorder, SLOT(recordTimer(Timer*)));
	connect(_time, SIGNAL(stopTimer(Timer*)), ui->recorder, SLOT(stopTimer(Timer*)));
}

void MainWindow::createMenus()
{
	_ratioGroup = new QActionGroup(this);
	_ratioGroup->addAction(ui->actionRatioOriginal);
	_ratioGroup->addAction(ui->actionRatio1_1);
	_ratioGroup->addAction(ui->actionRatio4_3);
	_ratioGroup->addAction(ui->actionRatio16_9);
	_ratioGroup->addAction(ui->actionRatio16_10);
	_ratioGroup->addAction(ui->actionRatio2_21_1);
	_ratioGroup->addAction(ui->actionRatio5_4);

	_cropGroup = new QActionGroup(this);
	_cropGroup->addAction(ui->actionCropOriginal);
	_cropGroup->addAction(ui->actionCrop1_1);
	_cropGroup->addAction(ui->actionCrop4_3);
	_cropGroup->addAction(ui->actionCrop16_9);
	_cropGroup->addAction(ui->actionCrop16_10);
	_cropGroup->addAction(ui->actionCrop1_85_1);
	_cropGroup->addAction(ui->actionCrop2_21_1);
	_cropGroup->addAction(ui->actionCrop2_35_1);
	_cropGroup->addAction(ui->actionCrop2_39_1);
	_cropGroup->addAction(ui->actionCrop5_4);
	_cropGroup->addAction(ui->actionCrop5_3);

	_filterGroup = new QActionGroup(this);
	_filterGroup->addAction(ui->actionFilterDisabled);
	_filterGroup->addAction(ui->actionFilterDiscard);
	_filterGroup->addAction(ui->actionFilterBlend);
	_filterGroup->addAction(ui->actionFilterMean);
	_filterGroup->addAction(ui->actionFilterBob);
	_filterGroup->addAction(ui->actionFilterLinear);
	_filterGroup->addAction(ui->actionFilterX);

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
	_rightMenu->addAction(ui->actionClose);

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
		<< ui->actionMute
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
		<< ui->actionTray;

	_shortcuts = new Shortcuts(_actions, this);
}

void MainWindow::createSession()
{
	ui->volumeSlider->setVolume(_sessionVolume);

	if(_sessionAutoplayEnabled && _hasPlaylist)
		playChannel(_sessionChannel);

	if(_updatesOnStart)
		_update->getUpdates();
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

void MainWindow::createRecorder()
{
	if(_recorderEnabled) {
		ui->recorder->openPlaylist(_playlistName);
		ui->recorder->setGlobals(_trayIcon, ui->actionRecord);
		ui->recorder->createSettings();
		if(!_timersEditor)
			_timersEditor = new EditTimers(_time, _playlistName, this);
		if(ui->buttonRecord->isHidden()) {
			ui->buttonRecord->show();
			ui->menuRecorder->setEnabled(true);
		}
		ui->toolBar->insertAction(ui->actionEditPlaylist, ui->actionRecorder);
		ui->toolBar->insertAction(ui->actionEditPlaylist, ui->actionTimers);
	} else {
		ui->buttonRecord->hide();
		ui->menuRecorder->setEnabled(false);
		ui->toolBar->removeAction(ui->actionRecorder);
		ui->toolBar->removeAction(ui->actionTimers);
	}
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
	Common::about();
}
void MainWindow::aboutPlugins()
{
	PluginsManager p;
	p.exec();
}


//Media controls
void MainWindow::playChannel(QTreeWidgetItem* clickedChannel)
{
	_channel = ui->playlistWidget->channelRead(clickedChannel);
	play();
}
void MainWindow::playChannel(const int &clickedChannel)
{
	_channel = ui->playlistWidget->channelRead(clickedChannel);
	play();
}

void MainWindow::setState(const bool &playing, const bool &audio, const bool &video)
{
	if(playing) {
		ui->actionPlay->setIcon(QIcon(":/icons/images/player_pause.png"));
		ui->buttonPlay->setIcon(QIcon(":/icons/images/player_pause.png"));
		ui->actionPlay->setText(tr("Pause"));
		ui->actionPlay->setToolTip(tr("Pause"));
		ui->buttonPlay->setToolTip(tr("Pause"));
		ui->buttonPlay->setStatusTip(tr("Pause"));
		ui->actionMute->setEnabled(true);
		ui->buttonMute->setEnabled(true);
	} else {
		ui->actionPlay->setIcon(QIcon(":/icons/images/player_play.png"));
		ui->buttonPlay->setIcon(QIcon(":/icons/images/player_play.png"));
		ui->actionPlay->setText(tr("Play"));
		ui->actionPlay->setToolTip(tr("Play"));
		ui->buttonPlay->setToolTip(tr("Play"));
		ui->buttonPlay->setStatusTip(tr("Play"));
		ui->actionMute->setEnabled(false);
		ui->buttonMute->setEnabled(false);
	}

	ui->menuAudio->setEnabled(audio);
	ui->menuVideo->setEnabled(video);
}

void MainWindow::play(const QString &itemFile)
{
	this->stop();

	if(itemFile.isNull()) {
		ui->infoBarWidget->setInfo(_channel->name(), _channel->language());
		ui->infoBarWidget->setLogo(_channel->logo());

		_epg->request(_channel->epg(), MainWindow::IDENTIFIER);
		ui->channelNumber->display(_channel->number());

		_mediaPlayer->open(_channel->url());
		tooltip(_channel->name());
		_trayIcon->changeToolTip(_channel->name());
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
		ui->actionRatioOriginal->trigger();
		ui->actionCropOriginal->trigger();
	}

	_epg->stop();

	ui->infoBarWidget->clear();

	ui->scheduleWidget->clear();
	ui->scheduleWidget->setPage(0);

	tooltip();
	_trayIcon->changeToolTip();

	_audioController->reset();
	_videoController->reset();
}

void MainWindow::showEpg(const QStringList &epgValue, const int &id, const QString &identifier)
{
	if(identifier != MainWindow::IDENTIFIER)
		return;

	switch (id) {
		case 0:
			ui->infoBarWidget->setEpg(epgValue[0], epgValue[1]);
			break;
		case 1:
		case 2:
		case 3:
		case 4:
			ui->scheduleWidget->setEpg(epgValue, id);
			ui->scheduleWidget->setPage(1);
			break;
		default:
			break;
	}
}

void MainWindow::processMenu(const QString &type, const QList<QAction *> &list)
{
	QMenu *menu;
	if(type == "sub")
		menu = ui->menuSubtitles;
	else if(type == "audio")
		menu = ui->menuAudioTrack;
	else if(type == "video")
		menu = ui->menuVideoTrack;

	if(list.size()==0) {
		menu->setDisabled(true);
		ui->menuSubtitles->setDisabled(false);
		return;
	} else {
		menu->setDisabled(false);
	}

	for(int i = 0; i < list.size(); ++i)
		menu->addAction(list[i]);
}

// Open dialogs
void MainWindow::openPlaylist(const bool &start)
{
	if(_select != 0) {
		disconnect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
		disconnect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
		disconnect(_select, SIGNAL(channelSelect(int)), this, SLOT(playChannel(int)));
		delete _select;
	}

	if (!start)
	_playlistName =
		QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
						QDir::homePath(),
						tr("Tano TV Channel list Files(*.m3u)"));
	else
		_playlistName = Common::locateResource(_defaultPlaylist);

	if (!_playlistName.isEmpty()) {
		_schedule->openPlaylist(_playlistName);
		ui->recorder->openPlaylist(_playlistName);
	} else {
		return;
	}

	ui->playlistWidget->open(_playlistName);

	_hasPlaylist = true;

	_select = new ChannelSelect(this, ui->channelNumber, ui->playlistWidget->nums());
	connect(ui->actionBack, SIGNAL(triggered()), _select, SLOT(back()));
	connect(ui->actionNext, SIGNAL(triggered()), _select, SLOT(next()));
	connect(_select, SIGNAL(channelSelect(int)), this, SLOT(playChannel(int)));
	mouseWheel();

	ui->channelToolBox->setItemText(0,ui->playlistWidget->name());
	_epg->setEpg(ui->playlistWidget->epg(), ui->playlistWidget->epgPlugin());
	_epgShow->loadPlugin(ui->playlistWidget->epgPlugin());
}
void MainWindow::openFile()
{
	QString file =
		QFileDialog::getOpenFileName(this, tr("Open File or URL"),
						QDir::homePath(),
						tr("Multimedia files(*)"));

	if (file.isEmpty())
		return;

	play(file);
}
void MainWindow::openUrl()
{
	bool ok;
	QString file =
		QInputDialog::getText(this, tr("Open URL or stream"),
							 tr("Enter the URL of multimedia file or stream you want to play:"),
							 QLineEdit::Normal, "", &ok);

	if (!ok && file.isEmpty())
		return;

	play(file);
}
void MainWindow::openSubtitles()
{
	QString file =
		QFileDialog::getOpenFileName(this, tr("Open Subtitles file"),
						QDir::homePath(),
						tr("Subtitles files(*.sub *.srt *.txt)"));

	if (file.isEmpty())
		return;

	_videoController->loadSubtitle(file);
}


//GUI
void MainWindow::showSchedule()
{
	_schedule->show();
}

void MainWindow::showSettings()
{
	EditSettings s(_shortcuts, this);
	s.exec();
	_locale->setLocale();
	createSettings();
	createRecorder();
}

void MainWindow::showPlaylistEditor()
{
	if(_playlistEditor) {
		if(_playlistEditor->isVisible()) {
			_playlistEditor->activateWindow();
		} else {
			delete _playlistEditor;
			_playlistEditor = new EditPlaylist(_playlistName, ui->videoWidget->widgetId());
			_playlistEditor->show();
		}
	} else {
		_playlistEditor = new EditPlaylist(_playlistName, ui->videoWidget->widgetId());
		_playlistEditor->show();
	}
}

void MainWindow::showTimersEditor()
{
	_timersEditor->showTimersEditor();
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
		ui->osdWidget->resize(2*_desktopWidth/3,ui->osdWidget->height());
		ui->osdWidget->setFloating(true);
		ui->osdWidget->move(_desktopWidth/6, _desktopHeight-ui->osdWidget->height());
		ui->osdWidget->show();
		ui->osdWidget->setWindowFlags(Qt::ToolTip);
	} else {
		ui->osdWidget->setFloating(false);
		ui->osdWidget->show();
	}
}

void MainWindow::showOsd(const QPoint &pos)
{
	if((pos.x() < ui->osdWidget->pos().x()+ui->osdWidget->width()) &&
	   (pos.x() > ui->osdWidget->pos().x()) &&
	   (pos.y() < ui->osdWidget->pos().y()+ui->osdWidget->height()) &&
	   (pos.y() > ui->osdWidget->pos().y())) {
		ui->videoWidget->disableMouseHide();
	} else {
		ui->videoWidget->enableMouseHide();
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
