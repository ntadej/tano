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
	slash = "/";

	ui.setupUi(this);

	handler = new TanoHandler(ui.playlistWidget);

	//Init
	start = true;
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
	fileName = settings.value("playlist","playlists/siol.xml").toString();
	settings.beginGroup("Recorder");
	ui.fileEdit->setText(settings.value("dir",QDir::homePath()+"/Videos").toString());
	settings.endGroup();
	openPlaylist();
	start = false;

	recording = false;

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

}

void Recorder::stop()
{
	frip->terminate();
}

void Recorder::closeEvent(QCloseEvent *event)
{
	QStringList arg;
	arg << "close";
    if (trayIcon->isVisible()) {
    	trayIcon->message(arg);
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
	int ret;
	if(recording) {
		ret = QMessageBox::warning(this, tr("Recorder"),
								   tr("Do you want to close Recorder?\nThis will stop any recording in progress."),
								   QMessageBox::Close | QMessageBox::Cancel,
								   QMessageBox::Close);
	} else {
		ret = QMessageBox::Close;
	}

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
        QMessageBox::warning(this, tr("Recorder"),
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
	if(dfile != "")
		ui.fileEdit->setText(dfile);
}

void Recorder::record(bool status)
{
	if(status) {
		if(ui.fileEdit->text() == "") {
			ui.buttonRecord->setChecked(false);
			return;
		} else if(!QDir(ui.fileEdit->text()).exists()) {
			ui.buttonRecord->setChecked(false);
			QMessageBox::critical(this, tr("Recorder"),
										tr("Cannot write to %1.")
										.arg(ui.fileEdit->text()));
			return;
		} else if(ui.valueSelected->text() == "-") {
			ui.buttonRecord->setChecked(false);
			QMessageBox::critical(this, tr("Recorder"),
										tr("Channel is not selected!"));
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

#ifdef Q_WS_WIN
		fileName.replace("/","\\");
#endif

		QStringList arguments;
		arguments << "-cl";

#ifdef Q_WS_WIN
		arguments << QString(QDir::tempPath()+slash+"tano.txt").replace("/","\\");
#else
		arguments << QDir::tempPath()+slash+"tano.txt";
#endif
		arguments << "-s"
				  << "-fi"
				  << fileName;

		frip->start(fripPath, arguments);

		trayIcon->changeToolTip(channel->name());

		timer->start(1000);
		time = QTime(0,0);

		ui.valueCurrent->setText(channel->name());
		ui.valueTime->setText(time.toString("hh:mm:ss"));
		ui.valueRemaining->setText(tr("No timer - press button to stop."));
		ui.valueFile->setText(fileName);

		ui.buttonRecord->setText(tr("Stop recording"));
		ui.actionRecord->setText(tr("Stop recording"));

		tray->insertAction(ui.actionRestore, ui.actionRecord);

		QStringList arg;
		arg << "record" << channel->name() << fileName;
	    if (trayIcon->isVisible()) {
	    	trayIcon->message(arg);
	    }

		recording = true;
	} else {
		frip->kill();
		timer->stop();
		ui.valueCurrent->setText("-");
		ui.valueTime->setText("0");
		ui.valueRemaining->setText(tr("0"));
		ui.valueFile->setText("-");

		ui.buttonRecord->setText(tr("Record"));
		ui.actionRecord->setText(tr("Record"));

		tray->removeAction(ui.actionRecord);

		trayIcon->changeToolTip("stop");

		recording = false;
	}
}

void Recorder::recordNow(int nmb, QString url, QString name)
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
	settings.beginGroup("Recorder");
	ui.fileEdit->setText(settings.value("dir",QDir::homePath()+"/Videos").toString());
	settings.endGroup();
	trayIcon->show();

	channel = handler->channelReadNum(nmb);
	if (channel->isCategory() != true) {
		ui.valueSelected->setText(channel->name());
		ui.actionRecord->setChecked(true);
	} else {
		QMessageBox::critical(this, tr("Recorder"),
									tr("Channel is not selected!"));
	}
}

bool Recorder::isRecording()
{
	return recording;
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
