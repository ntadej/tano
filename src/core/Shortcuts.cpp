/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#if defined(Qt5)
    #include <QtWidgets/QAction>
#elif defined(Qt4)
    #include <QtGui/QAction>
#endif

#include "core/Shortcuts.h"

Shortcuts::Shortcuts(const QList<QAction *> &list,
                     QObject *parent)
    : QSettings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Shortcuts", parent),
      _actions(list)
{
    _defaultList << "Space"
                 << "S"
                 << "N"
                 << "B"
                 << "F"
                 << "I"
                 << "C"
                 << "M"
                 << "T"
                 << "Ctrl+Up"
                 << "Ctrl+Down"
                 << "Ctrl+R"
                 << "Ctrl+O"
                 << "Ctrl+U"
                 << "Ctrl+P"
                 << "Ctrl+E"
                 << "Ctrl+S"
                 << "Ctrl+K"
                 << "Ctrl+T"
                 << "Ctrl+L"
                 << "Ctrl+H"
                 << "Ctrl+Shift+A"
                 << "Ctrl+Shift+V"
                 << "Ctrl+Shift+S"
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
                 << "Teletext"
                 << "VolumeUp"
                 << "VolumeDown"
                 << "Recorder"
                 << "OpenFile"
                 << "OpenUrl"
                 << "OpenPlaylist"
                 << "EditPlaylist"
                 << "Settings"
                 << "SettingsShortcuts"
                 << "Top"
                 << "Lite"
                 << "Tray"
                 << "NextAudioTrack"
                 << "NextVideoTrack"
                 << "NextSubtitleTrack"
                 << "NextRatio"
                 << "NextCrop"
                 << "NextDeinterlace";

    apply();
}

Shortcuts::~Shortcuts() { }

void Shortcuts::apply()
{
    QStringList currentKeys = readKeys();

    for (int i = 0; i < _actions.size(); i++) {
        _actions[i]->setShortcut(QKeySequence(currentKeys[i]));
        _actions[i]->setShortcutContext(Qt::ApplicationShortcut);
    }
}

QStringList Shortcuts::readKeys() const
{
    QStringList list;
    for (int i = 0; i < _actions.size(); i++)
        list << value(_actionsName[i], _defaultList[i]).toString();

    return list;
}

void Shortcuts::write(const QStringList &keys)
{
    for (int i = 0; i < _actions.size(); i++)
        setValue(_actionsName[i], keys[i]);

    sync();
}
