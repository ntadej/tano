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

#include <QtCore/QDir>

#include "core/Common.h"
#include "core/Settings.h"

// Define defaults - General
const QString Settings::DEFAULT_LANGUAGE = "";

// Playlist
const QString Settings::DEFAULT_PLAYLIST = "";
const bool Settings::DEFAULT_PLAYLIST_UPDATE = false;
const QString Settings::DEFAULT_PLAYLIST_UPDATE_URL = "";
const QString Settings::DEFAULT_RADIO_CATEGORY = "Radio";
const bool Settings::DEFAULT_UDPXY = false;
const QString Settings::DEFAULT_UDPXY_URL = "";
const int Settings::DEFAULT_UDPXY_PORT = 1234;

// GUI - start
const bool Settings::DEFAULT_SPLASH = true;
const bool Settings::DEFAULT_START_LITE = false;
const bool Settings::DEFAULT_START_ON_TOP = false;
const bool Settings::DEFAULT_START_CONTROLS = true;
const bool Settings::DEFAULT_START_INFO = true;

// GUI
const bool Settings::DEFAULT_OSD = true;
const bool Settings::DEFAULT_INFO = true;
const bool Settings::DEFAULT_TRAY_ENABLED = true;
const bool Settings::DEFAULT_HIDE_TO_TRAY = true;
const QString Settings::DEFAULT_MOUSE_WHEEL = "volume";
const int Settings::DEFAULT_TOOLBAR_LOOK = Qt::ToolButtonIconOnly;
const bool Settings::DEFAULT_REMEMBER_MAIN_SIZE = true;
const int Settings::DEFAULT_MAIN_WIDTH = 650;
const int Settings::DEFAULT_MAIN_HEIGHT = 300;

// Backend - core
const int Settings::DEFAULT_VOUT = 0;
const int Settings::DEFAULT_AOUT = 0;
#if defined(Q_OS_WIN32)
const bool Settings::DEFAULT_YUV_TO_RGB = true;
#endif
const bool Settings::DEFAULT_SPDIF = false;
const int Settings::DEFAULT_INTERFACE_INDEX = 0;

// Backend - video settings
const bool Settings::DEFAULT_REMEMBER_VIDEO_SETTINGS = false;
const int Settings::DEFAULT_ASPECT_RATIO = 0;
const int Settings::DEFAULT_CROP_RATIO = 0;
const int Settings::DEFAULT_DEINTERLACING = 0;
const QString Settings::DEFAULT_AUDIO_LANGUAGE = QObject::tr("Disabled");
const QString Settings::DEFAULT_SUBTITLE_LANGUAGE = QObject::tr("Disabled");

// Backend - misc
const bool Settings::DEFAULT_MUTE_ON_MINIMIZE = false;
const bool Settings::DEFAULT_TELETEXT = false;

// Recorder
#if defined(Q_OS_WIN32)
const QString Settings::DEFAULT_RECORDER_DIRECTORY = QDir::homePath() + "/Videos";
const QString Settings::DEFAULT_SNAPSHOTS_DIRECTORY = QDir::homePath() + "/Pictures";
#elif defined(Q_OS_MAC)
const QString Settings::DEFAULT_RECORDER_DIRECTORY = QDir::homePath() + "/Videos"; // Need to confirm
const QString Settings::DEFAULT_SNAPSHOTS_DIRECTORY = QDir::homePath() + "/Pictures";
#elif defined(Q_OS_LINUX)
const QString Settings::DEFAULT_RECORDER_DIRECTORY = QDir::homePath() + "/" + Tano::linuxVideoPath();
const QString Settings::DEFAULT_SNAPSHOTS_DIRECTORY = QDir::homePath() + "/" + Tano::linuxPicturesPath();
#else
const QString Settings::DEFAULT_RECORDER_DIRECTORY = QDir::homePath() + "/Videos";
const QString Settings::DEFAULT_SNAPSHOTS_DIRECTORY = QDir::homePath() + "/Pictures";
#endif

// Session
const bool Settings::DEFAULT_SESSION_AUTOPLAY = false;
const int Settings::DEFAULT_SESSION_CHANNEL = 1;
const bool Settings::DEFAULT_SESSION_REMEMBER_VOLUME = true;
const int Settings::DEFAULT_SESSION_VOLUME = 50;

// Schedule
const QString Settings::DEFAULT_XMLTV_LOCATION = Tano::settingsPath() + "xmltv.xml";
const bool Settings::DEFAULT_XMLTV_UPDATE = false;
const bool Settings::DEFAULT_XMLTV_UPDATE_GRABBER = false;
const QString Settings::DEFAULT_XMLTV_UPDATE_URL = "";
const QString Settings::DEFAULT_XMLTV_GRABBER = "";
const QString Settings::DEFAULT_XMLTV_GRABBER_PATH = "";

// Constants
const QString Settings::SUPPORT_ADDRESS = "info@tano.si";
