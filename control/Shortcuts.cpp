#include "Shortcuts.h"
#include "../Common.h"

Shortcuts::Shortcuts(QList<QAction*> list) {
	actions = list;

	defaultList << "Space" << "S" << "N" << "P" << "F";

	settings = new SettingsShortcuts(Common::settingsShortcutsFile(), defaultList);

	apply();
}

Shortcuts::~Shortcuts() {

}

void Shortcuts::apply()
{
	read();
	for (int i=0; i < actions.size(); i++) {
		actions.at(i)->setShortcut(QKeySequence(keys.at(i)));
		actions.at(i)->setShortcutContext(Qt::ApplicationShortcut);
	}
}

void Shortcuts::read()
{
	keys = settings->read();
}

QStringList Shortcuts::defaultKeys()
{
	return defaultList;
}
