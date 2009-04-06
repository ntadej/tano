#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QSettings>

#include "Tano.h"
#include "Common.h"

Tano::Tano(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	ui.toolBarOsd->addWidget(ui.controlsWidget);
	ui.buttonRefresh->hide();
	ui.buttonReload->hide();
	ui.labelNow->hide();
	ui.labelNext->hide();
	ui.labelLanguage->hide();

	flags = this->windowFlags();

	update = new Updates();
	handler = new TanoHandler(ui.playlistTree);
	epg = new Epg();
	browser = new EpgBrowser();

	createSettings();

	createMenus();
	createConnections();
	createShortcuts();
}

Tano::~Tano()
{
	if(sessionEnabled) {
		QSettings session(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
		session.beginGroup("Session");
		session.setValue("volume", ui.videoWidget->volume());
		session.setValue("channel", ui.channelNumber->value());
		session.endGroup();
	}
}

void Tano::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        trayIcon->message("close");
        hide();
        event->ignore();
    }
}

void Tano::createSettings()
{
	settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");

	sessionEnabled = settings->value("session", true).toBool();
	defaultP = settings->value("playlist","siol.xml").toString();

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
		osd = new TanoOsd();
		createOsd();
	} else {
		osdEnabled = false;
	}
	settings->endGroup();

	settings->beginGroup("Recorder");
	if(settings->value("enabled",true).toBool()) {
		record = new Recorder();
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

	if(sessionEnabled) {
		settings->beginGroup("Session");
		ui.videoWidget->setVolume(settings->value("volume",0.5).toString().toFloat());
		ui.volumeSlider->setValue(settings->value("volume",0.5).toString().toFloat()*100);
		key(settings->value("channel",1).toInt());
		settings->endGroup();
	}
}

void Tano::createConnections()
{
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
	connect(ui.actionUpdate, SIGNAL(triggered()), update, SLOT(getUpdates()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui.actionClose, SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(ui.actionTop, SIGNAL(triggered()), this, SLOT(top()));
	connect(ui.actionLite, SIGNAL(triggered()), this, SLOT(lite()));

	connect(ui.actionFullscreen, SIGNAL(triggered()), ui.videoWidget, SLOT(controlFull()));

	connect(ui.actionOpenToolbar, SIGNAL(triggered()), this, SLOT(menuOpen()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(showSettings()));
	connect(ui.actionBrowser, SIGNAL(triggered()), this, SLOT(showBrowser()));
	connect(ui.actionEpg, SIGNAL(triggered()), this, SLOT(showSiolEpg()));
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), editor, SLOT(show()));

	connect(ui.actionPlay, SIGNAL(triggered()), ui.videoWidget, SLOT(controlPlay()));
	connect(ui.actionStop, SIGNAL(triggered()), ui.videoWidget, SLOT(controlStop()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	connect(ui.actionBack, SIGNAL(triggered()), select, SLOT(back()));
	connect(ui.actionNext, SIGNAL(triggered()), select, SLOT(next()));
	connect(ui.actionMute, SIGNAL(triggered(bool)), ui.videoWidget, SLOT(controlMute(bool)));
	connect(ui.actionMute, SIGNAL(triggered(bool)), ui.buttonMute, SLOT(setChecked(bool)));
	connect(ui.actionMute, SIGNAL(triggered(bool)), ui.volumeSlider, SLOT(setDisabled(bool)));
	connect(ui.actionVolumeUp, SIGNAL(triggered()), ui.videoWidget, SLOT(controlVUp()));
	connect(ui.actionVolumeDown, SIGNAL(triggered()), ui.videoWidget, SLOT(controlVDown()));

	connect(ui.volumeSlider, SIGNAL(valueChanged(int)), ui.videoWidget, SLOT(controlVolume(int)));

	connect(ui.videoWidget, SIGNAL(volumeChanged(int)), ui.volumeSlider, SLOT(setValue(int)));
	connect(ui.durationSlider, SIGNAL(sliderMoved(int)), ui.videoWidget, SLOT(controlDuration(int)));

	connect(ui.buttonRefresh, SIGNAL(clicked()), epg, SLOT(refresh()));
	connect(ui.buttonReload, SIGNAL(clicked()), epg, SLOT(reload()));

	connect(ui.playlistTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));
	connect(select, SIGNAL(channelSelect(int)), this, SLOT(key(int)));
	connect(select, SIGNAL(error(QString, int)), this->statusBar(), SLOT(showMessage(QString, int)));

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(showNormal()));
	connect(ui.actionRestore, SIGNAL(triggered()), this, SLOT(showNormal()));

	connect(ui.videoWidget, SIGNAL(playing(QString)), trayIcon, SLOT(changeToolTip(QString)));
	connect(ui.videoWidget, SIGNAL(stopped()), trayIcon, SLOT(changeToolTip()));
	connect(ui.videoWidget, SIGNAL(playing(QString)), this, SLOT(tooltip(QString)));
	connect(ui.videoWidget, SIGNAL(stopped()), this, SLOT(tooltip()));
	connect(ui.videoWidget, SIGNAL(rightClick(QPoint)), this, SLOT(rightMenu(QPoint)));
	connect(ui.videoWidget, SIGNAL(wheel(bool)), select, SLOT(channel(bool)));
	connect(ui.videoWidget, SIGNAL(full()), ui.actionFullscreen, SLOT(trigger()));
	connect(ui.videoWidget, SIGNAL(tick(qint64)), this, SLOT(time(qint64)));
	connect(ui.videoWidget, SIGNAL(totalTimeChanged(qint64)), this, SLOT(totalTime(qint64)));

	connect(epg, SIGNAL(epgDone(QStringList)), this, SLOT(showEpg(QStringList)));
	connect(epg, SIGNAL(epgDoneFull(QStringList)), ui.epgToday, SLOT(setEpg(QStringList)));
	connect(ui.epgToday, SIGNAL(urlClicked(QString)), browser, SLOT(open(QString)));
	connect(update, SIGNAL(updatesDone(QString)), this, SLOT(processUpdates(QString)));

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), browser, SLOT(open(QString)));
	connect(ui.labelNext, SIGNAL(linkActivated(QString)), browser, SLOT(open(QString)));

	connect(ui.actionChannel_info, SIGNAL(triggered()), this, SLOT(actionChannelShow()));
	connect(ui.actionToolbar, SIGNAL(triggered()), this, SLOT(actionToolbarShow()));

	connect(ui.actionRatioOriginal, SIGNAL(triggered()), ui.videoWidget, SLOT(ratioOriginal()));
	connect(ui.actionRatio43, SIGNAL(triggered()), ui.videoWidget, SLOT(ratio43()));
	connect(ui.actionRatio169, SIGNAL(triggered()), ui.videoWidget, SLOT(ratio169()));

	connect(right, SIGNAL(aboutToHide()), ui.videoWidget, SLOT(enableMove()));
	connect(right, SIGNAL(aboutToShow()), ui.videoWidget, SLOT(disableMove()));
}

void Tano::createMenus()
{
	ratioGroup = new QActionGroup(this);
	ratioGroup->addAction(ui.actionRatioOriginal);
	ratioGroup->addAction(ui.actionRatio43);
	ratioGroup->addAction(ui.actionRatio169);

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
	right->addMenu(ui.menuRatio);
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

void Tano::createShortcuts()
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
			<< ui.actionBrowser
			<< ui.actionEditPlaylist
			<< ui.actionSettings
			<< ui.actionTop
			<< ui.actionLite
			<< ui.actionHelp
			<< ui.actionAbout;

	shortcuts = new Shortcuts(actions);
}

void Tano::aboutTano()
{
	Common::about();
}

//Media controls
void Tano::playlist(QTreeWidgetItem* clickedChannel)
{
	channel = handler->channelRead(clickedChannel);
	if (channel->isCategory() != true) {
		play();
	}
}

void Tano::key(int clickedChannel)
{
	channel = handler->channelReadNum(clickedChannel);
	if (channel->isCategory() != true) {
		play();
	}
}

void Tano::play()
{
	epg->stop();
	ui.playlistWidget->setWindowTitle(channel->longName() + " (" + channel->name() + ")");
	ui.labelLanguage->setText(tr("Language:") + " " + channel->language());
	ui.labelLanguage->show();

	epg->getEpg(channel->epg());

	ui.labelNow->hide();
	ui.labelNext->hide();
	ui.buttonRefresh->hide();
	ui.buttonReload->hide();
	ui.epgToday->epgClear();

	ui.channelNumber->display(channel->num());
	if(osdEnabled)
		osd->setNumber(channel->num());

	ui.videoWidget->ratioOriginal();
	ui.videoWidget->playTv(channel->url(), QString(channel->longName() + " (" + channel->name() + ")"));
	statusBar()->showMessage(tr("Channel")+" #"+channel->numToString()+" "+tr("selected"), 2000);
}

void Tano::showEpg(QStringList epgValue)
{
	ui.labelNow->setText(tr("Now:") + " " + epgValue.at(0));
	ui.labelNow->show();
	ui.labelNext->setText(tr("Next:") + " " + epgValue.at(1));
	ui.labelNext->show();
	ui.buttonRefresh->show();
	ui.buttonReload->show();
}

void Tano::stop()
{
	ui.playlistWidget->setWindowTitle(tr("Channel info"));
	ui.labelLanguage->hide();
	ui.labelNow->hide();
	ui.labelNext->hide();
	ui.buttonRefresh->hide();
	ui.buttonReload->hide();
	ui.epgToday->epgClear();
	ui.videoWidget->ratioOriginal();
	epg->stop();
}

void Tano::openPlaylist(bool start)
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
        QMessageBox::warning(this, tr("Tano Player"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }
    QXmlInputSource xmlInputSource(&file);
    if (reader.parse(xmlInputSource))
        statusBar()->showMessage(tr("File loaded"), 2000);

    select = new ChannelSelect(this, ui.channelNumber, handler->limit());
}

void Tano::openFile()
{
	epg->stop();
	fileName =
        QFileDialog::getOpenFileName(this, tr("Open File or URL"),
									QDir::homePath(),
									tr("Multimedia files(*)"));

    if (fileName.isEmpty())
        return;

    ui.videoWidget->playTv(fileName, fileName);
    statusBar()->showMessage(tr("Playing file"), 5000);

    ui.playlistWidget->hide();
}

void Tano::openUrl()
{
	epg->stop();
	bool ok;
	fileName =
		QInputDialog::getText(this, tr("Open URL or stream"),
	                                          tr("Enter the URL of multimedia file or stream you want to play:"), QLineEdit::Normal,
	                                          "", &ok);

	if (ok && !fileName.isEmpty()) {
		ui.videoWidget->playTv(fileName, fileName);
		statusBar()->showMessage(tr("Playing URL"), 5000);
	    ui.playlistWidget->hide();
	}
}


//GUI
void Tano::showBrowser()
{
	browser->open("http://tano.pfusion.co.cc");
}

void Tano::showSiolEpg()
{
	browser->open("http://tano.pfusion.co.cc/siol/");
}

void Tano::showSettings()
{
    EditSettings s(this, shortcuts);
    s.exec();
}

void Tano::tooltip(QString channelNow)
{
	if (channelNow != "stop")
		setWindowTitle(channelNow + " - " + tr("Tano Player"));
	else
		setWindowTitle(tr("Tano Player"));
}

void Tano::processUpdates(QString updates)
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

void Tano::actionChannelShow()
{
    if(ui.playlistWidget->isVisible())
    	ui.playlistWidget->hide();
    else
    	ui.playlistWidget->show();
}

void Tano::actionToolbarShow()
{
    if(ui.toolBar->isVisible())
    	ui.toolBar->hide();
    else
    	ui.toolBar->show();
}

void Tano::rightMenu(QPoint pos)
{
	right->exec(pos);
}

void Tano::menuOpen()
{
	open->exec(QCursor::pos());
}

void Tano::top()
{
	Qt::WindowFlags top = flags;
	top |= Qt::WindowStaysOnTopHint;
	if(ui.actionTop->isChecked())
		this->setWindowFlags(top);
	else
		this->setWindowFlags(flags);

	this->show();
}

void Tano::lite()
{
	if(isLite) {
		ui.centralLayout-> setContentsMargins(4,4,4,4);
		ui.playlistWidget->show();
		ui.toolBar->show();
		//ui.menubar->show();
		ui.statusbar->show();
		isLite = false;
	} else {
		ui.centralLayout-> setContentsMargins(0,0,0,0);
		ui.playlistWidget->hide();
		ui.toolBar->hide();
		//ui.menubar->hide(); Causes shortcuts not working
		ui.statusbar->hide();
		isLite = true;
	}
}

void Tano::time(qint64 t) {
	int tm = t*1;
	timeNow = QTime();
	timeNow = timeNow.addMSecs(tm);
	ui.labelDuration->setText(timeNow.toString("hh:mm:ss"));
	ui.durationSlider->setValue(tm);
}

void Tano::totalTime(qint64 t) {
	int tm = t*1;
	ui.durationSlider->setMaximum(tm);
	timeNow = QTime();
	timeNow = timeNow.addMSecs(tm);
	ui.labelLenght->setText(timeNow.toString("hh:mm:ss"));
}

void Tano::createOsd()
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

	connect(osd, SIGNAL(volume(int)), ui.volumeSlider, SLOT(setValue(int)));

	connect(ui.videoWidget, SIGNAL(tick(qint64)), osd, SLOT(setDuration(qint64)));
	connect(ui.videoWidget, SIGNAL(totalTimeChanged(qint64)), osd, SLOT(setLenght(qint64)));
	connect(osd, SIGNAL(seek(int)), ui.videoWidget, SLOT(controlDuration(int)));

	connect(ui.volumeSlider, SIGNAL(valueChanged(int)), osd, SLOT(setVolume(int)));
}

void Tano::recorder()
{
	//record->show();

	QMessageBox::warning(this, tr("Tano Player"),
	                     tr("Recorder is not working at the moment!"));
}

void Tano::help()
{
	Common::help();
}
