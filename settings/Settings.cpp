#include <QTextStream>

#include "Settings.h"

Settings::Settings(QString settingsFile, QStringList defaultL) {

	fileName = settingsFile;
	defaultList = defaultL;

	read();
}

Settings::~Settings() {

}

QStringList Settings::read()
{
	QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (settings.size() > 0) {
        	return settings;
        } else {
        	settings = defaultList;
        	return settings;
        }
    }

    settings.clear();
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        settings << line;
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
	for (int i = 0; i < settingsList.size(); ++i) {
		if (0 < i && settingsList[i] != "") out << "\n";
		if (settingsList[i] != "") out << settingsList[i];
	}
    file.close();
    return true;
}

QString Settings::settingsAt(int i)
{
	return settings[i];
}

int Settings::size()
{
	return settings.size();
}
