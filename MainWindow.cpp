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
	ui.setupUi(this);
	ui.toolBarOsd->addWidget(ui.controlsWidget);
	ui.buttonRefresh->hide();
	ui.buttonReload->hide();
	ui.labelNow->hide();
	ui.labelNext->hide();
	ui.labelLanguage->hide();

	backend = new VlcInstance(ui.videoWidget->getWinId());
	flags = this->windowFlags();

	update = new Updates();
	handler = new TanoHandler(ui.playlistTree);
	epg = new Epg();
	browser = new EpgBrowser();
	epgShow = new EpgShow();

	createSettings();

	createMenus();
	createConnections();
	createShortcuts();
	createSession();
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
	if(record->isRecording()) {
		ret = QMessageBox::warning(this, tr("Tano"),
								   tr("Do you want to exit Tano?\nThis will stop recording in progress."),
								   QMessageBox::Close | QMessageBox::Cancel,
								   QMessageBox::Close);
	} else {
		ret = QMessageBox::Close;
	}

	switch (ret) {
		case QMessageBox::Close:
			record->stop();
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
    if (trayIcon->isVisible()) {
        trayIcon->message("close");
        hide();
        event->ignore();
    }
}

void MainWindow::createSettings()
{
	settings = Common::settings();

	sessionEnabled = settings->value("session", true).toBool();
	defaultP = settings->value("playlist","playlists/siol.xml").toString();

	openPlaylist(true);
	editor = new EditPlaylist(this, fileName);

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
		ui.playlistWidget->hide();
		ui.actionChannel_info->setChecked(false);
	}
	if(settings->value("wheel",false).toBool()) {
		connect(ui.videoWidget, SIGNAL(wheel(bool)), this, SLOT(volumeControl(bool)));
	} else {
		connect(ui.videoWidget, SIGNAL(wheel(bool)), select, SLOT(channel(bool)));
	}
	settings->endGroup();

	record = new Recorder();
	settings->beginGroup("Recorder");
	if(settings->value("enabled",true).toBool() && Common::fripExists()) {
		connect(ui.actionRecorder, SIGNAL(triggered()), record, SLOT(showRecorder()));
		connect(ui.actionRecord, SIGNAL(triggered()), this, SLOT(recorder()));
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
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
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
	connect(ui.actionSchedule, SIGNAL(triggered()), this, SLOT(showBrowser()));
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), editor, SLOT(show()));

	//connect(ui.actionPlay, SIGNAL(triggered()), ui.videoWidget, SLOT(controlPlay()));
	//connect(ui.actionStop, SIGNAL(triggered()), ui.videoWidget, SLOT(controlStop()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	connect(ui.actionBack, SIGNAL(triggered()), select, SLOT(back()));
	connect(ui.actionNext, SIGNAL(triggered()), select, SLOT(next()));
	//connect(ui.actionMute, SIGNAL(triggered(bool)), ui.videoWidget, SLOT(controlMute(bool)));
	connect(ui.actionMute, SIGNAL(triggered(bool)), ui.buttonMute, SLOT(setChecked(bool)));
	connect(ui.actionMute, SIGNAL(triggered(bool)), ui.volumeSlider, SLOT(setDisabled(bool)));
	connect(ui.actionVolumeUp, SIGNAL(triggered()), ui.volumeSlider, SLOT(vup()));
	connect(ui.actionVolumeDown, SIGNAL(triggered()), ui.volumeSlider, SLOT(vdown()));

	connect(ui.buttonRefresh, SIGNAL(clicked()), epg, SLOT(refresh()));
	connect(ui.buttonReload, SIGNAL(clicked()), epg, SLOT(reload()));

	connect(ui.playlistTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));
	connect(select, SIGNAL(channelSelect(int)), this, SLOT(key(int)));
	connect(select, SIGNAL(error(QString, int)), this->statusBar(), SLOT(showMessage(QString, int)));

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(showNormal()));
	connect(ui.actionRestore, SIGNAL(triggered()), this, SLOT(showNormal()));

	connect(ui.videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(rightMenu(QPoint)));
	connect(ui.videoWidget, SIGNAL(full()), ui.actionFullscreen, SLOT(trigger()));

	connect(epg, SIGNAL(epgDone(int,QStringList,QString)), this, SLOT(showEpg(int,QStringList,QString)));
	connect(ui.epgToday, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_2, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_3, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_4, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(ui.epgToday_5, SIGNAL(urlClicked(QString)), epgShow, SLOT(open(QString)));
	connect(update, SIGNAL(updatesDone(QString)), this, SLOT(processUpdates(QString)));

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), epgShow, SLOT(open(QString)));
	connect(ui.labelNext, SIGNAL(linkActivated(QString)), epgShow, SLOT(open(QString)));

	connect(ui.actionChannel_info, SIGNAL(toggled(bool)), ui.playlistWidget, SLOT(setVisible(bool)));
	connect(ui.playlistWidget, SIGNAL(visibilityChanged(bool)), ui.actionChannel_info, SLOT(setChecked(bool)));

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

	connect(right, SIGNAL(aboutToHide()), ui.videoWidget, SLOT(enableMove()));
	connect(right, SIGNAL(aboutToShow()), ui.videoWidget, SLOT(disableMove()));
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
			<< ui.actionSchedule
			<< ui.actionEditPlaylist
			<< ui.actionSettings
			<< ui.actionTop
			<< ui.actionLite
			<< ui.actionHelp
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
	channel = handler->channelRead(clickedChannel);
	if (channel->isCategory() != true) {
		play();
	}
}

void MainWindow::key(int clickedChannel)
{
	channel = handler->channelReadNum(clickedChannel);
	if (channel->isCategory() != true) {
		play();
	}
}

void MainWindow::play()
{
	ui.actionRatioOriginal->trigger();

	epg->stop();
	ui.playlistWidget->setWindowTitle(channel->name());
	ui.labelLanguage->setText(tr("Language:") + " " + channel->language());
	ui.labelLanguage->show();

	if(osdEnabled) {
		osd->setInfo(channel->name(), tr("Language:") + " " + channel->language());
		osd->setEpg(false);
	}

	epg->getEpg(channel->epg());

	ui.labelNow->hide();
	ui.labelNext->hide();
	ui.buttonRefresh->hide();
	ui.buttonReload->hide();
	ui.epgToday->epgClear();

	ui.channelNumber->display(channel->num());
	if(osdEnabled)
		osd->setNumber(channel->num());

	//ui.videoWidget->ratioOriginal();

	backend->openMedia(channel->url());
	statusBar()->showMessage(tr("Channel")+" #"+channel->numToString()+" "+tr("selected"), 2000);
}

void MainWindow::showEpg(int id, QStringList epgValue, QString date)
{
	switch (id) {
		case 0:
			ui.labelNow->setText(tr("Now:") + " " + epgValue.at(0));
			ui.labelNow->show();
			ui.labelNext->setText(tr("Next:") + " " + epgValue.at(1));
			ui.labelNext->show();
			ui.buttonRefresh->show();
			ui.buttonReload->show();

			if(osdEnabled)
				osd->setEpg(true, tr("Now:") + " " + epgValue.at(0), tr("Next:") + " " + epgValue.at(1));

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
		case 5:
			ui.epgTabWidget->setTabText(4,date);
			ui.epgToday_5->setEpg(epgValue);
			break;
		default:
			break;
	}
}

void MainWindow::stop()
{
	ui.playlistWidget->setWindowTitle(tr("Channel info"));
	ui.labelLanguage->hide();
	ui.labelNow->hide();
	ui.labelNext->hide();
	ui.buttonRefresh->hide();
	ui.buttonReload->hide();
	ui.epgToday->epgClear();
	//ui.videoWidget->ratioOriginal();
	epg->stop();
}

void MainWindow::openPlaylist(bool start)
{
	handler->clear();

	if (start != true) {
    	fileName =
            QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
                                         QDir::homePath(),
                                         tr("Tano TV Channel list Files(*.tano *.xml)"));
    	if (!fileName.isEmpty())
    		editor->setFile(fileName);
	}
	else fileName = Common::locateResource(defaultP);
    if (fileName.isEmpty())
        return;

    ui.playlistTree->clear();

    QXmlSimpleReader reader;
    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);

    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Tano"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        hasPlaylist = false;
        select = new ChannelSelect(this, ui.channelNumber, handler->limit());
        return;
    }
    QXmlInputSource xmlInputSource(&file);
    if (reader.parse(xmlInputSource))
        statusBar()->showMessage(tr("File loaded"), 2000);

    hasPlaylist = true;
    select = new ChannelSelect(this, ui.channelNumber, handler->limit());
}

void MainWindow::openFile()
{
	epg->stop();
	fileName =
        QFileDialog::getOpenFileName(this, tr("Open File or URL"),
									QDir::homePath(),
									tr("Multimedia files(*)"));

    if (fileName.isEmpty())
        return;

    backend->openMedia(fileName);
    statusBar()->showMessage(tr("Playing file"), 5000);

    ui.playlistWidget->hide();
}

void MainWindow::openUrl()
{
	epg->stop();
	bool ok;
	fileName =
		QInputDialog::getText(this, tr("Open URL or stream"),
	                                          tr("Enter the URL of multimedia file or stream you want to play:"), QLineEdit::Normal,
	                                          "", &ok);

	if (ok && !fileName.isEmpty()) {
        backend->openMedia(fileName);
		statusBar()->showMessage(tr("Playing URL"), 5000);
	    ui.playlistWidget->hide();
	}
}


//GUI
void MainWindow::showBrowser()
{
	browser->open("http://www.siol.net/tv-spored.aspx");
}

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

void MainWindow::processUpdates(QString updates)
{
	qDebug() << Common::version();
	if(Common::version() != updates) {
		if(Common::version().contains("svn")) {
			if (trayIcon->isVisible())
				trayIcon->message(updates+","+Common::version());
		} else{
			if (trayIcon->isVisible())
				trayIcon->message(updates);
		}
	} else {
		if (trayIcon->isVisible())
			trayIcon->message("latest");
	}
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
		ui.playlistWidget->show();
		ui.toolBar->show();
		ui.toolBarOsd->show();
		ui.statusbar->show();
		isLite = false;
	} else {
		ui.playlistWidget->hide();
		ui.toolBar->hide();
		ui.toolBarOsd->hide();
		ui.statusbar->hide();
		isLite = true;
	}
}

void MainWindow::time(qint64 t)
{
	int tm = t*1;
	timeNow = QTime();
	timeNow = timeNow.addMSecs(tm);
	ui.labelDuration->setText(timeNow.toString("hh:mm:ss"));
	ui.durationSlider->setValue(tm);
}

void MainWindow::totalTime(qint64 t)
{
	int tm = t*1;
	ui.durationSlider->setMaximum(tm);
	timeNow = QTime();
	timeNow = timeNow.addMSecs(tm);
	ui.labelLenght->setText(timeNow.toString("hh:mm:ss"));
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
}

void MainWindow::recorder()
{
	record->recordNow(ui.channelNumber->value(), channel->url(), channel->name());
}

void MainWindow::help()
{
	Common::help();
}
