#include <QResource>

#include "TrayRecorder.h"

TrayRecorder::TrayRecorder(QMenu *menu)
{
	QResource::registerResource("images.qrc");

	this->setContextMenu(menu);
	this->setIcon(QIcon(":/icons/images/record.png"));
	this->setToolTip(tr("Tano Recorder"));

	connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
			this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

TrayRecorder::~TrayRecorder() {
}


void TrayRecorder::iconActivated(QSystemTrayIcon::ActivationReason reason)
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

void TrayRecorder::eRestore()
{
	emit restoreClick();
}

void TrayRecorder::message(QStringList arg)
{
	if (arg.at(0) == "close")
		this->showMessage(tr("Still running"), tr("Tano Recorder is still running/recording.\nRight click to close."), QSystemTrayIcon::Information, 10000);
	else if (arg.at(0) == "record")
		this->showMessage(tr("Recording"), tr("Tano Recorder is recording %1 to\n%2.").arg(arg.at(1), arg.at(2)), QSystemTrayIcon::Information, 10000);
}

void TrayRecorder::changeToolTip(QString text)
{
	if (text != "stop")
		this->setToolTip(tr("Tano Recorder") + " - " + tr("Currently recording:") + " " + text);
	else
		this->setToolTip(tr("Tano Recorder"));
}
