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

void TrayRecorder::message(QString type)
{
	if (type == "close")
		this->showMessage(tr("Still running"), tr("Tano Recorder is still running/recording.\nRight click to close."), QSystemTrayIcon::Information, 10000);
}

void TrayRecorder::changeToolTip(QString text)
{
	if (text != "stop")
		this->setToolTip(tr("Tano Recorder") + " - " + tr("Currently recording:") + " " + text);
	else
		this->setToolTip(tr("Tano Recorder"));
}
