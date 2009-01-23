#include <QResource>

#include "TrayIcon.h"

TrayIcon::TrayIcon()
{
	QResource::registerResource("images.qrc");

	actionRestore = new QAction(QIcon(":/icons/images/win.png"), tr("Restore"), this);
	actionQuit = new QAction(QIcon(":/icons/images/exit.png"), tr("Exit Tano Player"), this);
	connect(actionRestore, SIGNAL(triggered()), this, SLOT(eRestore()));
	connect(actionQuit, SIGNAL(triggered()), this, SLOT(eQuit()));

	trayIconMenu = new QMenu();
	trayIconMenu->addAction(actionRestore);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(actionQuit);

	this->setContextMenu(trayIconMenu);
	this->setIcon(QIcon(":/icons/images/tano.png"));
	this->setToolTip(tr("Tano Player"));

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

	this->show();

}

TrayIcon::~TrayIcon() {
}


void TrayIcon::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        eRestore();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void TrayIcon::eRestore()
{
	emit restoreClick();
}

void TrayIcon::eQuit()
{
	emit quitClick();
}

void TrayIcon::message(QString type)
{
	if (type == "close")
		this->showMessage(tr("Still running"), tr("Tano Player is still running.\nRight click to exit."), QSystemTrayIcon::Information, 10000);
	else if (type == "latest")
			this->showMessage(tr("Latest version"), tr("You are using the latest version of Tano Player."), QSystemTrayIcon::Information, 10000);
	else
		this->showMessage(tr("Update available"), tr("A new version of Tano Player is available!")+"\n" + tr("Version:") + " " + type, QSystemTrayIcon::Information, 10000);
}

void TrayIcon::changeToolTip(QString text)
{
	if (text != "stop")
		this->setToolTip(tr("Tano Player") + " - " + tr("Currently playing:") + " " + text);
	else
		this->setToolTip(tr("Tano Player"));
}
