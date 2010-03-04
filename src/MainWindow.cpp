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

#include <QtCore/QDir>
#include <QtGui/QApplication>
#include <QtGui/QBitmap>
#include <QtGui/QDesktopWidget>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QSplashScreen>

#include "MainWindow.h"
#include "Common.h"
#include "plugins/PluginsManager.h"
#include "ui/EditSettings.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), _select(0), _time(new Time()), _update(new Updates()),
	_playlistEditor(0), _timersEditor(0), _epg(new EpgLoader(Common::defaultEpgPlugin())), _epgShow(new EpgShow())
{
	QPixmap pixmap(":/icons/images/splash.png");
	QSplashScreen *splash = new QSplashScreen(pixmap);
	splash->setMask(pixmap.mask());
	splash->show();

	ui.setupUi(this);

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
	if(_sessionEnabled) {
		QSettings session(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
		session.beginGroup("Session");
		session.setValue("volume", ui.volumeSlider->volume());
		session.setValue("channel", ui.channelNumber->value());
		session.endGroup();
	}
}

void MainWindow::exit()
{
	int ret;
	if(ui.recorder->isRecording()) {
		ret = QMessageBox::warning(this, tr("Tano"),
					  tr("Do you want to exit Tano?\nThis will stop recording in progress."),
					  QMessageBox::Close | QMessageBox::Cancel,
					  QMessageBox::Close);
	} else {
		ret = QMessageBox::Close;
	}

	switch (ret) {
		case QMessageBox::Close:
			ui.recorder->stop();
			qApp->quit();
			break;
		case QMessageBox::Cancel:
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

// Init functions
void MainWindow::createGui()
{
	openPlaylist(true);
	playingState(-1);
	ui.pageMain->setStyleSheet("background-color: rgb(0,0,0);");
}

void MainWindow::createBackend()
{
	_backend = new VlcInstance(Common::libvlcArgs(), ui.videoWidget->getWinId());
	_backend->init();

	_controller = new VlcControl(_defaultSubtitleLanguage);

#if VLC_TRUNK
	ui.menuDeinterlacing->setEnabled(true);
#endif
}

void MainWindow::createSettings()
{
	_desktopWidth = QApplication::desktop()->width();
	_desktopHeight = QApplication::desktop()->height();

	_settings = Common::settings();
	_defaultPlaylist = _settings->value("playlist","playlists/siol-mpeg2.m3u").toString();
	_hideToTray = _settings->value("tray",false).toBool();
	_updatesOnStart = _settings->value("updates",true).toBool();

	//Session
	_sessionEnabled = _settings->value("session", true).toBool();
	_settings->beginGroup("Session");
	_sessionVolume = _settings->value("volume",50).toString().toInt();
	_sessionChannel = _settings->value("channel",1).toInt();
	_settings->endGroup();

	//GUI Settings
	_settings->beginGroup("GUI");
	if(_settings->value("lite",false).toBool()) {
		ui.actionLite->setChecked(true);
		lite();
	} else
		_isLite = false;

	if(_settings->value("ontop",false).toBool()) {
		ui.actionTop->setChecked(true);;
		top();
	}

	_osdEnabled = _settings->value("OSD",true).toBool();

	ui.osdWidget->setVisible(_settings->value("controls",true).toBool());
	ui.actionControls->setChecked(_settings->value("controls",true).toBool());
	ui.infoWidget->setVisible(_settings->value("info",true).toBool());
	ui.actionInfoPanel->setChecked(_settings->value("info",true).toBool());

	if(_settings->value("wheel",false).toBool())
		_wheelType = "volume";
	else
		_wheelType = "channel";
	_settings->endGroup();

	//Playback settings
	_settings->beginGroup("VLC");
	_defaultSubtitleLanguage = _settings->value("default-sub-lang",tr("Disabled")).toString();
	_videoSettings = _settings->value("remember-video-config",false).toBool();
	_settings->endGroup();

	//Recorder settings
	_settings->beginGroup("Recorder");
	_recorderEnabled = _settings->value("enabled",true).toBool();
	_settings->endGroup();
}

void MainWindow::createConnections()
{
	connect(ui.actionUpdate, SIGNAL(triggered()), _update, SLOT(getUpdates()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui.actionAboutPlugins, SIGNAL(triggered()), this, SLOT(aboutPlugins()));
	connect(ui.actionAboutQt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(exit()));

	connect(ui.actionTop, SIGNAL(triggered()), this, SLOT(top()));
	connect(ui.actionLite, SIGNAL(triggered()), this, SLOT(lite()));
	connect(ui.actionFullscreen, SIGNAL(toggled(bool)), this, SLOT(fullscreen(bool)));

	connect(ui.actionOpenToolbar, SIGNAL(triggered()), this, SLOT(menuOpen()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), this, SLOT(showPlaylistEditor()));

	connect(ui.actionPlay, SIGNAL(triggered()), _backend, SLOT(pause()));
	connect(ui.actionStop, SIGNAL(triggered()), _backend, SLOT(stop()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));

	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playChannel(QTreeWidgetItem*)));

	connect(_trayIcon, SIGNAL(restoreClick()), this, SLOT(tray()));
	connect(ui.actionTray, SIGNAL(triggered()), this, SLOT(tray()));

	connect(ui.videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(showRightMenu(QPoint)));
	connect(ui.videoWidget, SIGNAL(osdVisibility(bool)), ui.osdWidget, SLOT(setVisible(bool)));

	connect(_epg, SIGNAL(epgDone(QStringList, int)), this, SLOT(showEpg(QStringList, int)));
	connect(ui.epgToday, SIGNAL(urlClicked(QString)), _epgShow, SLOT(open(QString)));
	connect(ui.epgToday_2, SIGNAL(urlClicked(QString)), _epgShow, SLOT(open(QString)));
	connect(ui.epgToday_3, SIGNAL(urlClicked(QString)), _epgShow, SLOT(open(QString)));
	connect(ui.epgToday_4, SIGNAL(urlClicked(QString)), _epgShow, SLOT(open(QString)));
	connect(ui.infoBarWidget, SIGNAL(open(QString)), _epgShow, SLOT(open(QString)));
	connect(_update, SIGNAL(updatesDone(QStringList)), _trayIcon, SLOT(message(QStringList)));

	connect(_rightMenu, SIGNAL(aboutToHide()), ui.videoWidget, SLOT(enableMove()));
	connect(_rightMenu, SIGNAL(aboutToShow()), ui.videoWidget, SLOT(disableMove()));

	connect(_controller, SIGNAL(vlcAction(QString, QList<QAction*>)), this, SLOT(processMenu(QString, QList<QAction*>)));
	connect(_controller, SIGNAL(stateChanged(int)), this, SLOT(playingState(int)));

	connect(ui.actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
	connect(ui.actionRecordNow, SIGNAL(triggered()), this, SLOT(recordNow()));
	connect(ui.actionTimers, SIGNAL(triggered()), this, SLOT(showTimersEditor()));

	if(_wheelType == "volume")
		connect(ui.videoWidget, SIGNAL(wheel(bool)), ui.volumeSlider, SLOT(volumeControl(bool)));

	connect(_time, SIGNAL(startTimer(Timer*)), ui.recorder, SLOT(recordTimer(Timer*)));
	connect(_time, SIGNAL(stopTimer(Timer*)), ui.recorder, SLOT(stopTimer(Timer*)));
}

void MainWindow::createMenus()
{
	_ratioGroup = new QActionGroup(this);
	_ratioGroup->addAction(ui.actionRatioOriginal);
	_ratioGroup->addAction(ui.actionRatio1_1);
	_ratioGroup->addAction(ui.actionRatio4_3);
	_ratioGroup->addAction(ui.actionRatio16_9);
	_ratioGroup->addAction(ui.actionRatio16_10);
	_ratioGroup->addAction(ui.actionRatio2_21_1);
	_ratioGroup->addAction(ui.actionRatio5_4);

	_cropGroup = new QActionGroup(this);
	_cropGroup->addAction(ui.actionCropOriginal);
	_cropGroup->addAction(ui.actionCrop1_1);
	_cropGroup->addAction(ui.actionCrop4_3);
	_cropGroup->addAction(ui.actionCrop16_9);
	_cropGroup->addAction(ui.actionCrop16_10);
	_cropGroup->addAction(ui.actionCrop1_85_1);
	_cropGroup->addAction(ui.actionCrop2_21_1);
	_cropGroup->addAction(ui.actionCrop2_35_1);
	_cropGroup->addAction(ui.actionCrop2_39_1);
	_cropGroup->addAction(ui.actionCrop5_4);
	_cropGroup->addAction(ui.actionCrop5_3);

	_filterGroup = new QActionGroup(this);
	_filterGroup->addAction(ui.actionFilterDisabled);
	_filterGroup->addAction(ui.actionFilterDiscard);
	_filterGroup->addAction(ui.actionFilterBlend);
	_filterGroup->addAction(ui.actionFilterMean);
	_filterGroup->addAction(ui.actionFilterBob);
	_filterGroup->addAction(ui.actionFilterLinear);
	_filterGroup->addAction(ui.actionFilterX);

	_rightMenu = new QMenu();
	_rightMenu->addAction(ui.actionPlay);
	_rightMenu->addAction(ui.actionStop);
	_rightMenu->addAction(ui.actionBack);
	_rightMenu->addAction(ui.actionNext);
	_rightMenu->addSeparator();
	_rightMenu->addAction(ui.actionTop);
	_rightMenu->addAction(ui.actionLite);
	_rightMenu->addAction(ui.actionFullscreen);
	_rightMenu->addSeparator();
	_rightMenu->addMenu(ui.menuVolume);
	_rightMenu->addMenu(ui.menuVideo);
	_rightMenu->addSeparator();
	if(_recorderEnabled) {
		_rightMenu->addAction(ui.actionRecordNow);
		_rightMenu->addAction(ui.actionRecord);
		_rightMenu->addSeparator();
	}
	_rightMenu->addAction(ui.actionTray);
	_rightMenu->addAction(ui.actionClose);

	_openMenu = new QMenu();
	_openMenu->addAction(ui.actionOpenFile);
	_openMenu->addAction(ui.actionOpenUrl);
	_openMenu->addAction(ui.actionOpen);

	_trayIcon = new TrayIcon(_rightMenu);
	_trayIcon->show();
}

void MainWindow::createShortcuts()
{
	_actions << ui.actionPlay
		<< ui.actionStop
		<< ui.actionNext
		<< ui.actionBack
		<< ui.actionFullscreen
		<< ui.actionMute
		<< ui.actionVolumeUp
		<< ui.actionVolumeDown
		<< ui.actionRecorder
		<< ui.actionOpenFile
		<< ui.actionOpenUrl
		<< ui.actionOpen
		<< ui.actionEditPlaylist
		<< ui.actionSettings
		<< ui.actionTop
		<< ui.actionLite
		<< ui.actionTray
		<< ui.actionAbout;

	_shortcuts = new Shortcuts(_actions);
}

void MainWindow::createSession()
{
	if(_sessionEnabled) {
		ui.volumeSlider->setVolume(_sessionVolume);
		if(_hasPlaylist)
			playChannel(_sessionChannel);
	} else {
		ui.volumeSlider->setVolume(50);
	}

	if(_updatesOnStart)
		_update->getUpdates();
}

void MainWindow::createRecorder()
{
	if(_recorderEnabled) {
		ui.recorder->openPlaylist(_playlistName);
		ui.recorder->setGlobals(_trayIcon, ui.actionRecord);
		_timersEditor = new EditTimers(_time, _playlistName, this);
	} else {
		ui.buttonRecord->hide();
		ui.menuRecorder->hide();
		ui.toolBar->removeAction(ui.actionRecorder);
		ui.toolBar->removeAction(ui.actionTimers);
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
	_channel = ui.playlistWidget->channelRead(clickedChannel);
	play();
}
void MainWindow::playChannel(const int &clickedChannel)
{
	_channel = ui.playlistWidget->channelRead(clickedChannel);
	play();
}

void MainWindow::playingState(const int &status)
{
	if(status == 1) {
		ui.actionPlay->setEnabled(true);
		ui.buttonPlay->setEnabled(true);
		ui.actionPlay->setIcon(QIcon(":/icons/images/player_pause.png"));
		ui.buttonPlay->setIcon(QIcon(":/icons/images/player_pause.png"));
		ui.actionPlay->setText(tr("Pause"));
		ui.actionPlay->setToolTip(tr("Pause"));
		ui.buttonPlay->setToolTip(tr("Pause"));
		ui.buttonPlay->setStatusTip(tr("Pause"));
		ui.actionMute->setEnabled(true);
		ui.buttonMute->setEnabled(true);
	} else if(status == 0 || status == -1) {
		ui.actionPlay->setEnabled(true);
		ui.buttonPlay->setEnabled(true);
		ui.actionPlay->setIcon(QIcon(":/icons/images/player_play.png"));
		ui.buttonPlay->setIcon(QIcon(":/icons/images/player_play.png"));
		ui.actionPlay->setText(tr("Play"));
		ui.actionPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setStatusTip(tr("Play"));
		ui.actionMute->setEnabled(false);
		ui.buttonMute->setEnabled(false);
	}
}

void MainWindow::play(const QString &itemFile)
{
	this->stop();

	if(itemFile.isNull()) {
		ui.infoBarWidget->setInfo(_channel->name(), _channel->language());

		_epg->getEpg(_channel->epg());
		ui.channelNumber->display(_channel->number());

		_backend->openMedia(_channel->url());
		tooltip(_channel->name());
		_trayIcon->changeToolTip(_channel->name());
	} else {
		ui.infoWidget->hide();
		_backend->openMedia(itemFile);
		tooltip(itemFile);
	}

	if(_videoSettings)
		ui.videoWidget->setPreviousSettings();
}

void MainWindow::stop()
{
	ui.epgToday->clearList();
	ui.epgToday_2->clearList();
	ui.epgToday_3->clearList();
	ui.epgToday_4->clearList();

	if(!_videoSettings) {
		ui.actionRatioOriginal->trigger();
		ui.actionCropOriginal->trigger();
	}

	ui.infoBarWidget->clear();
	_epg->stop();
	tooltip();
	_trayIcon->changeToolTip();

	_controller->update();
}

void MainWindow::showEpg(const QStringList &epgValue, const int &id)
{
	switch (id) {
		case 0:
			ui.infoBarWidget->setEpg(epgValue.at(0), epgValue.at(1));
			break;
		case 1:
			ui.epgTabWidget->setTabText(0,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday->setEpg(epgValue);
			break;
		case 2:
			ui.epgTabWidget->setTabText(1,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday_2->setEpg(epgValue);
			break;
		case 3:
			ui.epgTabWidget->setTabText(2,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday_3->setEpg(epgValue);
			break;
		case 4:
			ui.epgTabWidget->setTabText(3,QDate::currentDate().addDays(id-1).toString("d.M."));
			ui.epgToday_4->setEpg(epgValue);
			break;
		default:
			break;
	}
}

void MainWindow::processMenu(const QString &type, const QList<QAction *> &list)
{
	if(type == "sub")
		ui.menuSubtitles->clear();
	else if(type == "audio")
		ui.menuAudio_channel->clear();

	if(list.size()==0) {
		if(type == "sub")
			ui.menuSubtitles->setDisabled(true);
		else if(type == "audio")
			ui.menuAudio_channel->setDisabled(true);

		return;
	} else {
		if(type == "sub")
			ui.menuSubtitles->setDisabled(false);
		else if(type == "audio")
			ui.menuAudio_channel->setDisabled(false);
	}

	for (int i = 0; i < list.size(); ++i) {
		if(type == "sub")
			ui.menuSubtitles->addAction(list.at(i));
		else if(type == "audio")
			ui.menuAudio_channel->addAction(list.at(i));
	}
}

// Open dialogs
void MainWindow::openPlaylist(const bool &start)
{
	if(_select != 0) {
		disconnect(ui.actionBack, SIGNAL(triggered()), _select, SLOT(back()));
		disconnect(ui.actionNext, SIGNAL(triggered()), _select, SLOT(next()));
		disconnect(_select, SIGNAL(channelSelect(int)), this, SLOT(playChannel(int)));
		if(_wheelType != "volume")
			disconnect(ui.videoWidget, SIGNAL(wheel(bool)), _select, SLOT(channel(bool)));
		delete _select;
	}

	if (!start)
	_playlistName =
		QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
						QDir::homePath(),
						tr("Tano TV Channel list Files(*.m3u)"));
	else
		_playlistName = Common::locateResource(_defaultPlaylist);

	if (!_playlistName.isEmpty())
		ui.recorder->openPlaylist(_playlistName);
	else
		return;

	ui.playlistWidget->open(_playlistName);

	_hasPlaylist = true;

	_select = new ChannelSelect(this, ui.channelNumber, ui.playlistWidget->nums());
	connect(ui.actionBack, SIGNAL(triggered()), _select, SLOT(back()));
	connect(ui.actionNext, SIGNAL(triggered()), _select, SLOT(next()));
	connect(_select, SIGNAL(channelSelect(int)), this, SLOT(playChannel(int)));
	if(_wheelType != "volume")
		connect(ui.videoWidget, SIGNAL(wheel(bool)), _select, SLOT(channel(bool)));

	ui.channelToolBox->setItemText(0,ui.playlistWidget->name());
}
void MainWindow::openFile()
{
	_fileName =
		QFileDialog::getOpenFileName(this, tr("Open File or URL"),
						QDir::homePath(),
						tr("Multimedia files(*)"));

	if (_fileName.isEmpty())
		return;

	play(_fileName);
}
void MainWindow::openUrl()
{
	bool ok;
	_fileName =
		QInputDialog::getText(this, tr("Open URL or stream"),
							 tr("Enter the URL of multimedia file or stream you want to play:"),
							 QLineEdit::Normal, "", &ok);

	if (!ok && _fileName.isEmpty())
		return;

	play(_fileName);
}


//GUI
void MainWindow::showSettings()
{
	EditSettings s(this, _shortcuts);
	s.exec();
}

void MainWindow::showPlaylistEditor()
{
	if(_playlistEditor)
		delete _playlistEditor;

	_playlistEditor = new EditPlaylist(_playlistName);
	_playlistEditor->show();
}

void MainWindow::showTimersEditor()
{
	_timersEditor->show();
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
	if(ui.actionTop->isChecked())
		this->setWindowFlags(top);
	else
		this->setWindowFlags(_flags);

	this->show();
}

void MainWindow::lite()
{
	ui.infoWidget->setVisible(_isLite);
	ui.toolBar->setVisible(_isLite);
	ui.osdWidget->setVisible(_isLite);
	_isLite = !_isLite;
}

void MainWindow::tray()
{
	if (!_trayIcon->isVisible())
		return;

	if(this->isHidden()) {
		ui.actionTray->setText(tr("Hide to tray"));
		show();
		ui.osdWidget->setVisible(_controlsVisible);
		ui.infoWidget->setVisible(_infoWidgetVisible);
	} else {
		_controlsVisible = ui.osdWidget->isVisible();
		_infoWidgetVisible = ui.infoWidget->isVisible();
		ui.actionTray->setText(tr("Restore"));
		_trayIcon->message(QStringList() << "close");
		hide();
	}
}

void MainWindow::fullscreen(const bool &on)
{
	if(!_osdEnabled)
		return;

	if(on) {
		ui.videoWidget->setOsdSize(ui.osdWidget->width(), ui.osdWidget->height());
		ui.osdWidget->resize(2*_desktopWidth/3,ui.osdWidget->height());
		ui.osdWidget->setFloating(true);
		ui.osdWidget->move(_desktopWidth/6, _desktopHeight-ui.osdWidget->height());
		ui.osdWidget->show();
		ui.osdWidget->setWindowFlags(Qt::ToolTip);
	} else {
		ui.osdWidget->setFloating(false);
		ui.osdWidget->show();
	}
}

// Recorder
void MainWindow::recordNow()
{
	ui.recorder->recordNow(_channel->name(), _channel->url());
}

void MainWindow::recorder(const bool &enabled)
{
	if(enabled) {
		ui.stackedWidget->setCurrentIndex(1);
		ui.infoWidget->setVisible(false);
		ui.osdWidget->setVisible(false);
	} else {
		ui.stackedWidget->setCurrentIndex(0);
		ui.infoWidget->setVisible(true);
		ui.osdWidget->setVisible(true);
	}
}
