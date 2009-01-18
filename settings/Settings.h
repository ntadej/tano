#include <QFile>
#include <QString>
#include <QStringList>

#ifndef SETTINGS_H_
#define SETTINGS_H_

class Settings {
public:
	Settings(QString settingsFile);
	virtual ~Settings();

	QStringList read();
	QString locale();
	QString defaultPlaylist();
	bool write(QStringList settingsList);

private:
	QFile file;
	QString fileName;
	QStringList settings;
};

#endif /* SETTINGS_H_ */
