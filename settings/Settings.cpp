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
    while (!in.atEnd() && i < 3) {
        QString line = in.readLine();
        qDebug() << "Tano Debug: Settings size: " << settings.size();
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
		QString locale;

	if (settings[0] != "Default")
	{
	   	if (settings[1] == "0") locale = "en";
	   	else if (settings[1] == "1") locale = "sl";
	}
	else locale = QString(QLocale::system().name()).replace(2, 3, "");

	qDebug() << "Tano Debug: Locale: " << locale;

	return locale;
}

QString Settings::defaultPlaylist()
{
	QString defaultP;

	qDebug() << "Tano Debug: Settings size: " << settings.size();
	if (settings.size() < 3)
		return "siol.xml";

	qDebug() << "Tano Debug: Default playlist: " << settings[2];

	if (settings[2] != "") defaultP = settings[2];
	else defaultP = "siol.xml";

	return defaultP;
}


