#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDateTime>
#include <QDebug>

#include "../Common.h"
#include "Recorder.h"

Recorder::Recorder(QWidget *parent)
    : QMainWindow(parent)
{
#ifdef Q_WS_WIN
	slash = "\\";
#else
	slash = "/";
#endif

	ui.setupUi(this);

	handler = new TanoHandler(ui.playlistWidget);

	//Init
	start = true;
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
	fileName = settings.value("playlist","siol.xml").toString();
	settings.beginGroup("Recorder");
	ui.fileEdit->setText(settings.value("dir",QDir::homePath()+"/Videos").toString());
	settings.endGroup();
	openPlaylist();
	start = false;

	tray = new QMenu();
	tray->addAction(ui.actionRestore);
	tray->addSeparator();
	tray->addAction(ui.actionClose);

	trayIcon = new TrayRecorder(tray);
	frip = new QProcess(this);
	fripPath = Common::frip();

    timer = new QTimer(this);

    connect(timer, SIGNAL(timeout()), this, SLOT(sec()));

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(showNormal()));
	connect(ui.actionRestore, SIGNAL(triggered()), this, SLOT(showNormal()));

	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(closeRecorder()));

	connect(ui.buttonBrowse, SIGNAL(clicked()), this, SLOT(fileBrowse()));
	connect(ui.buttonRecord, SIGNAL(toggled(bool)), this, SLOT(record(bool)));

	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));
}

Recorder::~Recorder()
{
	frip->kill();
}

void Recorder::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
    	trayIcon->message("close");
        hide();
        event->ignore();
    }
}

void Recorder::showRecorder()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
	settings.beginGroup("Recorder");
	ui.fileEdit->setText(settings.value("dir",QDir::homePath()+"/Videos").toString());
	settings.endGroup();
	this->show();
	trayIcon->show();
}

void Recorder::closeRecorder()
{
	int ret = QMessageBox::warning(this, tr("Tano Player"),
								   tr("Do you want to close Recorder?\nThis will stop any recording in progress."),
								   QMessageBox::Close | QMessageBox::Cancel,
								   QMessageBox::Close);

	switch (ret) {
		case QMessageBox::Close:
			this->hide();
			trayIcon->hide();
			ui.buttonRecord->setChecked(false);
			break;
		case QMessageBox::Cancel:
			break;
		default:
			break;
	}
}

void Recorder::openPlaylist()
{
	handler->clear();

	if (start != true) {
    	fileName =
            QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
                                         QDir::homePath(),
                                         tr("Tano TV Channel list Files(*.tano *.xml)"));
	}
	else fileName = Common::locateResource(fileName);
    if (fileName.isEmpty())
        return;

    ui.playlistWidget->clear();

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

void Recorder::playlist(QTreeWidgetItem* clickedChannel)
{
	channel = handler->channelRead(clickedChannel);
	if (channel->isCategory() != true) {
		ui.valueSelected->setText(channel->name());
	}
}

void Recorder::fileBrowse()
{
	QString dir;
	if(ui.fileEdit->text() == "")
		dir = QDir::homePath();
	else
		dir = ui.fileEdit->text();
	QString dfile = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
														dir,
														QFileDialog::ShowDirsOnly
														| QFileDialog::DontResolveSymlinks);
	ui.fileEdit->setText(dfile);
}

void Recorder::record(bool status)
{
	if(status) {
		if(ui.fileEdit->text() == "") {
			ui.buttonRecord->setChecked(false);
			return;
		}

		QFile file(QDir::tempPath()+"/tano.txt");
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
			return;

		QTextStream out(&file);
		out << "#EXTM3U" << "\n"
			<< "#EXTINF:0," << channel->name() << "\n"
			<< channel->url();

		QString fileName = ui.fileEdit->text()+slash+channel->name()+QDateTime::currentDateTime().toString("-dd_MM_yyyy-hh_mm_ss")+".avi";
		QStringList arguments;
		arguments << "-cl" << QDir::tempPath()+slash+"tano.txt"
				  << "-s"
				  << "-fi" << fileName;

		frip->start(fripPath, arguments);
		trayIcon->changeToolTip(channel->name());

		timer->start(1000);
		time = QTime(0,0);

		ui.valueCurrent->setText(channel->name());
		ui.valueTime->setText(time.toString("hh:mm:ss"));
		ui.valueRemaining->setText(tr("No timer - press button to stop."));
		ui.valueFile->setText(fileName);
	} else {
		frip->terminate();
		timer->stop();
		ui.valueCurrent->setText("-");
		ui.valueTime->setText("0");
		ui.valueRemaining->setText(tr("0"));
		ui.valueFile->setText("-");
	}
}

void Recorder::sec()
{
	time = time.addSecs(1);
	ui.valueTime->setText(time.toString("hh:mm:ss"));
}

void Recorder::about()
{
	Common::about();
}

void Recorder::help()
{
	Common::help();
}
