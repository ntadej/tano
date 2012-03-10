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

#include "core/Common.h"
#include "core/Settings.h"

Settings::Settings(QObject *parent)
    : QSettings(QSettings::IniFormat,
                QSettings::UserScope,
                "Tano",
                "Main",
                parent)
{
    readSettings();
}

Settings::~Settings() { }

void Settings::writeSettings()
{
    setValue("general/version", configurationVersion());
    setValue("general/configured", configured());
    setValue("general/sessionvolume", sessionVolume());
    setValue("general/sessionplay", sessionAutoplay());
    setValue("general/language", language());

    setValue("channels/playlist", playlist());
    setValue("channels/update", playlistUpdate());
    setValue("channels/updateurl", playlistUpdateUrl());
    setValue("channels/radio", radioCategory());
    setValue("channels/udpxy", udpxy());
    setValue("channels/udpxyurl", udpxyUrl());
    setValue("channels/udpxyport", udpxyPort());

    setValue("start/splash", splash());
    setValue("start/lite", startLite());
    setValue("start/ontop", startOnTop());
    setValue("start/controls", startControls());
    setValue("start/info", startInfo());

    setValue("gui/osd", osd());
    setValue("gui/trayicon", trayEnabled());
    setValue("gui/hidetotray", hideToTray());
    setValue("gui/mousewheel", mouseWheel());
    setValue("gui/toolbarlook", toolbarLook());

    setValue("backend/videosettings", rememberVideoSettings());
    setValue("backend/aspectratio", aspectRatio());
    setValue("backend/cropratio", cropRatio());
    setValue("backend/deinterlacing", deinterlacing());
    setValue("backend/audio", audioLanguage());
    setValue("backend/subtitles", subtitleLanguage());

    setValue("recorder/directory", recorderDirectory());

    setValue("session/channel", channel());
    setValue("session/volume", volume());

    setValue("xmltv/location", xmltvLocation());
    setValue("xmltv/update", xmltvUpdate());
    setValue("xmltv/updategrabber", xmltvUpdateGrabber());
    setValue("xmltv/updateurl", xmltvUpdateUrl());
    setValue("xmltv/grabber", xmltvGrabber());
    setValue("xmltv/path", xmltvGrabberPath());

    sync();
}

void Settings::readSettings()
{
    setConfigurationVersion(value("general/version", Tano::version()).toString());
    setConfigured(value("general/configured", DEFAULT_CONFIGURED).toBool());
    setSessionVolume(value("general/sessionvolume", DEFAULT_SESSION_VOLUME).toBool());
    setSessionAutoplay(value("general/sessionplay", DEFAULT_SESSION_AUTOPLAY).toBool());
    setLanguage(value("general/language", DEFAULT_LANGUAGE).toString());

    setPlaylist(value("channels/playlist", DEFAULT_PLAYLIST).toString());
    setPlaylistUpdate(value("channels/update", DEFAULT_PLAYLIST_UPDATE).toBool());
    setPlaylistUpdateUrl(value("channels/updateurl", DEFAULT_PLAYLIST_UPDATE_URL).toString());
    setRadioCategory(value("channels/radio", DEFAULT_RADIO_CATEGORY).toString());
    setUdpxy(value("channels/udpxy", DEFAULT_UDPXY).toBool());
    setUdpxyUrl(value("channels/udpxyurl", DEFAULT_UDPXY_URL).toString());
    setUdpxyPort(value("channels/udpxyport", DEFAULT_UDPXY_PORT).toInt());

    setSplash(value("start/splash", DEFAULT_SPLASH).toBool());
    setStartLite(value("start/lite", DEFAULT_START_LITE).toBool());
    setStartOnTop(value("start/ontop", DEFAULT_START_ON_TOP).toBool());
    setStartControls(value("start/controls", DEFAULT_START_CONTROLS).toBool());
    setStartInfo(value("start/info", DEFAULT_START_INFO).toBool());

    setOsd(value("gui/osd", DEFAULT_OSD).toBool());
    setTrayEnabled(value("gui/trayicon", DEFAULT_TRAY_ENABLED).toBool());
    setHideToTray(value("gui/hidetotray", DEFAULT_HIDE_TO_TRAY).toBool());
    setMouseWheel(value("gui/mousewheel", DEFAULT_MOUSE_WHEEL).toString());
    setToolbarLook(value("gui/toolbarlook", DEFAULT_TOOLBAR_LOOK).toInt());

    setRememberVideoSettings(value("backend/videosettings", DEFAULT_REMEMBER_VIDEO_SETTINGS).toBool());
    setAspectRatio(value("backend/aspectratio", DEFAULT_ASPECT_RATIO).toInt());
    setCropRatio(value("backend/cropratio", DEFAULT_CROP_RATIO).toInt());
    setDeinterlacing(value("backend/deinterlacing", DEFAULT_DEINTERLACING).toInt());
    setAudioLanguage(value("backend/audio", DEFAULT_AUDIO_LANGUAGE).toString());
    setSubtitleLanguage(value("backend/subtitles", DEFAULT_SUBTITLE_LANGUAGE).toString());

    setRecorderDirectory(value("recorder/directory", DEFAULT_RECORDER_DIRECTORY).toString());

    setChannel(value("session/channel", DEFAULT_CHANNEL).toInt());
    setVolume(value("session/volume", DEFAULT_VOLUME).toInt());

    setXmltvLocation(value("xmltv/location", DEFAULT_XMLTV_LOCATION).toString());
    setXmltvUpdate(value("xmltv/update", DEFAULT_XMLTV_UPDATE).toBool());
    setXmltvUpdateGrabber(value("xmltv/updategrabber", DEFAULT_XMLTV_UPDATE_GRABBER).toBool());
    setXmltvUpdateUrl(value("xmltv/updateurl", DEFAULT_XMLTV_UPDATE_URL).toString());
    setXmltvGrabber(value("xmltv/grabber", DEFAULT_XMLTV_GRABBER).toString());
    setXmltvGrabberPath(value("xmltv/path", DEFAULT_XMLTV_GRABBER_PATH).toString());
}
