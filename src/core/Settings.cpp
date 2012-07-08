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
                Tano::nameGroup(),
                Tano::nameShort(),
                parent)
{
    readSettings();
}

Settings::~Settings() { }

void Settings::writeSettings()
{
    setValue("general/language", language());

#if !EDITOR
    setValue("channels/playlist", playlist());
    setValue("channels/update", playlistUpdate());
    setValue("channels/updateurl", playlistUpdateUrl());
#endif
    setValue("channels/radio", radioCategory());
    setValue("channels/udpxy", udpxy());
    setValue("channels/udpxyurl", udpxyUrl());
    setValue("channels/udpxyport", udpxyPort());

    setValue("start/splash", splash());
#if !EDITOR
    setValue("start/lite", startLite());
    setValue("start/ontop", startOnTop());
    setValue("start/controls", startControls());
    setValue("start/info", startInfo());

    setValue("gui/osd", osd());
    setValue("gui/info", info());
    setValue("gui/trayicon", trayEnabled());
    setValue("gui/hidetotray", hideToTray());
    setValue("gui/mousewheel", mouseWheel());
#endif
    setValue("gui/toolbarlook", toolbarLook());
#if !EDITOR
    setValue("gui/rememberMainSize", rememberMainSize());
    setValue("gui/mainWidth", mainWidth());
    setValue("gui/mainHeight", mainHeight());

    setValue("backend/vout", vout());
    setValue("backend/aout", aout());
#if defined(Q_OS_WIN32)
    setValue("backend/yuvToRgb", yuvToRgb());
#endif
    setValue("backend/spdif", spdif());
    setValue("backend/interfaceIndex", interfaceIndex());

    setValue("backend/videosettings", rememberVideoSettings());
    setValue("backend/aspectratio", aspectRatio());
    setValue("backend/cropratio", cropRatio());
    setValue("backend/deinterlacing", deinterlacing());
    setValue("backend/audio", audioLanguage());
    setValue("backend/subtitles", subtitleLanguage());

    setValue("backend/muteOnMinimize", muteOnMinimize());
    setValue("backend/teletext", teletext());

    setValue("recorder/directory", recorderDirectory());
    setValue("recorder/snapshots", snapshotsDirectory());

    setValue("session/autoplay", sessionAutoplay());
    setValue("session/channel", sessionChannel());
    setValue("session/remembervolume", sessionRememberVolume());
    setValue("session/volume", sessionVolume());

    setValue("xmltv/location", xmltvLocation());
    setValue("xmltv/update", xmltvUpdate());
    setValue("xmltv/updategrabber", xmltvUpdateGrabber());
    setValue("xmltv/updateurl", xmltvUpdateUrl());
    setValue("xmltv/grabber", xmltvGrabber());
    setValue("xmltv/path", xmltvGrabberPath());
#endif

    sync();
}

void Settings::readSettings()
{
    setLanguage(value("general/language", DEFAULT_LANGUAGE).toString());

#if !EDITOR
    setPlaylist(value("channels/playlist", DEFAULT_PLAYLIST).toString());
    setPlaylistUpdate(value("channels/update", DEFAULT_PLAYLIST_UPDATE).toBool());
    setPlaylistUpdateUrl(value("channels/updateurl", DEFAULT_PLAYLIST_UPDATE_URL).toString());
#endif
    setRadioCategory(value("channels/radio", DEFAULT_RADIO_CATEGORY).toString());
    setUdpxy(value("channels/udpxy", DEFAULT_UDPXY).toBool());
    setUdpxyUrl(value("channels/udpxyurl", DEFAULT_UDPXY_URL).toString());
    setUdpxyPort(value("channels/udpxyport", DEFAULT_UDPXY_PORT).toInt());

    setSplash(value("start/splash", DEFAULT_SPLASH).toBool());
#if !EDITOR
    setStartLite(value("start/lite", DEFAULT_START_LITE).toBool());
    setStartOnTop(value("start/ontop", DEFAULT_START_ON_TOP).toBool());
    setStartControls(value("start/controls", DEFAULT_START_CONTROLS).toBool());
    setStartInfo(value("start/info", DEFAULT_START_INFO).toBool());

    setOsd(value("gui/osd", DEFAULT_OSD).toBool());
    setInfo(value("gui/info", DEFAULT_INFO).toBool());
    setTrayEnabled(value("gui/trayicon", DEFAULT_TRAY_ENABLED).toBool());
    setHideToTray(value("gui/hidetotray", DEFAULT_HIDE_TO_TRAY).toBool());
    setMouseWheel(value("gui/mousewheel", DEFAULT_MOUSE_WHEEL).toString());
#endif
    setToolbarLook(value("gui/toolbarlook", DEFAULT_TOOLBAR_LOOK).toInt());
#if !EDITOR
    setRememberMainSize(value("gui/rememberMainSize", DEFAULT_REMEMBER_MAIN_SIZE).toBool());
    setMainWidth(value("gui/mainWidth", DEFAULT_MAIN_WIDTH).toInt());
    setMainHeight(value("gui/mainHeight", DEFAULT_MAIN_HEIGHT).toInt());

    setVout(value("backend/vout", DEFAULT_VOUT).toInt());
    setAout(value("backend/aout", DEFAULT_AOUT).toInt());
#if defined(Q_OS_WIN32)
    setYuvToRgb(value("backend/muteOnMinimize", DEFAULT_MUTE_ON_MINIMIZE).toBool());
#endif
    setSpdif(value("backend/spdif", DEFAULT_SPDIF).toBool());
    setInterfaceIndex(value("backend/interfaceIndex", DEFAULT_INTERFACE_INDEX).toInt());

    setRememberVideoSettings(value("backend/videosettings", DEFAULT_REMEMBER_VIDEO_SETTINGS).toBool());
    setAspectRatio(value("backend/aspectratio", DEFAULT_ASPECT_RATIO).toInt());
    setCropRatio(value("backend/cropratio", DEFAULT_CROP_RATIO).toInt());
    setDeinterlacing(value("backend/deinterlacing", DEFAULT_DEINTERLACING).toInt());
    setAudioLanguage(value("backend/audio", DEFAULT_AUDIO_LANGUAGE).toString());
    setSubtitleLanguage(value("backend/subtitles", DEFAULT_SUBTITLE_LANGUAGE).toString());

    setMuteOnMinimize(value("backend/muteOnMinimize", DEFAULT_MUTE_ON_MINIMIZE).toBool());
    setTeletext(value("backend/teletext", DEFAULT_TELETEXT).toBool());

    setRecorderDirectory(value("recorder/directory", DEFAULT_RECORDER_DIRECTORY).toString());
    setSnapshotsDirectory(value("recorder/snapshots", DEFAULT_SNAPSHOTS_DIRECTORY).toString());

    setSessionAutoplay(value("session/autoplay", DEFAULT_SESSION_AUTOPLAY).toBool());
    setSessionChannel(value("session/channel", DEFAULT_SESSION_CHANNEL).toInt());
    setSessionRememberVolume(value("session/remembervolume", DEFAULT_SESSION_REMEMBER_VOLUME).toBool());
    setSessionVolume(value("session/volume", DEFAULT_SESSION_VOLUME).toInt());

    setXmltvLocation(value("xmltv/location", DEFAULT_XMLTV_LOCATION).toString());
    setXmltvUpdate(value("xmltv/update", DEFAULT_XMLTV_UPDATE).toBool());
    setXmltvUpdateGrabber(value("xmltv/updategrabber", DEFAULT_XMLTV_UPDATE_GRABBER).toBool());
    setXmltvUpdateUrl(value("xmltv/updateurl", DEFAULT_XMLTV_UPDATE_URL).toString());
    setXmltvGrabber(value("xmltv/grabber", DEFAULT_XMLTV_GRABBER).toString());
    setXmltvGrabberPath(value("xmltv/path", DEFAULT_XMLTV_GRABBER_PATH).toString());
#endif
}
