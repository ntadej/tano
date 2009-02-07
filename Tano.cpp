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

	ui.setupUi(this);
	ui.videoControls->addWidget(ui.videoWidget->slider());
	ui.buttonRefresh->hide();
	ui.labelNow->hide();
	ui.labelLanguage->hide();

	update = new Updates();
	handler = new TanoHandler(ui.playlistTree);
	trayIcon = new TrayIcon();
	epg = new Epg();
	browser = new EpgBrowser();

	defaultP = defaultPlaylist;
	openPlaylist(true);

	editor = new EditPlaylist(parent, fileName);

	createActions();
}

Tano::~Tano()
{

}

void Tano::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        trayIcon->message("close");
        hide();
        event->ignore();
    }
}

void Tano::createActions()
{
	connect(ui.actionHelp, SIGNAL(triggered()), browser, SLOT(help()));
	connect(ui.actionUpdate, SIGNAL(triggered()), update, SLOT(getUpdates()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui.actionClose, SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(ui.actionFullscreen, SIGNAL(triggered()), ui.videoWidget, SLOT(controlFull()));

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));
	connect(ui.actionOpenUrl, SIGNAL(triggered()), this, SLOT(openUrl()));

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
	connect(ui.actionBrowser, SIGNAL(triggered()), this, SLOT(showBrowser()));
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), editor, SLOT(show()));

	connect(ui.buttonPlay, SIGNAL(clicked()), ui.videoWidget, SLOT(controlPlay()));
	connect(ui.buttonStop, SIGNAL(clicked()), ui.videoWidget, SLOT(controlStop()));
	connect(ui.buttonStop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui.buttonFull, SIGNAL(clicked()), ui.videoWidget, SLOT(controlFull()));

	connect(ui.buttonRefresh, SIGNAL(clicked()), epg, SLOT(refresh()));

	connect(ui.playlistTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(showNormal()));
	connect(trayIcon, SIGNAL(quitClick()), qApp, SLOT(quit()));

	connect(ui.videoWidget, SIGNAL(playing(QString)), trayIcon, SLOT(changeToolTip(QString)));
	connect(ui.videoWidget, SIGNAL(stopped()), trayIcon, SLOT(changeToolTip()));
	connect(ui.videoWidget, SIGNAL(playing(QString)), this, SLOT(tooltip(QString)));
	connect(ui.videoWidget, SIGNAL(stopped()), this, SLOT(tooltip()));

	connect(epg, SIGNAL(epgDone(QString, bool)), this, SLOT(showEpg(QString, bool)));
	connect(ui.epgToday, SIGNAL(urlClicked(QString)), browser, SLOT(open(QString)));
	connect(update, SIGNAL(updatesDone(QString)), this, SLOT(processUpdates(QString)));

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), browser, SLOT(open(QString)));

	connect(ui.playlistWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(actionShow(bool)));
}

void Tano::aboutTano()
{
	QMessageBox::about(this, tr("About Tano Player"),
						QString("<h2>Tano Player<br>") +
						QString(version + " (" + build + ")</h2>") +
						QString("<p>" + tr("Copyright &copy; 2008-2009 Tadej Novak") + "<p>") +
#ifdef Q_WS_WIN
						QString("MPlayer Backend<br>") +
#endif
						QString("Crystal Icons &copy; The Yellow Icon."));
}

void Tano::playlist(QTreeWidgetItem* clickedChannel)
{
	channel = handler->channelRead(clickedChannel);
	if (channel->isCategory() != true) {
		ui.playlistWidget->setWindowTitle(channel->longName() + " (" + channel->name() + ")");
		ui.labelLanguage->setText(tr("Language:") + " " + channel->language());
		ui.labelLanguage->show();

		epg->getEpg(channel->epg());

		ui.labelNow->hide();
		ui.buttonRefresh->hide();
		ui.epgToday->epgClear();

		ui.videoWidget->playTv(channel->url(), QString(channel->longName() + " (" + channel->name() + ")"));
		statusBar()->showMessage(tr("Channel selected"), 2000);
	}
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
}

void Tano::openPlaylist(bool start)
{
	if (start != true) {
    	fileName =
            QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
                                         QDir::homePath(),
                                         tr("Tano TV Channel list Files(*.tano *.xml)"));
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

void Tano::showBrowser()
{
	browser->open("http://siol-tv.pfusion.co.cc");
}

void Tano::settings()
{
    EditSettings s(this, Common::settingsFile());
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
		if (trayIcon->isVisible())
	        trayIcon->message(updates);
	} else {
		if (trayIcon->isVisible())
			trayIcon->message("latest");
	}
}

void Tano::actionShow(bool status)
{
    if(status != true)
    	ui.actionChannel_info->setEnabled(true);
    else
    	ui.actionChannel_info->setDisabled(true);
}
