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
	: QMainWindow(parent)
{
	QPixmap pixmap(":/icons/images/splash.png");
	QSplashScreen *splash = new QSplashScreen(pixmap);
	splash->setMask(pixmap.mask());
	splash->show();

	createCommon();
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
void MainWindow::createCommon()
{
	ui.setupUi(this);

	flags = this->windowFlags();

	update = new Updates();
	epg = new Epg();
	epgShow = new EpgShow();
	select = 0;

	editor = new EditPlaylist(this);

	time = new Time();
}

void MainWindow::createGui()
{
	openPlaylist(true);

	ui.pageMain->setStyleSheet("background-color: rgb(0,0,0);\nbackground-image: url(:/icons/images/name.png);\nbackground-position: center;\nbackground-repeat: none;");
}

void MainWindow::createBackend()
{
	backend = new VlcInstance(Common::libvlcArgs(), ui.videoWidget->getWinId());
	backend->init();

	controller = new VlcControl(_defaultSubtitleLanguage);

#if VLC_TRUNK
	ui.menuDeinterlacing->setEnabled(true);
#endif
}

void MainWindow::createSettings()
{
	_desktopWidth = QApplication::desktop()->width();
	_desktopHeight = QApplication::desktop()->height();

	settings = Common::settings();
	_defaultPlaylist = settings->value("playlist","playlists/siol-mpeg2.m3u").toString();
	_hideToTray = settings->value("tray",false).toBool();
	_updatesOnStart = settings->value("updates",true).toBool();

	//Session
	_sessionEnabled = settings->value("session", true).toBool();
	settings->beginGroup("Session");
	_sessionVolume = settings->value("volume",50).toString().toInt();
	_sessionChannel = settings->value("channel",1).toInt();
	settings->endGroup();

	//GUI Settings
	settings->beginGroup("GUI");
	if(settings->value("lite",false).toBool()) {
		ui.actionLite->setChecked(true);
		lite();
	} else
		_isLite = false;

	if(settings->value("ontop",false).toBool()) {
		ui.actionTop->setChecked(true);;
		top();
	}

	if(settings->value("OSD",true).toBool())
		_osdEnabled = true;
	else
		_osdEnabled = false;

	if(!settings->value("info",true).toBool()) {
		ui.infoWidget->hide();
		ui.actionInfoPanel->setChecked(false);
	}

	if(settings->value("wheel",false).toBool())
		_wheelType = "volume";
	else
		_wheelType = "channel";
	settings->endGroup();

	//Playback settings
	settings->beginGroup("VLC");
	_defaultSubtitleLanguage = settings->value("default-sub-lang",tr("Disabled")).toString();
	_videoSettings = settings->value("remember-video-config",false).toBool();
	settings->endGroup();

	//Recorder settings
	settings->beginGroup("Recorder");
	if(settings->value("enabled",true).toBool())
		_recorderEnabled = true;
	else
		_recorderEnabled = false;
	settings->endGroup();
}

void MainWindow::createConnections()
{
	connect(ui.actionUpdate, SIGNAL(triggered()), update, SLOT(getUpdates()));
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
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), editor, SLOT(open()));

	connect(ui.actionPlay, SIGNAL(triggered()), backend, SLOT(pause()));
	connect(ui.actionStop, SIGNAL(triggered()), backend, SLOT(stop()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	connect(ui.actionBack, SIGNAL(triggered()), select, SLOT(back()));
	connect(ui.actionNext, SIGNAL(triggered()), select, SLOT(next()));

	connect(ui.infoBarWidget, SIGNAL(refresh()), epg, SLOT(refresh()));
	connect(ui.infoBarWidget, SIGNAL(open(QString)), epgShow, SLOT(open(QString)));

	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playChannel(QTreeWidgetItem*)));
	connect(select, SIGNAL(channelSelect(int)), this, SLOT(playChannel(int)));

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(tray()));
	connect(ui.actionTray, SIGNAL(triggered()), this, SLOT(tray()));

	connect(ui.videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(showRightMenu(QPoint)));
	connect(ui.videoWidget, SIGNAL(osdVisibility(bool)), ui.osdWidget, SLOT(setVisible(bool)));

	connect(epg, SIGNAL(epgDone(int,QStringList,QString)), this, SLOT(showEpg(int,QStringList,QString)));
	connect(ui.epgToday, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_2, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_3, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_4, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(update, SIGNAL(updatesDone(QStringList)), trayIcon, SLOT(message(QStringList)));

	connect(rightMenu, SIGNAL(aboutToHide()), ui.videoWidget, SLOT(enableMove()));
	connect(rightMenu, SIGNAL(aboutToShow()), ui.videoWidget, SLOT(disableMove()));

	connect(controller, SIGNAL(vlcAction(QString, QList<QAction*>)), this, SLOT(processMenu(QString, QList<QAction*>)));
	connect(controller, SIGNAL(stateChanged(int)), this, SLOT(playingState(int)));

	connect(ui.actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
	connect(ui.actionRecordNow, SIGNAL(triggered()), this, SLOT(recordNow()));
	connect(ui.actionTimers, SIGNAL(triggered()), timers, SLOT(show()));

	if(_wheelType == "volume")
		connect(ui.videoWidget, SIGNAL(wheel(bool)), ui.volumeSlider, SLOT(volumeControl(bool)));
	else
		connect(ui.videoWidget, SIGNAL(wheel(bool)), select, SLOT(channel(bool)));

	connect(ui.actionTest_button, SIGNAL(triggered()), this, SLOT(test()));
}

void MainWindow::createMenus()
{
	ratioGroup = new QActionGroup(this);
	ratioGroup->addAction(ui.actionRatioOriginal);
	ratioGroup->addAction(ui.actionRatio1_1);
	ratioGroup->addAction(ui.actionRatio4_3);
	ratioGroup->addAction(ui.actionRatio16_9);
	ratioGroup->addAction(ui.actionRatio16_10);
	ratioGroup->addAction(ui.actionRatio2_21_1);
	ratioGroup->addAction(ui.actionRatio5_4);

	cropGroup = new QActionGroup(this);
	cropGroup->addAction(ui.actionCropOriginal);
	cropGroup->addAction(ui.actionCrop1_1);
	cropGroup->addAction(ui.actionCrop4_3);
	cropGroup->addAction(ui.actionCrop16_9);
	cropGroup->addAction(ui.actionCrop16_10);
	cropGroup->addAction(ui.actionCrop1_85_1);
	cropGroup->addAction(ui.actionCrop2_21_1);
	cropGroup->addAction(ui.actionCrop2_35_1);
	cropGroup->addAction(ui.actionCrop2_39_1);
	cropGroup->addAction(ui.actionCrop5_4);
	cropGroup->addAction(ui.actionCrop5_3);

	filterGroup = new QActionGroup(this);
	filterGroup->addAction(ui.actionFilterDisabled);
	filterGroup->addAction(ui.actionFilterDiscard);
	filterGroup->addAction(ui.actionFilterBlend);
	filterGroup->addAction(ui.actionFilterMean);
	filterGroup->addAction(ui.actionFilterBob);
	filterGroup->addAction(ui.actionFilterLinear);
	filterGroup->addAction(ui.actionFilterX);

	rightMenu = new QMenu();
	rightMenu->addAction(ui.actionPlay);
	rightMenu->addAction(ui.actionStop);
	rightMenu->addAction(ui.actionBack);
	rightMenu->addAction(ui.actionNext);
	rightMenu->addSeparator();
	rightMenu->addAction(ui.actionTop);
	rightMenu->addAction(ui.actionLite);
	rightMenu->addAction(ui.actionFullscreen);
	rightMenu->addSeparator();
	rightMenu->addMenu(ui.menuVolume);
	rightMenu->addMenu(ui.menuVideo);
	rightMenu->addSeparator();
	if(_recorderEnabled) {
		rightMenu->addAction(ui.actionRecordNow);
		rightMenu->addAction(ui.actionRecord);
		rightMenu->addSeparator();
	}
	rightMenu->addAction(ui.actionTray);
	rightMenu->addAction(ui.actionClose);

	openMenu = new QMenu();
	openMenu->addAction(ui.actionOpenFile);
	openMenu->addAction(ui.actionOpenUrl);
	openMenu->addAction(ui.actionOpen);

	trayIcon = new TrayIcon(rightMenu);
	trayIcon->show();
}

void MainWindow::createShortcuts()
{
	actions << ui.actionPlay
		<< ui.actionStop
		<< ui.actionNext
		<< ui.actionBack
		<< ui.actionFullscreen
		<< ui.actionMute
		<< ui.actionVolumeUp
		<< ui.actionVolumeDown
		<< ui.actionRecordNow
		<< ui.actionOpenFile
		<< ui.actionOpenUrl
		<< ui.actionOpen
		<< ui.actionEditPlaylist
		<< ui.actionSettings
		<< ui.actionTop
		<< ui.actionLite
		<< ui.actionAbout;

	shortcuts = new Shortcuts(actions);
}

void MainWindow::createSession()
{
	if(_sessionEnabled) {
		ui.volumeSlider->setVolume(_sessionVolume);
		if(_hasPlaylist)
			playChannel(_sessionChannel);
	}

	if(_updatesOnStart)
		update->getUpdates();
}

void MainWindow::createRecorder()
{
	timers = new TimersManager(time);

	if(_recorderEnabled) {
		ui.recorder->openPlaylist(_playlistName);
		ui.recorder->setGlobals(trayIcon, ui.actionRecord);
		timers->openPlaylist(_playlistName);
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
	Channel *tmp = ui.playlistWidget->channelRead(clickedChannel);
	if (tmp->isCategory() != true) {
		channel = tmp;
		play();
	}
}
void MainWindow::playChannel(const int &clickedChannel)
{
	Channel *tmp = ui.playlistWidget->channelRead(clickedChannel);
	if (tmp->isCategory() != true) {
		channel = tmp;
		play();
	}
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
	} else if(status == 0) {
		ui.actionPlay->setEnabled(true);
		ui.buttonPlay->setEnabled(true);
		ui.actionPlay->setIcon(QIcon(":/icons/images/player_play.png"));
		ui.buttonPlay->setIcon(QIcon(":/icons/images/player_play.png"));
		ui.actionPlay->setText(tr("Play"));
		ui.actionPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setStatusTip(tr("Play"));
	} else if(status == -1) {
		ui.actionPlay->setEnabled(false);
		ui.buttonPlay->setEnabled(false);
		ui.actionPlay->setText(tr("Play"));
		ui.actionPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setStatusTip(tr("Play"));
	}
}

void MainWindow::play(const QString &itemFile)
{
	this->stop();

	if(itemFile.isNull()) {
		ui.infoBarWidget->setInfo(channel->name(), channel->language());

		epg->getEpg(channel->epg());
		ui.channelNumber->display(channel->num());

		backend->openMedia(channel->url());
		tooltip(channel->name());
		trayIcon->changeToolTip(channel->name());
	} else {
		ui.infoWidget->hide();
		backend->openMedia(itemFile);
		tooltip(itemFile);
	}

	if(_videoSettings)
		ui.videoWidget->setPreviousSettings();
}

void MainWindow::stop()
{
	ui.epgToday->epgClear();
	ui.epgToday_2->epgClear();
	ui.epgToday_3->epgClear();
	ui.epgToday_4->epgClear();

	if(!_videoSettings) {
		ui.actionRatioOriginal->trigger();
		ui.actionCropOriginal->trigger();
	}

	ui.infoBarWidget->clear();
	epg->stop();
	tooltip();
	trayIcon->changeToolTip();

	controller->update();
}

void MainWindow::showEpg(const int &id, const QStringList &epgValue, const QString &date)
{
	switch (id) {
		case 0:
			ui.infoBarWidget->setEpg(epgValue.at(0), epgValue.at(1));
			break;
		case 1:
			ui.epgTabWidget->setTabText(0,date);
			ui.epgToday->setEpg(epgValue);
			break;
		case 2:
			ui.epgTabWidget->setTabText(1,date);
			ui.epgToday_2->setEpg(epgValue);
			break;
		case 3:
			ui.epgTabWidget->setTabText(2,date);
			ui.epgToday_3->setEpg(epgValue);
			break;
		case 4:
			ui.epgTabWidget->setTabText(3,date);
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
	if (!start)
	_playlistName =
		QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
						QDir::homePath(),
						tr("Tano TV Channel list Files(*.m3u)"));
	else
		_playlistName = Common::locateResource(_defaultPlaylist);

	if (!_playlistName.isEmpty()) {
		editor->setFile(_playlistName);
		if(!start) {
			ui.recorder->openPlaylist(_playlistName);
			timers->openPlaylist(_playlistName);
		}
	} else
		return;

	ui.playlistWidget->open(_playlistName);

	_hasPlaylist = true;

	if(select != 0)
		delete select;

	select = new ChannelSelect(this, ui.channelNumber, ui.playlistWidget->nums());

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
	EditSettings s(this, shortcuts);
	s.exec();
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
	rightMenu->exec(pos);
}

void MainWindow::menuOpen()
{
	openMenu->exec(QCursor::pos());
}

void MainWindow::top()
{
	Qt::WindowFlags top = flags;
	top |= Qt::WindowStaysOnTopHint;
	if(ui.actionTop->isChecked())
		this->setWindowFlags(top);
	else
		this->setWindowFlags(flags);

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
	if (!trayIcon->isVisible())
		return;

	if(this->isHidden()) {
		ui.actionTray->setText(tr("Hide to tray"));
		show();
	} else {
		ui.actionTray->setText(tr("Restore"));
		trayIcon->message(QStringList() << "close");
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
	ui.recorder->recordNow(ui.channelNumber->value(), channel->url(), channel->name());
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

// Test
void MainWindow::test()
{

}
