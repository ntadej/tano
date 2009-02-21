#include <QFile>
#include <QString>
#include <QStringList>

#ifndef SETTINGS_H_
#define SETTINGS_H_

class Settings {
public:
	Settings(QString settingsFile, QStringList defaultL);
	virtual ~Settings();

	QStringList read();
	bool write(QStringList settingsList);
	QString settingsAt(int i);
	int size();

private:
	QFile file;
	QString fileName;
	QStringList settings;
	QStringList defaultList;
};

#endif /* SETTINGS_H_ */
