#include <QAction>
#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>

#include "../Common.h"
#include "Recorder.h"

Recorder::Recorder(QWidget *parent)
	: QWidget(parent)
{
	slash = "/";

	ui.setupUi(this);

	//Init
	settings = Common::settings();
	settings->beginGroup("Recorder");
	ui.fileEdit->setText(settings->value("dir",QDir::homePath()+"/Videos").toString());
	settings->endGroup();

	recording = false;

	trayIcon = 0;
	actionRecord = 0;

	frip = new QProcess(this);
	fripPath = Common::frip();

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(sec()));

	connect(ui.buttonBrowse, SIGNAL(clicked()), this, SLOT(fileBrowse()));
	connect(ui.buttonRecord, SIGNAL(toggled(bool)), this, SLOT(record(bool)));

	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));
}

Recorder::~Recorder()
{
	delete trayIcon;
	delete frip;
	delete timer;
	delete settings;
}

void Recorder::stop()
{
	frip->terminate();
}

void Recorder::openPlaylist(const QString &file)
{
	ui.playlistWidget->open(file);
}

void Recorder::playlist(QTreeWidgetItem* clickedChannel)
{
	channel = ui.playlistWidget->channelRead(clickedChannel);
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

void Recorder::record(const bool &status)
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

		QString fileName = ui.fileEdit->text()+slash+channel->name().replace(" ","_")+QDateTime::currentDateTime().toString("-dd_MM_yyyy-hh_mm_ss")+".avi";

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

		if(trayIcon)
			trayIcon->changeToolTip(channel->name(), "recorder");

		timer->start(1000);
		time = QTime(0,0);

		ui.valueCurrent->setText(channel->name());
		ui.valueTime->setText(time.toString("hh:mm:ss"));
		ui.valueRemaining->setText(tr("No timer - press button to stop."));
		ui.valueFile->setText(fileName);

		ui.buttonRecord->setText(tr("Stop recording"));
		if(actionRecord)
			actionRecord->setEnabled(true);

		QStringList arg;
		arg << "record" << channel->name() << fileName;
		if (trayIcon && trayIcon->isVisible()) {
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
		if(actionRecord)
			actionRecord->setEnabled(false);

		trayIcon->changeToolTip("stop", "recorder");

		recording = false;
	}
}

void Recorder::recordNow(const int &nmb, const QString &url, const QString &name)
{
	settings->beginGroup("Recorder");
	ui.fileEdit->setText(settings->value("dir",QDir::homePath()+"/Videos").toString());
	settings->endGroup();

	channel = ui.playlistWidget->channelRead(nmb);
	if (channel->isCategory() != true) {
		ui.valueSelected->setText(channel->name());
		if(actionRecord)
			actionRecord->setEnabled(true);
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

void Recorder::setGlobals(TrayIcon *icon, QAction *action)
{
	trayIcon = icon;
	actionRecord = action;
	connect(actionRecord, SIGNAL(triggered()), ui.buttonRecord, SLOT(toggle()));
}

void Recorder::recordTimer(const Timer *timer)
{

}

void Recorder::stopTimer(const Timer *timer)
{

}
