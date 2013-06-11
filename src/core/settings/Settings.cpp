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

#include "Config.h"

#include "Common.h"
#include "settings/Settings.h"

Settings::Settings(QObject *parent)
    : QSettings(QSettings::IniFormat,
                QSettings::UserScope,
                Tano::name(),
                Tano::name(),
                parent),
      _disableInterface(false),
      _disablePlayback(false),
      _disablePlaylist(false),
      _disableRecorder(false),
      _disableSchedule(false)
{
    readSettings();

#if BRANDING
    _disableInterface = !bool(SETTINGS_INTERFACE);
    _disablePlayback = !bool(SETTINGS_PLAYBACK);
    _disablePlaylist = !bool(SETTINGS_PLAYLIST);
    _disableRecorder = !bool(SETTINGS_RECORDER);
    _disableSchedule = !bool(SETTINGS_SCHEDULE);
#endif
}

Settings::~Settings() { }

void Settings::writeSettings()
{
    setValue("general/language", language());

    setValue("session/autoplay", sessionAutoplay());
    setValue("session/channel", sessionChannel());
    setValue("session/remembervolume", sessionRememberVolume());
    setValue("session/volume", sessionVolume());

    if (!_disablePlaylist) {
        setValue("channels/playlist", playlist());
        setValue("channels/update", playlistUpdate());
        setValue("channels/updateurl", playlistUpdateUrl());
        setValue("channels/radio", radioCategory());
        setValue("channels/hd", hdCategory());
        setValue("channels/udpxy", udpxy());
        setValue("channels/udpxyurl", udpxyUrl());
        setValue("channels/udpxyport", udpxyPort());
    } else {
        remove("channels");
    }

    if (!_disableInterface) {
        setValue("start/lite", startLite());
        setValue("start/ontop", startOnTop());
        setValue("start/controls", startControls());
        setValue("start/info", startInfo());
        setValue("start/width", width());
        setValue("start/height", height());
        setValue("start/posx", posX());
        setValue("start/posy", posY());

        setValue("gui/osd", osd());
        setValue("gui/info", info());
        setValue("gui/trayicon", trayEnabled());
        setValue("gui/hidetotray", hideToTray());
        setValue("gui/mousewheel", mouseWheel());
        setValue("gui/toolbarlook", toolbarLook());
        setValue("gui/session", rememberGuiSession());
        setValue("gui/filters", filtersVisible());
        setValue("gui/icons", icons());
    } else {
        remove("start");
        remove("gui");
    }

    if (!_disablePlayback) {
        setValue("backend/vout", vout());
        setValue("backend/aout", aout());
#if defined(Q_OS_WIN32)
        setValue("backend/yuvToRgb", yuvToRgb());
#endif
        setValue("backend/spdif", spdif());
        setValue("backend/interfaceIndex", interfaceIndex());

        setValue("backend/videosettings", rememberVideoSettings());
        setValue("backend/perchannel", rememberVideoPerChannel());
        setValue("backend/aspectratio", aspectRatio());
        setValue("backend/cropratio", cropRatio());
        setValue("backend/deinterlacing", deinterlacing());
        setValue("backend/audio", audioLanguage());
        setValue("backend/subtitles", subtitleLanguage());

        setValue("backend/muteOnMinimize", muteOnMinimize());
        setValue("backend/teletext", teletext());
    } else {
        remove("backend");
    }

    if (!_disableRecorder) {
        setValue("recorder/directory", recorderDirectory());
        setValue("recorder/snapshots", snapshotsDirectory());
    } else {
        remove("recorder");
    }

    if (!_disableSchedule) {
        setValue("xmltv/update", xmltvUpdate());
        setValue("xmltv/updatelocation", xmltvUpdateLocation());
        setValue("xmltv/updateremote", xmltvUpdateRemote());
    } else {
        remove("schedule");
    }

    sync();
}

void Settings::readSettings()
{
    setLanguage(value("general/language", DEFAULT_LANGUAGE).toString());

    setPlaylist(value("channels/playlist", DEFAULT_PLAYLIST).toString());
    setPlaylistUpdate(value("channels/update", DEFAULT_PLAYLIST_UPDATE).toBool());
    setPlaylistUpdateUrl(value("channels/updateurl", DEFAULT_PLAYLIST_UPDATE_URL).toString());
    setRadioCategory(value("channels/radio", DEFAULT_RADIO_CATEGORY).toString());
    setHdCategory(value("channels/hd", DEFAULT_HD_CATEGORY).toString());
    setUdpxy(value("channels/udpxy", DEFAULT_UDPXY).toBool());
    setUdpxyUrl(value("channels/udpxyurl", DEFAULT_UDPXY_URL).toString());
    setUdpxyPort(value("channels/udpxyport", DEFAULT_UDPXY_PORT).toInt());

    setStartLite(value("start/lite", DEFAULT_START_LITE).toBool());
    setStartOnTop(value("start/ontop", DEFAULT_START_ON_TOP).toBool());
    setStartControls(value("start/controls", DEFAULT_START_CONTROLS).toBool());
    setStartInfo(value("start/info", DEFAULT_START_INFO).toBool());
    setWidth(value("start/width", DEFAULT_WIDTH).toInt());
    setHeight(value("start/height", DEFAULT_HEIGHT).toInt());
    setPosX(value("start/posx", DEFAULT_POS_X).toInt());
    setPosY(value("start/posy", DEFAULT_POS_Y).toInt());

    setOsd(value("gui/osd", DEFAULT_OSD).toBool());
    setInfo(value("gui/info", DEFAULT_INFO).toBool());
    setTrayEnabled(value("gui/trayicon", DEFAULT_TRAY_ENABLED).toBool());
    setHideToTray(value("gui/hidetotray", DEFAULT_HIDE_TO_TRAY).toBool());
    setMouseWheel(value("gui/mousewheel", DEFAULT_MOUSE_WHEEL).toString());
    setToolbarLook(value("gui/toolbarlook", DEFAULT_TOOLBAR_LOOK).toInt());
    setRememberGuiSession(value("gui/session", DEFAULT_REMEMBER_GUI_SESSION).toBool());
    setFiltersVisible(value("gui/filters", DEFAULT_FILTERS_VISIBLE).toBool());
    setIcons(value("gui/icons", DEFAULT_ICONS).toString());

    setVout(value("backend/vout", DEFAULT_VOUT).toInt());
    setAout(value("backend/aout", DEFAULT_AOUT).toInt());
#if defined(Q_OS_WIN32)
    setYuvToRgb(value("backend/muteOnMinimize", DEFAULT_MUTE_ON_MINIMIZE).toBool());
#endif
    setSpdif(value("backend/spdif", DEFAULT_SPDIF).toBool());
    setInterfaceIndex(value("backend/interfaceIndex", DEFAULT_INTERFACE_INDEX).toInt());

    setRememberVideoSettings(value("backend/videosettings", DEFAULT_REMEMBER_VIDEO_SETTINGS).toBool());
    setRememberVideoPerChannel(value("backend/perchannel", DEFAULT_REMEMBER_VIDEO_PER_CHANNEL).toBool());
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

    setXmltvUpdate(value("xmltv/update", DEFAULT_XMLTV_UPDATE).toBool());
    setXmltvUpdateLocation(value("xmltv/updatelocation", DEFAULT_XMLTV_UPDATE_LOCATION).toString());
    setXmltvUpdateRemote(value("xmltv/updateremote", DEFAULT_XMLTV_UPDATE_REMOTE).toBool());
}
