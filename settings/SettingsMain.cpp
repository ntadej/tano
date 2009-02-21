#include <QLocale>

#include "SettingsMain.h"

SettingsMain::SettingsMain(QString settingsFile, QStringList defaultL)
	:Settings(settingsFile, defaultL) {

}

SettingsMain::~SettingsMain() {

}

QString SettingsMain::locale()
{
	QString locale;

	if (settingsAt(0) != "Default")
	{
	   	if (settingsAt(1) == "0") locale = "en";
	   	else if (settingsAt(1) == "1") locale = "sl";
	}
	else locale = QString(QLocale::system().name()).replace(2, 3, "");

	return locale;
}

QString SettingsMain::defaultPlaylist()
{
	QString defaultP;

	if (size() < 3)
		return "siol.xml";

	if (settingsAt(2) != "") defaultP = settingsAt(2);
	else defaultP = "siol.xml";

	return defaultP;
}


