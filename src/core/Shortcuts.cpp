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
	: _actions(list)
{
	_defaultList << "Space"
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
				<< "Ctrl+H"
				<< "Ctrl+F1";

	_actionsName << "Play"
				<< "Stop"
				<< "Next"
				<< "Back"
				<< "Fullscreen"
				<< "Mute"
				<< "VolumeUp"
				<< "VolumeDown"
				<< "Recorder"
				<< "OpenFile"
				<< "OpenUrl"
				<< "OpenPlaylist"
				<< "EditPlaylist"
				<< "Settings"
				<< "Top"
				<< "Lite"
				<< "Tray"
				<< "About";

	apply();
}

Shortcuts::~Shortcuts() {

}

void Shortcuts::apply()
{
	_settings = Common::settings();
	_settings->sync();

	_settings->beginGroup("Shortcuts");
	for (int i=0; i < _actions.size(); i++) {
		_actions[i]->setShortcut(QKeySequence(_settings->value(_actionsName[i],_defaultList[i]).toString()));
		_actions[i]->setShortcutContext(Qt::ApplicationShortcut);
	}
	_settings->endGroup();

	delete _settings;
}
