/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include "SettingsShortcuts.h"

const QStringList SettingsShortcuts::DEFAULT_SHORTCUTS_KEYS =
        QStringList() << "Space"
                      << "S"
                      << "N"
                      << "B"
                      << "F"
                      << "M"
#ifdef Q_OS_LINUX
                      << "T"
#endif
                      << "Ctrl+Up"
                      << "Ctrl+Down"
                      << "Ctrl+R"
                      << "Ctrl+E"
                      << "Ctrl+Shift+E"
                      << "Ctrl+O"
                      << "Ctrl+U"
#ifndef Q_OS_MAC
                      << "Ctrl+S"
#endif
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
                      << "Mute"
#ifdef Q_OS_LINUX
                      << "Teletext"
#endif
                      << "VolumeUp"
                      << "VolumeDown"
                      << "Recorder"
                      << "Schedule"
                      << "ScheduleCurrent"
                      << "OpenFile"
                      << "OpenUrl"
#ifndef Q_OS_MAC
                      << "Settings"
#endif
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

const QStringList SettingsShortcuts::DEFAULT_SHORTCUTS_STRINGS =
        QStringList() << tr("Play / Pause")
                      << tr("Stop")
                      << tr("Next channel")
                      << tr("Previous channel")
                      << tr("Fullscreen")
                      << tr("Mute")
#ifdef Q_OS_LINUX
                      << tr("Teletext")
#endif
                      << tr("Volume up")
                      << tr("Volume down")
                      << tr("Recorder")
                      << tr("Schedule")
                      << tr("Current show info")
                      << tr("Open file")
                      << tr("Open URL")
#ifndef Q_OS_MAC
                      << tr("Settings")
#endif
                      << tr("Always on top")
                      << tr("Simple mode")
                      << tr("Hide to tray")
                      << tr("Instant recording")
                      << tr("Take snapshot")
                      << tr("Preview channels")
                      << tr("Next audio track")
                      << tr("Next video track")
                      << tr("Next subtitle track")
                      << tr("Next aspect ratio mode")
                      << tr("Next crop mode")
                      << tr("Next scale/zoom mode")
                      << tr("Next deinterlacing mode");

