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

#include "core/Shortcuts.h"

Shortcuts::Shortcuts(const QList<QAction *> &list,
					 QObject *parent)
	: QSettings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Shortcuts", parent),
	_actions(list)
{
	_defaultList << "Space"
				 << "S"
				 << "N"
				 << "P"
				 << "F"
				 << "I"
				 << "C"
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
				 << "Ctrl+Shift+R"
				 << "Ctrl+Shift+C"
				 << "Ctrl+Shift+D";

	_actionsName << "Play"
				 << "Stop"
				 << "Next"
				 << "Back"
				 << "Fullscreen"
				 << "ToggleInfo"
				 << "ToggleControls"
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
				 << "NextRatio"
				 << "NextCrop"
				 << "NextDeinterlace";

	apply();
}

Shortcuts::~Shortcuts() { }

void Shortcuts::apply()
{
	QStringList currentKeys = readKeys();

	for(int i=0; i < _actions.size(); i++) {
		_actions[i]->setShortcut(QKeySequence(currentKeys[i]));
		_actions[i]->setShortcutContext(Qt::ApplicationShortcut);
	}
}

QStringList Shortcuts::readKeys() const
{
	QStringList list;
	for(int i=0; i < _actions.size(); i++)
		list << value(_actionsName[i], _defaultList[i]).toString();

	return list;
}

void Shortcuts::write(const QStringList &keys)
{
	for(int i=0; i < _actions.size(); i++)
		setValue(_actionsName[i], keys[i]);

	sync();
}
