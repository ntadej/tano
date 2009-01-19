#include <QFileDialog>
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
	ui.buttonToday->hide();

	handler = new TanoHandler(ui.playlistTree);
	trayIcon = new TrayIcon();
	epg = new Epg();
	epgToday = new EpgToday();

	createActions();

	defaultP = defaultPlaylist;
	openPlaylist(true);
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
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(aboutTano()));
	connect(ui.actionAbout_Qt, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	connect(ui.actionClose, SIGNAL(triggered()), qApp, SLOT(quit()));

	connect(ui.actionFullscreen, SIGNAL(triggered()), ui.videoWidget, SLOT(controlFull()));

	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionOpenFile, SIGNAL(triggered()), this, SLOT(openFile()));

	connect(ui.actionSettings, SIGNAL(triggered()), this, SLOT(settings()));
	connect(ui.actionBrowser, SIGNAL(triggered()), this, SLOT(showBrowser()));
	connect(ui.actionEditPlaylist, SIGNAL(triggered()), this, SLOT(editPlaylist()));

	connect(ui.buttonPlay, SIGNAL(clicked()), ui.videoWidget, SLOT(controlPlay()));
	connect(ui.buttonStop, SIGNAL(clicked()), ui.videoWidget, SLOT(controlStop()));
	connect(ui.buttonFull, SIGNAL(clicked()), ui.videoWidget, SLOT(controlFull()));
	connect(ui.buttonVlc, SIGNAL(clicked()), this, SLOT(vlc()));

	connect(ui.buttonRefresh, SIGNAL(clicked()), epg, SLOT(refresh()));
	connect(ui.buttonToday, SIGNAL(clicked()), epgToday, SLOT(showEpg()));

	connect(ui.playlistTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(showNormal()));
	connect(trayIcon, SIGNAL(quitClick()), qApp, SLOT(quit()));

	connect(ui.videoWidget, SIGNAL(playing(QString)), trayIcon, SLOT(changeToolTip(QString)));
	connect(ui.videoWidget, SIGNAL(stopped()), trayIcon, SLOT(changeToolTip()));
	connect(ui.videoWidget, SIGNAL(playing(QString)), this, SLOT(tooltip(QString)));
	connect(ui.videoWidget, SIGNAL(stopped()), this, SLOT(tooltip()));

	connect(epg, SIGNAL(epgDone(QString, bool)), this, SLOT(showEpg(QString, bool)));

    connect(ui.labelNow, SIGNAL(linkActivated(QString)), this, SLOT(browser(QString)));
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
		ui.epgWidget->setWindowTitle(channel->longName() + " (" + channel->name() + ")");
		ui.labelLanguage->setText(tr("Language:") + " " + channel->language());

		epg->getEpg(channel->epg());

		ui.labelNow->setText("");
		ui.buttonRefresh->hide();
		ui.buttonToday->hide();

		ui.videoWidget->playTv(channel->url(), QString(channel->longName() + " (" + channel->name() + ")"));
		statusBar()->showMessage(tr("Channel selected"), 2000);
	}
}

void Tano::showEpg(QString epgValue, bool full)
{
	if (full) {
		epgToday->setEpg(epgValue);
	}
	else {
		ui.labelNow->setText(tr("Now playing:") + " " + epgValue);
		ui.buttonRefresh->show();
		ui.buttonToday->show();
	}
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
}

void Tano::editPlaylist()
{
	EditPlaylist p(this, fileName);
	p.exec();
}

void Tano::showBrowser()
{
	browser("http://siol-tv.pfusion.co.cc");
}

void Tano::browser(QString link)
{
	EpgBrowser b(this, link);
	b.exec();
}

void Tano::settings()
{
    EditSettings s(this, Common::settingsFile());
    s.exec();
}

void Tano::vlc()
{
	vlcStatus = Common::vlcStatus();
	QStringList vlcArguments;

	qDebug() << "Tano Debug: Current media: "<< ui.videoWidget->currentMedia();

	if (ui.videoWidget->currentMedia() == "") return;

	vlcArguments << ui.videoWidget->currentMedia();
	if (vlcStatus == "") {
		QMessageBox::warning(this, tr("Tano Player"),
							tr("VLC executable can not be found."));
	} else {
		QProcess::startDetached(vlcStatus, vlcArguments);
	}
}

void Tano::tooltip(QString channelNow)
{
	if (channelNow != "stop")
		setWindowTitle(channelNow + " - " + tr("Tano Player"));
	else
		setWindowTitle(tr("Tano Player"));
}

void Tano::help()
{
	QStringList helpArguments;
	helpArguments << QLatin1String("-collectionFile")
				  << QLatin1String("doc/tano.qhc")
				  << QLatin1String("-enableRemoteControl");
    QProcess *process = new QProcess(this);
    QString app = QLibraryInfo::location(QLibraryInfo::BinariesPath)
        + QLatin1String("/assistant");

    process->start(app, helpArguments);
    if (!process->waitForStarted()) {
        QMessageBox::critical(this, tr("Tano Player"),
            tr("Could not start Qt Assistant from %1.").arg(app));
        return;
    }

    // show index page
    QTextStream str(process);
		str << QLatin1String("SetSource qthelp://tano-player.0_3/doc/index.html")
			<< QLatin1Char('\0') << endl;
}
