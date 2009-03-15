#include <QCloseEvent>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>

#include "../Common.h"
#include "Recorder.h"

Recorder::Recorder(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);
	ui.buttonRecord->setEnabled(false);

	handler = new TanoHandler(ui.playlistWidget);

	//Init
	start = true;
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
	fileName = settings.value("playlist","siol.xml").toString();
	openPlaylist();
	start = false;

	tray = new QMenu();
	//tray->addAction(ui.actionRecord);
	//tray->addSeparator();
	tray->addAction(ui.actionRestore);
	tray->addSeparator();
	tray->addAction(ui.actionClose);

	trayIcon = new TrayRecorder(tray);

	connect(trayIcon, SIGNAL(restoreClick()), this, SLOT(showNormal()));
	connect(ui.actionRestore, SIGNAL(triggered()), this, SLOT(showNormal()));

	connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(help()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openPlaylist()));
	connect(ui.actionClose, SIGNAL(triggered()), this, SLOT(closeRecorder()));

	connect(ui.playlistWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(playlist(QTreeWidgetItem*)));
}

Recorder::~Recorder()
{

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

void Recorder::about()
{
	Common::about();
}

void Recorder::help()
{
	Common::help();
}
