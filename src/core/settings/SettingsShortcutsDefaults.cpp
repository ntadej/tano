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

#include "Config.h"
#include "core/settings/SettingsShortcuts.h"

const QStringList SettingsShortcuts::DEFAULT_SHORTCUTS_KEYS =
        QStringList() << "Space"
                      << "S"
                      << "N"
                      << "B"
                      << "F"
                      << "I"
                      << "C"
                      << "M"
#if TELETEXT
                      << "T"
#endif
                      << "Ctrl+Up"
                      << "Ctrl+Down"
                      << "Ctrl+R"
                      << "Ctrl+O"
                      << "Ctrl+U"
                      << "Ctrl+Shift+O"
                      << "Ctrl+E"
                      << "Ctrl+S"
                      << "Ctrl+K"
                      << "Ctrl+T"
                      << "Ctrl+L"
                      << "Ctrl+H"
                      << "Ctrl+I"
                      << "Ctrl+Shift+I"
                      << "Ctrl+P"
                      << "Ctrl+Shift+A"
                      << "Ctrl+Shift+V"
                      << "Ctrl+Shift+S"
                      << "Ctrl+Shift+R"
                      << "Ctrl+Shift+C"
                      << "Ctrl+Shift+Z"
                      << "Ctrl+Shift+D";

const QStringList SettingsShortcuts::DEFAULT_SHORTCUTS_ACTIONS =
        QStringList() << "Play"
                      << "Stop"
                      << "Next"
                      << "Back"
                      << "Fullscreen"
                      << "ToggleInfo"
                      << "ToggleControls"
                      << "Mute"
#if TELETEXT
                      << "Teletext"
#endif
                      << "VolumeUp"
                      << "VolumeDown"
                      << "Recorder"
                      << "OpenFile"
                      << "OpenUrl"
                      << "OpenPlaylist"
                      << "EditPlaylist"
                      << "Settings"
                      << "SettingsSettingsShortcuts"
                      << "Top"
                      << "Lite"
                      << "Tray"
                      << "RecordNow"
                      << "Snapshot"
                      << "Preview"
                      << "NextAudioTrack"
                      << "NextVideoTrack"
                      << "NextSubtitleTrack"
                      << "NextRatio"
                      << "NextCrop"
                      << "NextScale"
                      << "NextDeinterlace";
