#include "Shortcuts.h"
#include "../Common.h"

Shortcuts::Shortcuts(QList<QAction*> list)
	: actions(list)
{
	defaultList << "Space"
				<< "S"
				<< "N"
				<< "P"
				<< "F"
				<< "M"
				<< "Ctrl+Up"
				<< "Ctrl+Down"
				<< "Ctrl+R"
				<< "Ctrl+O"
				<< "Ctrl+U"
				<< "Ctrl+P"
				<< "Ctrl+E"
				<< "Ctrl+S"
				<< "Ctrl+T"
				<< "Ctrl+L"
				<< "Ctrl+F1";

	actionsName << "Play"
				<< "Stop"
				<< "Next"
				<< "Back"
				<< "Fullscreen"
				<< "Mute"
				<< "VolumeUp"
				<< "VolumeDown"
				<< "Record"
				<< "OpenFile"
				<< "OpenUrl"
				<< "OpenPlaylist"
				<< "EditPlaylist"
				<< "Settings"
				<< "Top"
				<< "Lite"
				<< "About";

	apply();
}

Shortcuts::~Shortcuts() {

}

void Shortcuts::apply()
{
	settings = Common::settings();
	settings->sync();

	settings->beginGroup("Shortcuts");
	for (int i=0; i < actions.size(); i++) {
		actions.at(i)->setShortcut(QKeySequence(settings->value(actionsName.at(i),defaultList.at(i)).toString()));
		actions.at(i)->setShortcutContext(Qt::ApplicationShortcut);
	}
	settings->endGroup();
}
