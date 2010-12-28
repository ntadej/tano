/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
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

#include "Config.h"
#include "core/Settings.h"
#include "core/Version.h"

// Playlists
const QString Settings::PLAYLIST_SIOL_MPEG2 = "playlists/siol-mpeg2.m3u";
const QString Settings::PLAYLIST_SIOL_MPEG4 = "playlists/siol-mpeg4.m3u";
const QString Settings::PLAYLIST_T2 = "playlists/t-2.m3u";
const QString Settings::PLAYLIST_TUS = "playlists/tus.m3u";
const QString Settings::PLAYLIST_WORLDTV = "playlists/worldtv.m3u";

// Define defaults - General
const bool Settings::DEFAULT_CONFIGURED = false;
const bool Settings::DEFAULT_SESSION_VOLUME = true;
const bool Settings::DEFAULT_SESSION_AUTOPLAY = true;
const QString Settings::DEFAULT_LANGUAGE = "";
// Channels
const QString Settings::DEFAULT_PLAYLIST = Settings::PLAYLIST_SIOL_MPEG4;
// GUI - start
const bool Settings::DEFAULT_SPLASH = true;
const bool Settings::DEFAULT_START_LITE = false;
const bool Settings::DEFAULT_START_ON_TOP = false;
const bool Settings::DEFAULT_START_CONTROLS = true;
const bool Settings::DEFAULT_START_INFO = true;
// GUI
const bool Settings::DEFAULT_OSD = true;
const bool Settings::DEFAULT_HIDE_TO_TRAY = true;
const QString Settings::DEFAULT_MOUSE_WHEEL = "volume";
#if QT_OLD_VERSION
const int Settings::DEFAULT_TOOLBAR_LOOK = Qt::ToolButtonTextBesideIcon;
#else
const int Settings::DEFAULT_TOOLBAR_LOOK = Qt::ToolButtonFollowStyle;
#endif
// Backend
const bool Settings::DEFAULT_GLOBAL_SETTINGS = false;
const bool Settings::DEFAULT_REMEMBER_VIDEO_SETTINGS = false;
const QString Settings::DEFAULT_SUBTITLE_LANGUAGE = QObject::tr("Disabled");
// Recorder
const bool Settings::DEFAULT_RECORDER_ENABLED = true;
const QString Settings::DEFAULT_RECORDER_DIRECTORY = QDir::homePath();
const QString Settings::DEFAULT_RECORDER_BACKEND = "default";
// Session
const int Settings::DEFAULT_CHANNEL = 1;
const int Settings::DEFAULT_VOLUME = 50;

Settings::Settings(QObject *parent)
	: QSettings(QSettings::IniFormat, QSettings::UserScope, "Tano", "Main", parent)
{
	readSettings();
}

Settings::~Settings() { }

QString Settings::path() const
{
	return fileName().replace("Main.ini","");
}

void Settings::writeSettings()
{
	setValue("general/version", configurationVersion());
	setValue("general/configured", configured());
	setValue("general/sessionvolume", sessionVolume());
	setValue("general/sessionplay", sessionAutoplay());
	setValue("general/language", language());

	setValue("session/channel", channel());
	setValue("session/volume", volume());

	setValue("channels/playlist", playlist());

	setValue("start/splash", splash());
	setValue("start/lite", startLite());
	setValue("start/ontop", startOnTop());
	setValue("start/controls", startControls());
	setValue("start/info", startInfo());

	setValue("gui/osd", osd());
	setValue("gui/hidetotray", hideToTray());
	setValue("gui/mousewheel", mouseWheel());
	setValue("gui/toolbarlook", toolbarLook());

	setValue("backend/global", globalSettings());
	setValue("backend/videosettings", rememberVideoSettings());
	setValue("backend/language", subtitleLanguage());

	setValue("recorder/enabled", recorderEnabled());
	setValue("recorder/directory", recorderDirectory());
	setValue("recorder/backend", recorderBackend());

	sync();
}

void Settings::readSettings()
{
	setConfigurationVersion(value("general/version", Version::version()).toString());
	setConfigured(value("general/configured", DEFAULT_CONFIGURED).toBool());
	setSessionVolume(value("general/sessionvolume", DEFAULT_SESSION_VOLUME).toBool());
	setSessionAutoplay(value("general/sessionplay", DEFAULT_SESSION_AUTOPLAY).toBool());
	setLanguage(value("general/language", DEFAULT_LANGUAGE).toString());

	setPlaylist(value("channels/playlist", DEFAULT_PLAYLIST).toString());

	setSplash(value("start/splash", DEFAULT_SPLASH).toBool());
	setStartLite(value("start/lite", DEFAULT_START_LITE).toBool());
	setStartOnTop(value("start/ontop", DEFAULT_START_ON_TOP).toBool());
	setStartControls(value("start/controls", DEFAULT_START_CONTROLS).toBool());
	setStartInfo(value("start/info", DEFAULT_START_INFO).toBool());

	setOsd(value("gui/osd", DEFAULT_OSD).toBool());
	setHideToTray(value("gui/hidetotray", DEFAULT_HIDE_TO_TRAY).toBool());
	setMouseWheel(value("gui/mousewheel", DEFAULT_MOUSE_WHEEL).toString());
	setToolbarLook(value("gui/toolbarlook", DEFAULT_TOOLBAR_LOOK).toInt());

	setGlobalSettings(value("backend/global", DEFAULT_GLOBAL_SETTINGS).toBool());
	setRememberVideoSettings(value("backend/videosettings", DEFAULT_REMEMBER_VIDEO_SETTINGS).toBool());
	setSubtitleLanguage(value("backend/language", DEFAULT_SUBTITLE_LANGUAGE).toString());

	setRecorderEnabled(value("recorder/enabled", DEFAULT_RECORDER_ENABLED).toBool());
	setRecorderDirectory(value("recorder/directory", DEFAULT_RECORDER_DIRECTORY).toString());
	setRecorderBackend(value("recorder/backend", DEFAULT_RECORDER_BACKEND).toString());

	setChannel(value("session/channel", DEFAULT_CHANNEL).toInt());
	setVolume(value("session/volume", DEFAULT_VOLUME).toInt());
}
