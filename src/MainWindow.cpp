#include <QResource>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QSettings>
#include <QDebug>

#include "MainWindow.h"
#include "Common.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	QResource::registerResource("images.qrc");

	ui.setupUi(this);
	ui.toolBarOsd->addWidget(ui.controlsWidget);
	ui.menuSubtitles->clear();
	ui.menuAudio_channel->clear();
	//this->statusBar()->hide();

#ifdef TANO_DEINTERLACING
	ui.menuDeinterlacing->setEnabled(false);
#endif

	//errorHandler = QErrorMessage::qtHandler();

	settings = Common::settings();

	backend = new VlcInstance(false, ui.videoWidget->getWinId(), settings->value("network","").toString());
	backend->init();

	controller = new VlcControl();
	flags = this->windowFlags();

	update = new Updates();
	epg = new Epg();
	epgShow = new EpgShow();

	editor = new EditPlaylist(this);

	timers = new TimersManager();

	createSettings();

	createMenus();
	createConnections();
	createShortcuts();
	createSession();

	if(settings->value("updates",true).toBool())
		update->getUpdates();
}

MainWindow::~MainWindow()
{
	if(sessionEnabled) {
		QSettings session(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
		session.beginGroup("Session");
        session.setValue("volume", ui.volumeSlider->value());
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
	if(settings->value("tray",false).toBool()) {
		if (trayIcon->isVisible()) {
			trayIcon->message(QStringList() << "close");
			hide();
			event->ignore();
		}
	}
}

void MainWindow::createSettings()
{
	sessionEnabled = settings->value("session", true).toBool();
	defaultP = settings->value("playlist","playlists/siol-mpeg2.m3u").toString();

	openPlaylist(true);

	settings->beginGroup("GUI");
	if(settings->value("lite",false).toBool()) {
		lite();
	} else {
		isLite = false;
	}
	if(settings->value("ontop",false).toBool()) {
		ui.actionTop->trigger();
		top();
	}
	if(settings->value("OSD",true).toBool()) {
		osdEnabled = true;
		osd = new Osd();
		createOsd();
	} else {
		osdEnabled = false;
	}
	if(!settings->value("info",true).toBool()) {
		ui.infoWidget->hide();
		ui.actionChannel_info->setChecked(false);
	}
	if(settings->value("wheel",false).toBool()) {
		connect(ui.videoWidget, SIGNAL(wheel(bool)), this, SLOT(volumeControl(bool)));
	} else {
		connect(ui.videoWidget, SIGNAL(wheel(bool)), select, SLOT(channel(bool)));
	}
	settings->endGroup();

	settings->beginGroup("Recorder");
	if(settings->value("enabled",true).toBool() && Common::fripExists()) {
		connect(ui.actionRecorder, SIGNAL(triggered(bool)), this, SLOT(recorder(bool)));
		connect(ui.actionRecord, SIGNAL(triggered()), this, SLOT(recordNow()));
	} else {
		ui.buttonRecord->hide();
		ui.menuMedia->removeAction(ui.actionRecord);
		ui.menuFile->removeAction(ui.actionRecorder);
		ui.toolBar->removeAction(ui.actionRecorder);
		if(osdEnabled)
			osd->disableRecorder();
	}
	settings->endGroup();
}

void MainWindow::createConnections()
{
	connect(ui.actionUpdate, SIGNAL(triggered()), update, SLOT(getUpdates()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(exit()));

	connect(ui.actionTop, SIGNAL(triggered()), this, SLOT(top()));
	connect(ui.actionLite, SIGNAL(triggered()), this, SLOT(lite()));

	connect(ui.actionFullscreen, SIGNAL(triggered()), ui.videoWidget, SLOT(controlFull()));

	connect(ui.actionOpenToolbar, SIGNAL(triggered()), this, SLOT(menuOpen()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), editor, SLOT(open()));
	connect(ui.actionTimers, SIGNAL(triggered()), timers, SLOT(show()));

	connect(ui.actionPlay, SIGNAL(triggered()), backend, SLOT(pause()));
	connect(ui.actionStop, SIGNAL(triggered()), backend, SLOT(stop()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	connect(ui.actionBack, SIGNAL(triggered()), select, SLOT(back()));
	connect(ui.actionNext, SIGNAL(triggered()), select, SLOT(next()));
	connect(ui.actionMute, SIGNAL(triggered()), backend, SLOT(mute()));
	connect(ui.actionVolumeUp, SIGNAL(triggered()), ui.volumeSlider, SLOT(vup()));
	connect(ui.actionVolumeDown, SIGNAL(triggered()), ui.volumeSlider, SLOT(vdown()));

	connect(ui.infoBarWidget, SIGNAL(refresh()), epg, SLOT(refresh()));
	connect(ui.infoBarWidget, SIGNAL(open(QString)), epgShow, SLOT(open(QString)));

	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));
	connect(select, SIGNAL(channelSelect(int)), this, SLOT(key(int)));
	//connect(select, SIGNAL(error(QString, int)), this->statusBar(), SLOT(showMessage(QString, int)));

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(showNormal()));
	connect(ui.actionRestore, SIGNAL(triggered()), this, SLOT(showNormal()));

	connect(ui.videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(rightMenu(QPoint)));
	connect(ui.videoWidget, SIGNAL(full()), ui.actionFullscreen, SLOT(trigger()));

	connect(epg, SIGNAL(epgDone(int,QStringList,QString)), this, SLOT(showEpg(int,QStringList,QString)));
	connect(ui.epgToday, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_2, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_3, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_4, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(update, SIGNAL(updatesDone(QStringList)), trayIcon, SLOT(message(QStringList)));

	connect(ui.actionChannel_info, SIGNAL(toggled(bool)), ui.infoWidget, SLOT(setVisible(bool)));
	connect(ui.infoWidget, SIGNAL(visibilityChanged(bool)), ui.actionChannel_info, SLOT(setChecked(bool)));

	connect(ui.actionRatioOriginal, SIGNAL(triggered()), ui.videoWidget, SLOT(setRatioOriginal()));
	connect(ui.actionRatio1_1, SIGNAL(triggered()), ui.videoWidget, SLOT(setRatio1_1()));
	connect(ui.actionRatio4_3, SIGNAL(triggered()), ui.videoWidget, SLOT(setRatio4_3()));
	connect(ui.actionRatio16_9, SIGNAL(triggered()), ui.videoWidget, SLOT(setRatio16_9()));
	connect(ui.actionRatio16_10, SIGNAL(triggered()), ui.videoWidget, SLOT(setRatio16_10()));
	connect(ui.actionRatio2_21_1, SIGNAL(triggered()), ui.videoWidget, SLOT(setRatio2_21_1()));
	connect(ui.actionRatio5_4, SIGNAL(triggered()), ui.videoWidget, SLOT(setRatio5_4()));

	connect(ui.actionCropOriginal, SIGNAL(triggered()), ui.videoWidget, SLOT(setCropOriginal()));
	connect(ui.actionCrop1_1, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop1_1()));
	connect(ui.actionCrop4_3, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop4_3()));
	connect(ui.actionCrop16_9, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop16_9()));
	connect(ui.actionCrop16_10, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop16_10()));
	connect(ui.actionCrop1_85_1, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop1_85_1()));
	connect(ui.actionCrop2_21_1, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop2_21_1()));
	connect(ui.actionCrop2_35_1, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop2_35_1()));
	connect(ui.actionCrop2_39_1, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop2_39_1()));
	connect(ui.actionCrop5_4, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop5_4()));
	connect(ui.actionCrop5_3, SIGNAL(triggered()), ui.videoWidget, SLOT(setCrop5_3()));

#ifndef TANO_DEINTERLACING
	connect(ui.actionFilterDisabled, SIGNAL(triggered()), ui.videoWidget, SLOT(setFilterDisabled()));
	connect(ui.actionFilterDiscard, SIGNAL(triggered()), ui.videoWidget, SLOT(setFilterDiscard()));
	connect(ui.actionFilterBlend, SIGNAL(triggered()), ui.videoWidget, SLOT(setFilterBlend()));
	connect(ui.actionFilterMean, SIGNAL(triggered()), ui.videoWidget, SLOT(setFilterMean()));
	connect(ui.actionFilterBob, SIGNAL(triggered()), ui.videoWidget, SLOT(setFilterBob()));
	connect(ui.actionFilterLinear, SIGNAL(triggered()), ui.videoWidget, SLOT(setFilterLinear()));
	connect(ui.actionFilterX, SIGNAL(triggered()), ui.videoWidget, SLOT(setFilterX()));
#endif

	connect(right, SIGNAL(aboutToHide()), ui.videoWidget, SLOT(enableMove()));
	connect(right, SIGNAL(aboutToShow()), ui.videoWidget, SLOT(disableMove()));

	connect(controller, SIGNAL(vlcAction(QString, QList<QAction*>)), this, SLOT(processMenu(QString, QList<QAction*>)));
	connect(controller, SIGNAL(stateChanged(int)), this, SLOT(playingState(int)));
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

	right = new QMenu();
	right->addAction(ui.actionPlay);
	right->addAction(ui.actionStop);
	right->addAction(ui.actionBack);
	right->addAction(ui.actionNext);
	right->addSeparator();
	right->addAction(ui.actionTop);
	right->addAction(ui.actionLite);
	right->addAction(ui.actionFullscreen);
	right->addSeparator();
	right->addMenu(ui.menuVolume);
	right->addMenu(ui.menuVideo);
	right->addSeparator();
	right->addAction(ui.actionClose);

	open = new QMenu();
	open->addAction(ui.actionOpenFile);
	open->addAction(ui.actionOpenUrl);
	open->addAction(ui.actionOpen);

	tray = new QMenu();
	tray->addAction(ui.actionPlay);
	tray->addAction(ui.actionStop);
	tray->addAction(ui.actionBack);
	tray->addAction(ui.actionNext);
	tray->addSeparator();
	tray->addAction(ui.actionRestore);
	tray->addSeparator();
	tray->addAction(ui.actionClose);

	trayIcon = new TrayIcon(tray);
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
			<< ui.actionRecord
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
	if(sessionEnabled) {
		settings->beginGroup("Session");
        ui.volumeSlider->setValue(settings->value("volume",50).toString().toInt());
		if(hasPlaylist)
			key(settings->value("channel",1).toInt());
		settings->endGroup();
	}
}

void MainWindow::aboutTano()
{
	Common::about();
}

//Media controls
void MainWindow::playlist(QTreeWidgetItem* clickedChannel)
{
	Channel *tmp = ui.playlistWidget->channelRead(clickedChannel);
	if (tmp->isCategory() != true) {
		channel = tmp;
		play();
	}
}

void MainWindow::key(int clickedChannel)
{
	Channel *tmp = ui.playlistWidget->channelReadNum(clickedChannel);
	if (tmp->isCategory() != true) {
		channel = tmp;
		play();
	}
}

void MainWindow::playingState(int status)
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

void MainWindow::play(QString itemFile, QString itemType)
{
	this->stop();

	if(itemFile.isNull()) {
		ui.infoBarWidget->setInfo(channel->name(), channel->language());

		if(osdEnabled) {
			osd->setNumber(channel->num());
			osd->setInfo(channel->name(), channel->language());
		}

		epg->getEpg(channel->epg());

		ui.channelNumber->display(channel->num());

		backend->openMedia(channel->url());
		controller->update();
		tooltip(channel->name());
		trayIcon->changeToolTip(channel->name());
		//statusBar()->showMessage(tr("Channel")+" #"+channel->numToString()+" "+tr("selected"), 2000);
	} else if(itemType == "file") {
	    ui.infoWidget->hide();
	    backend->openMedia(fileName);
		controller->update();
		tooltip(fileName);
		//statusBar()->showMessage(tr("Playing file"), 5000);
	} else {
		ui.infoWidget->hide();
		backend->openMedia(fileName);
    	controller->update();
    	tooltip(fileName);
		//statusBar()->showMessage(tr("Playing URL"), 5000);

	}
}

void MainWindow::showEpg(int id, QStringList epgValue, QString date)
{
	switch (id) {
		case 0:
			ui.infoBarWidget->setEpg(epgValue.at(0), epgValue.at(1));

			if(osdEnabled)
				osd->setEpg(true, epgValue.at(0), epgValue.at(1));

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

void MainWindow::stop()
{
	ui.epgToday->epgClear();
	ui.epgToday_2->epgClear();
	ui.epgToday_3->epgClear();
	ui.epgToday_4->epgClear();
	ui.actionRatioOriginal->trigger();
	ui.actionCropOriginal->trigger();
	ui.infoBarWidget->clear();
	if(osdEnabled) {
		osd->setInfo();
		osd->setEpg(false);
	}
	epg->stop();
	tooltip();
	trayIcon->changeToolTip();
}

void MainWindow::openPlaylist(bool start)
{
	if (!start)
    	fileName =
            QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
                                         QDir::homePath(),
                                         tr("Tano TV Channel list Files(*.m3u)"));
	else
		fileName = Common::locateResource(defaultP);

	if (!fileName.isEmpty()) {
	    editor->setFile(fileName);
	    ui.recorder->openPlaylist(fileName);
	    timers->openPlaylist(fileName);
	} else
        return;

	ui.playlistWidget->open(fileName);

    hasPlaylist = true;
    select = new ChannelSelect(this, ui.channelNumber, ui.playlistWidget->nums());
    ui.channelToolBox->setItemText(0,ui.playlistWidget->name());
}

void MainWindow::openFile()
{
	fileName =
        QFileDialog::getOpenFileName(this, tr("Open File or URL"),
									QDir::homePath(),
									tr("Multimedia files(*)"));

    if (fileName.isEmpty())
        return;

    play(fileName, "file");
}

void MainWindow::openUrl()
{
	bool ok;
	fileName =
		QInputDialog::getText(this, tr("Open URL or stream"),
							 tr("Enter the URL of multimedia file or stream you want to play:"),
							 QLineEdit::Normal, "", &ok);

	if (!ok && fileName.isEmpty())
		return;

	play(fileName,"url");
}


//GUI
void MainWindow::showSettings()
{
    EditSettings s(this, shortcuts);
    s.exec();
}

void MainWindow::tooltip(QString channelNow)
{
	if (channelNow != "stop")
		setWindowTitle(channelNow + " - " + tr("Tano"));
	else
		setWindowTitle(tr("Tano"));
}

void MainWindow::rightMenu(QPoint pos)
{
	right->exec(pos);
}

void MainWindow::menuOpen()
{
	open->exec(QCursor::pos());
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
	if(isLite) {
		ui.infoWidget->show();
		ui.toolBar->show();
		ui.toolBarOsd->show();
		isLite = false;
	} else {
		ui.infoWidget->hide();
		ui.toolBar->hide();
		ui.toolBarOsd->hide();
		isLite = true;
	}
}

void MainWindow::volumeControl(bool type)
{
	if(type)
		ui.actionVolumeUp->trigger();
	else
		ui.actionVolumeDown->trigger();
}

void MainWindow::createOsd()
{
	connect(osd, SIGNAL(play()), ui.actionPlay, SLOT(trigger()));
	connect(osd, SIGNAL(stop()), ui.actionStop, SLOT(trigger()));
	connect(osd, SIGNAL(back()), ui.actionBack, SLOT(trigger()));
	connect(osd, SIGNAL(next()), ui.actionNext, SLOT(trigger()));
	connect(osd, SIGNAL(mute()), ui.actionMute, SLOT(trigger()));

	connect(ui.actionMute, SIGNAL(triggered(bool)), osd, SLOT(setMuted(bool)));

	connect(ui.videoWidget, SIGNAL(full()), osd, SLOT(hideOsd()));
	connect(ui.videoWidget, SIGNAL(mouseMove()), osd, SLOT(showOsd()));
	connect(ui.videoWidget, SIGNAL(osd(bool)), osd, SLOT(setStatus(bool)));

	connect(osd, SIGNAL(linkActivated(QString)), epgShow, SLOT(open(QString)));
	connect(osd, SIGNAL(linkActivated(QString)), epgShow, SLOT(open(QString)));

	connect(controller, SIGNAL(stateChanged(int)), osd, SLOT(playingState(int)));
}

void MainWindow::recordNow()
{
	ui.recorder->recordNow(ui.channelNumber->value(), channel->url(), channel->name());
}

void MainWindow::recorder(bool enabled)
{
	if(enabled) {
		ui.stackedWidget->setCurrentIndex(1);
		ui.infoWidget->setVisible(false);
		ui.toolBarOsd->setVisible(false);
	} else {
		ui.stackedWidget->setCurrentIndex(0);
		ui.infoWidget->setVisible(true);
		ui.toolBarOsd->setVisible(true);
	}
}

void MainWindow::processMenu(QString type, QList<QAction*> list)
{
	if(type == "sub")
		ui.menuSubtitles->clear();
	else if(type == "audio")
		ui.menuAudio_channel->clear();

	for (int i = 0; i < list.size(); ++i) {
		if(type == "sub")
			ui.menuSubtitles->addAction(list.at(i));
		else if(type == "audio")
			ui.menuAudio_channel->addAction(list.at(i));
	}
}
