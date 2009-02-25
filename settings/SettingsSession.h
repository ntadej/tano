#ifndef SETTINGSSESSION_H_
#define SETTINGSSESSION_H_

#include "Settings.h"

class SettingsSession : public Settings {
public:
	SettingsSession(QString settingsFile, QStringList defaultL);
	virtual ~SettingsSession();

	qreal volume();
	int channel();
	bool ok();

private:
	bool status;
};

#endif /* SETTINGSSESSION_H_ */
