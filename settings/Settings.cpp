#include <QTextStream>
#include <QMessageBox>
#include <QLocale>
#include <QDebug>

#include "Settings.h"

Settings::Settings(QString settingsFile) {

	fileName = settingsFile;

	read();
}

Settings::~Settings() {

}

QStringList Settings::read()
{
	QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (settings.size() > 0) return settings;
        else return settings << "Default" << "" << "";
    }

	int i = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        settings << line;
        i++;
    }
    file.close();

    return settings;
}

bool Settings::write(QStringList settingsList)
{
	QFile::remove(fileName);
	QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

	QTextStream out(&file);
	for (int i = 0; i < 3; ++i) {
		if (0 < i && settingsList[i] != "") out << "\n";
		if (settingsList[i] != "") out << settingsList[i];
	}
    file.close();
    return true;
}

QString Settings::locale()
{
	QStringList settingsL = read();
	QString locale;

	if (settingsL[0] != "Default")
	{
	   	if (settingsL[1] == "0") locale = "en";
	   	else if (settingsL[1] == "1") locale = "sl";
	}
	else locale = QString(QLocale::system().name()).replace(2, 3, "");

	qDebug() << "Tano Debug: Locale: " << locale;

	return locale;
}

QString Settings::defaultPlaylist()
{
	QStringList settingsL = read();
	QString defaultP;

	qDebug() << "Tano Debug: Settings size: " << settingsL.size();
	qDebug() << "Tano Debug: Default playlist: " << settingsL[2];

	if (settingsL.size() > 2 && settingsL[2] != "") defaultP = settingsL[2];
	else defaultP = "siol.xml";

	return defaultP;
}


