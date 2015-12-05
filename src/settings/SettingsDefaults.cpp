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

#include <QtCore/QDir>
#include <QtCore/QStandardPaths>

#include "common/Resources.h"
#include "Settings.h"

#ifdef Q_OS_LINUX
    #include "platform/Linux.h"
#endif

// Define defaults - General
const QString Settings::DEFAULT_LOCALE = "";
const bool Settings::DEFAULT_AUTOPLAY_LAST = false;
const bool Settings::DEFAULT_MUTE_ON_MINIMIZE = false;
const QString Settings::DEFAULT_RECORDINGS_DIRECTORY = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
const QString Settings::DEFAULT_SNAPSHOTS_DIRECTORY = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);

// Session
const int Settings::DEFAULT_SESSION_CHANNEL = 1;
const int Settings::DEFAULT_SESSION_VOLUME = 50;

// Channels
const QString Settings::DEFAULT_CHANNELS_LIST = "";
const QString Settings::DEFAULT_RADIO_CATEGORY = "Radio";
const QString Settings::DEFAULT_HD_CATEGORY = "HD";
const bool Settings::DEFAULT_UDPXY = false;
const QString Settings::DEFAULT_UDPXY_URL = "";
const int Settings::DEFAULT_UDPXY_PORT = 1234;

// GUI - start
const int Settings::DEFAULT_WIDTH = 650;
const int Settings::DEFAULT_HEIGHT = 300;
const int Settings::DEFAULT_POS_X = 0;
const int Settings::DEFAULT_POS_Y = 0;

// GUI
#ifndef Q_OS_MAC
const bool Settings::DEFAULT_TRAY_ENABLED = true;
const bool Settings::DEFAULT_HIDE_TO_TRAY = true;
#endif
const QString Settings::DEFAULT_MOUSE_WHEEL = "volume";

// Backend - core
const int Settings::DEFAULT_VOUT = 0;
#ifdef Q_OS_WIN
const bool Settings::DEFAULT_YUV_TO_RGB = true;
#endif
const bool Settings::DEFAULT_SPDIF = false;
#ifdef Q_OS_LINUX
const bool Settings::DEFAULT_TELETEXT = false;
#endif

// Backend - video settings
const bool Settings::DEFAULT_REMEMBER_VIDEO_SETTINGS = false;
const bool Settings::DEFAULT_REMEMBER_VIDEO_PER_CHANNEL = false;
const int Settings::DEFAULT_ASPECT_RATIO = 0;
const int Settings::DEFAULT_CROP_RATIO = 0;
const int Settings::DEFAULT_DEINTERLACING = 0;
const QString Settings::DEFAULT_AUDIO_LANGUAGE = QObject::tr("Disabled");
const QString Settings::DEFAULT_SUBTITLE_LANGUAGE = QObject::tr("Disabled");

// Schedule
const QString Settings::DEFAULT_XMLTV_UPDATE_LOCATION = "";
const bool Settings::DEFAULT_XMLTV_UPDATE_REMOTE = false;
const int Settings::DEFAULT_XMLTV_HOURS_OFFSET = 0;
