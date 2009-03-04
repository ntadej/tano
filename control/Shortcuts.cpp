#include <QSettings>

#include "Shortcuts.h"
#include "../Common.h"

Shortcuts::Shortcuts(QList<QAction*> list) {
	actions = list;

	defaultList << "Space"
				<< "S"
				<< "N"
				<< "P"
				<< "F"
				<< "M"
				<< "Ctrl+Up"
				<< "Ctrl+Down"
				<< "Ctrl+O"
				<< "Ctrl+U"
				<< "Ctrl+P"
				<< "Ctrl+B"
				<< "Ctrl+E"
				<< "Ctrl+S"
				<< "Ctrl+T"
				<< "Ctrl+L"
				<< "F1"
				<< "Ctrl+F1";

	actionsName << "Play"
				<< "Stop"
				<< "Next"
				<< "Back"
				<< "Fullscreen"
				<< "Mute"
				<< "VolumeUp"
				<< "VolumeDown"
				<< "OpenFile"
				<< "OpenUrl"
				<< "OpenPlaylist"
				<< "Browser"
				<< "EditPlaylist"
				<< "Settings"
				<< "Top"
				<< "Lite"
				<< "Help"
				<< "About";

	apply();
}

Shortcuts::~Shortcuts() {

}

void Shortcuts::apply()
{
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Settings");
	settings.sync();

	settings.beginGroup("Shortcuts");
	for (int i=0; i < actions.size(); i++) {
		actions.at(i)->setShortcut(QKeySequence(settings.value(actionsName.at(i),defaultList.at(i)).toString()));
		actions.at(i)->setShortcutContext(Qt::ApplicationShortcut);
	}
	settings.endGroup();
}

QStringList Shortcuts::defaultKeys()
{
	return defaultList;
}

QStringList Shortcuts::actionsNames()
{
	return actionsName;
}
