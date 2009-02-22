#ifndef SHORTCUTS_H_
#define SHORTCUTS_H_

#include <QAction>
#include <QList>
#include <QStringList>

#include "../settings/SettingsShortcuts.h"

class Shortcuts {
public:
	Shortcuts(QList<QAction*> list);
	virtual ~Shortcuts();

	void apply();
	QStringList defaultKeys();

private:
	void read();

	QList<QAction*> actions;
	QStringList defaultList;
	QStringList keys;

	SettingsShortcuts *settings;
};

#endif /* SHORTCUTS_H_ */
