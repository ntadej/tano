#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QDir>
#include <QScrollBar>
#include <QProcess>
#include <QLibraryInfo>

#include "Tano.h"
#include "Common.h"

Tano::Tano(QWidget *parent, QString defaultPlaylist)
    : QMainWindow(parent)
{

#ifdef TANO_VERSION
	version = QString(TANO_VERSION);
#else
	version = "Unknown version";
#endif

#ifdef TANO_BUILD
	build = QString(TANO_BUILD);
#else
	build = "Unknown build";
#endif

	isLite = false;

	ui.setupUi(this);
	ui.videoControls->addWidget(ui.videoWidget->slider());
	ui.buttonRefresh->hide();
	ui.labelNow->hide();
	ui.labelLanguage->hide();

	flags = this->windowFlags();

	update = new Updates();
	handler = new TanoHandler(ui.playlistTree);
	epg = new Epg();
	browser = new EpgBrowser();

	defaultP = defaultPlaylist;
	openPlaylist(true);

	editor = new EditPlaylist(parent, fileName);

	createMenus();
	createActions();
	createShortcuts();
	createSession();
}

Tano::~Tano()
{
	QStringList sessionList;
	QString vol;
	QString ch;

	vol.setNum(ui.videoWidget->volume());
	ch.setNum(ui.channelNumber->value());

	sessionList << vol << ch;
	session->write(sessionList);
}

void Tano::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        trayIcon->message("close");
        hide();
        event->ignore();
    }
}

void Tano::createSession()
{
	session = new SettingsSession(Common::settingsFile("session"), Common::settingsDefault("session"));
	if(session->ok()) {
		ui.videoWidget->setVolume(session->volume());
		key(session->channel());
	}
}

void Tano::createActions()
{
	connect(ui.actionHelp, SIGNAL(triggered()), browser, SLOT(help()));
	connect(ui.actionUpdate, SIGNAL(triggered()), update, SLOT(getUpdates()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui.actionClose, SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(ui.actionTop, SIGNAL(triggered()), this, SLOT(top()));
	connect(ui.actionLite, SIGNAL(triggered()), this, SLOT(lite()));

	connect(ui.actionFullscreen, SIGNAL(triggered()), ui.videoWidget, SLOT(controlFull()));

	connect(ui.actionOpenToolbar, SIGNAL(triggered()), this, SLOT(menuOpen()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
	connect(ui.actionBrowser, SIGNAL(triggered()), this, SLOT(showBrowser()));
	connect(ui.actionEpg, SIGNAL(triggered()), this, SLOT(showSiolEpg()));
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), editor, SLOT(show()));

	connect(ui.buttonPlay, SIGNAL(clicked()), ui.videoWidget, SLOT(controlPlay()));
	connect(ui.buttonStop, SIGNAL(clicked()), ui.videoWidget, SLOT(controlStop()));
	connect(ui.buttonStop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui.buttonBack, SIGNAL(clicked()), select, SLOT(back()));
	connect(ui.buttonNext, SIGNAL(clicked()), select, SLOT(next()));
	connect(ui.actionPlay, SIGNAL(triggered()), ui.videoWidget, SLOT(controlPlay()));
	connect(ui.actionStop, SIGNAL(triggered()), ui.videoWidget, SLOT(controlStop()));
	connect(ui.actionStop, SIGNAL(triggered()), this, SLOT(stop()));
	connect(ui.actionBack, SIGNAL(triggered()), select, SLOT(back()));
	connect(ui.actionNext, SIGNAL(triggered()), select, SLOT(next()));
	connect(ui.actionMute, SIGNAL(triggered()), ui.videoWidget, SLOT(controlMute()));
	connect(ui.actionVolumeUp, SIGNAL(triggered()), ui.videoWidget, SLOT(controlVUp()));
	connect(ui.actionVolumeDown, SIGNAL(triggered()), ui.videoWidget, SLOT(controlVDown()));

	connect(ui.buttonRefresh, SIGNAL(clicked()), epg, SLOT(refresh()));

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

	connect(epg, SIGNAL(epgDone(QString, bool)), this, SLOT(showEpg(QString, bool)));
	connect(ui.epgToday, SIGNAL(urlClicked(QString)), browser, SLOT(open(QString)));
	connect(update, SIGNAL(updatesDone(QString)), this, SLOT(processUpdates(QString)));

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), browser, SLOT(open(QString)));

	connect(ui.actionChannel_info, SIGNAL(triggered()), this, SLOT(actionChannelShow()));
	connect(ui.actionToolbar, SIGNAL(triggered()), this, SLOT(actionToolbarShow()));

	connect(ui.actionRatioOriginal, SIGNAL(triggered()), ui.videoWidget, SLOT(ratioOriginal()));
	connect(ui.actionRatio43, SIGNAL(triggered()), ui.videoWidget, SLOT(ratio43()));
	connect(ui.actionRatio169, SIGNAL(triggered()), ui.videoWidget, SLOT(ratio169()));
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
	tray->addAction(ui.actionTop);
	tray->addAction(ui.actionLite);
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
			<< ui.actionBack
			<< ui.actionNext
			<< ui.actionFullscreen
			<< ui.actionMute
			<< ui.actionVolumeUp
			<< ui.actionVolumeDown;

	shortcuts = new Shortcuts(actions);

	/* right->addAction(ui.actionTop);
	right->addAction(ui.actionLite); */
}

void Tano::aboutTano()
{
	QMessageBox::about(this, tr("About Tano Player"),
						QString("<h2>Tano Player<br>") +
						QString(version + " (" + build + ")</h2>") +
						QString("<p>" + tr("Copyright &copy; 2008-2009 Tadej Novak") + "<p>") +
#ifdef Q_WS_WIN
						QString("VLC Backend<br>") +
#endif
						QString("Crystal Icons &copy; The Yellow Icon."));
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
	ui.playlistWidget->setWindowTitle(channel->longName() + " (" + channel->name() + ")");
	ui.labelLanguage->setText(tr("Language:") + " " + channel->language());
	ui.labelLanguage->show();

	epg->getEpg(channel->epg());

	ui.labelNow->hide();
	ui.buttonRefresh->hide();
	ui.epgToday->epgClear();

	ui.channelNumber->display(channel->num());

	ui.videoWidget->playTv(channel->url(), QString(channel->longName() + " (" + channel->name() + ")"));
	statusBar()->showMessage(tr("Channel")+" #"+channel->numToString()+" "+tr("selected"), 2000);
}

void Tano::showEpg(QString epgValue, bool full)
{
	if (full) {
		ui.epgToday->setEpg(epgValue);
	}
	else {
		ui.labelNow->setText(tr("Now:") + " " + epgValue);
		ui.labelNow->show();
		ui.buttonRefresh->show();
	}
}

void Tano::stop()
{
	ui.playlistWidget->setWindowTitle(tr("Channel info"));
	ui.labelLanguage->hide();
	ui.labelNow->hide();
	ui.buttonRefresh->hide();
	ui.epgToday->epgClear();
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
	bool ok;
	fileName =
		QInputDialog::getText(this, tr("Open URL or stream"),
	                                          tr("Enter the URL of multimedia file or stream you want to play:"), QLineEdit::Normal,
	                                          "", &ok);

	if (ok && !fileName.isEmpty()) {
		ui.videoWidget->playTv(fileName, fileName);
		statusBar()->showMessage(tr("Playing file"), 5000);
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

void Tano::settings()
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
	qDebug() << version;
	if(version != updates) {
		if(version.contains("svn")) {
			if (trayIcon->isVisible())
				trayIcon->message(updates+","+version);
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
		ui.menubar->show();
		ui.statusbar->show();
		ui.videoControlsFrame->show();
		isLite = false;
	} else {
		ui.centralLayout-> setContentsMargins(0,0,0,0);
		ui.playlistWidget->hide();
		ui.toolBar->hide();
		ui.menubar->hide();
		ui.statusbar->hide();
		ui.videoControlsFrame->hide();
		isLite = true;
	}
}
