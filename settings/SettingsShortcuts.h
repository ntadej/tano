#ifndef SETTINGSSHORTCUTS_H_
#define SETTINGSSHORTCUTS_H_

#include "Settings.h"

class SettingsShortcuts : public Settings{
public:
	SettingsShortcuts(QString settingsFile, QStringList defaultL);
	virtual ~SettingsShortcuts();
};

#endif /* SETTINGSSHORTCUTS_H_ */
