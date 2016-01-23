/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include "settings/Settings.h"

// Define keys - General
const QString Settings::KEY_LANGUAGE = "session/language";

// Playlist
const QString Settings::KEY_PLAYLIST = "channels/playlist";
const QString Settings::KEY_PLAYLIST_UPDATE = "channels/update";
const QString Settings::KEY_PLAYLIST_UPDATE_URL = "channels/updateurl";
const QString Settings::KEY_RADIO_CATEGORY = "channels/radio";
const QString Settings::KEY_HD_CATEGORY = "channels/hd";
const QString Settings::KEY_UDPXY = "channels/udpxy";
const QString Settings::KEY_UDPXY_URL = "channels/udpxyurl";
const QString Settings::KEY_UDPXY_PORT = "channels/udpxyport";

// GUI - start
const QString Settings::KEY_WIDTH = "gui/width";
const QString Settings::KEY_HEIGHT = "gui/height";
const QString Settings::KEY_POS_X = "gui/posx";
const QString Settings::KEY_POS_Y = "gui/posy";

// GUI
const QString Settings::KEY_OSD = "gui/osd";
const QString Settings::KEY_TRAY_ENABLED = "gui/trayicon";
const QString Settings::KEY_HIDE_TO_TRAY = "gui/hidetotray";
const QString Settings::KEY_MOUSE_WHEEL = "gui/mousewheel";
const QString Settings::KEY_REMEMBER_GUI_SESSION = "gui/session";
const QString Settings::KEY_ICONS = "gui/icons";

// Backend - core
const QString Settings::KEY_VOUT = "backend/vout";
const QString Settings::KEY_AOUT = "backend/aout";
const QString Settings::KEY_YUV_TO_RGB = "backend/yuvToRgb";
const QString Settings::KEY_SPDIF = "backend/spdif";

// Backend - video settings
const QString Settings::KEY_REMEMBER_VIDEO_SETTINGS = "backend/videosettings";
const QString Settings::KEY_REMEMBER_VIDEO_PER_CHANNEL = "backend/perchannel";
const QString Settings::KEY_ASPECT_RATIO = "backend/aspectratio";
const QString Settings::KEY_CROP_RATIO = "backend/cropratio";
const QString Settings::KEY_DEINTERLACING = "backend/deQStringerlacing";
const QString Settings::KEY_AUDIO_LANGUAGE = "backend/audio";
const QString Settings::KEY_SUBTITLE_LANGUAGE = "backend/subtitles";

// Backend - misc
const QString Settings::KEY_MUTE_ON_MINIMIZE = "backend/muteOnMinimize";
const QString Settings::KEY_TELETEXT = "backend/teletext";

// Recorder
const QString Settings::KEY_RECORDER_DIRECTORY = "recorder/directory";
const QString Settings::KEY_SNAPSHOTS_DIRECTORY = "recorder/snapshots";

// Session
const QString Settings::KEY_SESSION_AUTOPLAY = "session/autoplay";
const QString Settings::KEY_SESSION_CHANNEL = "session/channel";
const QString Settings::KEY_SESSION_REMEMBER_VOLUME = "session/remembervolume";
const QString Settings::KEY_SESSION_VOLUME = "session/volume";

// Schedule
const QString Settings::KEY_XMLTV_UPDATE = "xmltv/update";
const QString Settings::KEY_XMLTV_UPDATE_LOCATION = "xmltv/updatelocation";
const QString Settings::KEY_XMLTV_UPDATE_REMOTE = "xmltv/updateremote";
const QString Settings::KEY_XMLTV_HOURS_OFFSET = "xmltv/offset";
