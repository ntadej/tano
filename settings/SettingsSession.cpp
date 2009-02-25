#include <QLocale>
#include <QDebug>

#include "SettingsSession.h"

SettingsSession::SettingsSession(QString settingsFile, QStringList defaultL)
	:Settings(settingsFile, defaultL) {

	if(size() == 1) {
		status = false;
		qDebug() << "No session";
	}
}

SettingsSession::~SettingsSession() {

}

qreal SettingsSession::volume()
{
	if(status)
		return settingsAt(0).toFloat();
}

int SettingsSession::channel()
{
	if(status)
		return settingsAt(1).toInt();
}

bool SettingsSession::ok()
{
	return status;
}


