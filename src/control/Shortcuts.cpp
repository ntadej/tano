/****************************************************************************
* Shortcuts.cpp: Setting shortcuts on actions
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include "Shortcuts.h"
#include "../Common.h"

Shortcuts::Shortcuts(const QList<QAction *> &list)
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

	delete settings;
}
