#ifndef SHORTCUTS_H_
#define SHORTCUTS_H_

#include <QAction>
#include <QList>

#include "../settings/SettingsShortcuts.h"

class Shortcuts {
public:
	Shortcuts(QList<QAction*> list);
	virtual ~Shortcuts();

private:
	QList<QAction*> actions;

	SettingsShortcuts *settings;
};

#endif /* SHORTCUTS_H_ */
