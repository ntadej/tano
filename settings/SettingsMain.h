#ifndef SETTINGSMAIN_H_
#define SETTINGSMAIN_H_

#include "Settings.h"

class SettingsMain : public Settings {
public:
	SettingsMain(QString settingsFile, QStringList defaultL);
	virtual ~SettingsMain();

	QString locale();
	QString defaultPlaylist();
};

#endif /* SETTINGSMAIN_H_ */
