#include "TrayIcon.h"

TrayIcon::TrayIcon(QMenu *menu, bool recorder)
	: _recorder(recorder)
{
	this->setContextMenu(menu);

	if(!_recorder) {
		this->setIcon(QIcon(":/icons/images/tano.png"));
		this->setToolTip(tr("Tano"));
	} else {
		this->setIcon(QIcon(":/icons/images/record.png"));
		this->setToolTip(tr("Tano Recorder"));
	}

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

TrayIcon::~TrayIcon()
{

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

void TrayIcon::message(QStringList arg)
{
	if (arg.at(0) == "record")
		this->showMessage(tr("Recording"), tr("Tano Recorder is recording %1 to\n%2.").arg(arg.at(1), arg.at(2)), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "close")
		this->showMessage(tr("Still running"), tr("Tano is still running.\nRight click to exit."), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "latest")
		this->showMessage(tr("Latest version"), tr("You are using the latest version of Tano."), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "svn")
		this->showMessage(tr("SVN"), tr("You are using SVN version:")+" "+arg.at(2)+"\n" + tr("Stable version:") + " " + arg.at(1), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "update")
		this->showMessage(tr("Update available"), tr("A new version of Tano is available!")+"\n" + tr("Version:") + " " + arg.at(1), QSystemTrayIcon::Information, 10000);
}

void TrayIcon::changeToolTip(QString text)
{
	if(text != "stop" && !_recorder)
		this->setToolTip(tr("Tano") + " - " + tr("Currently playing:") + " " + text);
	else if(text != "stop" && _recorder)
		this->setToolTip(tr("Tano") + " - " + tr("Currently recording:") + " " + text);
	else
		this->setToolTip(tr("Tano"));
}
