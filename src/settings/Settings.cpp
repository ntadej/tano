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

#include "Config.h"

#include "application/Common.h"
#include "settings/Settings.h"

Settings::Settings(QObject *parent)
    : QSettings(parent)
{
    _map[KEY_LOCALE] = DEFAULT_LOCALE;
    _map[KEY_AUTOPLAY_LAST] = DEFAULT_AUTOPLAY_LAST;
    _map[KEY_MUTE_ON_MINIMIZE] = DEFAULT_MUTE_ON_MINIMIZE;
    _map[KEY_RECORDINGS_DIRECTORY] = DEFAULT_RECORDINGS_DIRECTORY;
    _map[KEY_SNAPSHOTS_DIRECTORY] = DEFAULT_SNAPSHOTS_DIRECTORY;
    _map[KEY_SESSION_CHANNEL] = DEFAULT_SESSION_CHANNEL;
    _map[KEY_SESSION_VOLUME] = DEFAULT_SESSION_VOLUME;
    _map[KEY_CHANNELS_LIST] = DEFAULT_CHANNELS_LIST;
    _map[KEY_RADIO_CATEGORY] = DEFAULT_RADIO_CATEGORY;
    _map[KEY_HD_CATEGORY] = DEFAULT_HD_CATEGORY;
    _map[KEY_UDPXY] = DEFAULT_UDPXY;
    _map[KEY_UDPXY_URL] = DEFAULT_UDPXY_URL;
    _map[KEY_UDPXY_PORT] = DEFAULT_UDPXY_PORT;
    _map[KEY_WIDTH] = DEFAULT_WIDTH;
    _map[KEY_HEIGHT] = DEFAULT_HEIGHT;
    _map[KEY_POS_X] = DEFAULT_POS_X;
    _map[KEY_POS_Y] = DEFAULT_POS_Y;
#ifndef Q_OS_MAC
    _map[KEY_TRAY_ENABLED] = DEFAULT_TRAY_ENABLED;
    _map[KEY_HIDE_TO_TRAY] = DEFAULT_HIDE_TO_TRAY;
#endif
    _map[KEY_MOUSE_WHEEL] = DEFAULT_MOUSE_WHEEL;
    _map[KEY_VOUT] = DEFAULT_VOUT;
#ifdef Q_OS_WIN
    _map[KEY_YUV_TO_RGB] = DEFAULT_YUV_TO_RGB;
#endif
    _map[KEY_SPDIF] = DEFAULT_SPDIF;
#ifdef Q_OS_LINUX
    _map[KEY_TELETEXT] = DEFAULT_TELETEXT;
#endif
    _map[KEY_REMEMBER_VIDEO_SETTINGS] = DEFAULT_REMEMBER_VIDEO_SETTINGS;
    _map[KEY_REMEMBER_VIDEO_PER_CHANNEL] = DEFAULT_REMEMBER_VIDEO_PER_CHANNEL;
    _map[KEY_ASPECT_RATIO] = DEFAULT_ASPECT_RATIO;
    _map[KEY_CROP_RATIO] = DEFAULT_CROP_RATIO;
    _map[KEY_DEINTERLACING] = DEFAULT_DEINTERLACING;
    _map[KEY_AUDIO_LANGUAGE] = DEFAULT_AUDIO_LANGUAGE;
    _map[KEY_SUBTITLE_LANGUAGE] = DEFAULT_SUBTITLE_LANGUAGE;
    _map[KEY_XMLTV_UPDATE_LOCATION] = DEFAULT_XMLTV_UPDATE_LOCATION;
    _map[KEY_XMLTV_UPDATE_REMOTE] = DEFAULT_XMLTV_UPDATE_REMOTE;
    _map[KEY_XMLTV_HOURS_OFFSET] = DEFAULT_XMLTV_HOURS_OFFSET;

    readSettings();
}

Settings::~Settings() { }

void Settings::writeSettings()
{
    setValue(KEY_LOCALE, locale());
    setValue(KEY_AUTOPLAY_LAST, autoplayLast());
    setValue(KEY_MUTE_ON_MINIMIZE, muteOnMinimize());
    setValue(KEY_RECORDINGS_DIRECTORY, recordingsDirectory());
    setValue(KEY_SNAPSHOTS_DIRECTORY, snapshotsDirectory());

    setValue(KEY_SESSION_CHANNEL, sessionChannel());
    setValue(KEY_SESSION_VOLUME, sessionVolume());

    setValue(KEY_CHANNELS_LIST, channelsList());
    setValue(KEY_RADIO_CATEGORY, radioCategory());
    setValue(KEY_HD_CATEGORY, hdCategory());
    setValue(KEY_UDPXY, udpxy());
    setValue(KEY_UDPXY_URL, udpxyUrl());
    setValue(KEY_UDPXY_PORT, udpxyPort());

    setValue(KEY_WIDTH, width());
    setValue(KEY_HEIGHT, height());
    setValue(KEY_POS_X, posX());
    setValue(KEY_POS_Y, posY());

#ifndef Q_OS_MAC
    setValue(KEY_TRAY_ENABLED, trayEnabled());
    setValue(KEY_HIDE_TO_TRAY, hideToTray());
    setValue(KEY_MOUSE_WHEEL, mouseWheel());
#endif

    setValue(KEY_VOUT, vout());
#ifdef Q_OS_WIN
    setValue(KEY_YUV_TO_RGB, yuvToRgb());
#endif
    setValue(KEY_SPDIF, spdif());
#ifdef Q_OS_LINUX
    setValue(KEY_TELETEXT, teletext());
#endif

    setValue(KEY_REMEMBER_VIDEO_SETTINGS, rememberVideoSettings());
    setValue(KEY_REMEMBER_VIDEO_PER_CHANNEL, rememberVideoPerChannel());
    setValue(KEY_ASPECT_RATIO, aspectRatio());
    setValue(KEY_CROP_RATIO, cropRatio());
    setValue(KEY_DEINTERLACING, deinterlacing());
    setValue(KEY_AUDIO_LANGUAGE, audioLanguage());
    setValue(KEY_SUBTITLE_LANGUAGE, subtitleLanguage());

    setValue(KEY_XMLTV_UPDATE_LOCATION, xmltvUpdateLocation());
    setValue(KEY_XMLTV_UPDATE_REMOTE, xmltvUpdateRemote());
    setValue(KEY_XMLTV_HOURS_OFFSET, xmltvHoursOffset());

    sync();
}

void Settings::readSettings()
{
    setLocale(value(KEY_LOCALE, defaultValue(KEY_LOCALE)).toString());
    setAutoplayLast(value(KEY_AUTOPLAY_LAST, defaultValue(KEY_AUTOPLAY_LAST)).toBool());
    setMuteOnMinimize(value(KEY_MUTE_ON_MINIMIZE, defaultValue(KEY_MUTE_ON_MINIMIZE)).toBool());
    setRecordingsDirectory(value(KEY_RECORDINGS_DIRECTORY, defaultValue(KEY_RECORDINGS_DIRECTORY)).toString());
    setSnapshotsDirectory(value(KEY_SNAPSHOTS_DIRECTORY, defaultValue(KEY_SNAPSHOTS_DIRECTORY)).toString());

    setSessionChannel(value(KEY_SESSION_CHANNEL, defaultValue(KEY_SESSION_CHANNEL)).toInt());
    setSessionVolume(value(KEY_SESSION_VOLUME, defaultValue(KEY_SESSION_VOLUME)).toInt());

    setChannelsList(value(KEY_CHANNELS_LIST, defaultValue(KEY_CHANNELS_LIST)).toString());
    setRadioCategory(value(KEY_RADIO_CATEGORY, defaultValue(KEY_RADIO_CATEGORY)).toString());
    setHdCategory(value(KEY_HD_CATEGORY, defaultValue(KEY_HD_CATEGORY)).toString());
    setUdpxy(value(KEY_UDPXY, defaultValue(KEY_UDPXY)).toBool());
    setUdpxyUrl(value(KEY_UDPXY_URL, defaultValue(KEY_UDPXY_URL)).toString());
    setUdpxyPort(value(KEY_UDPXY_PORT, defaultValue(KEY_UDPXY_PORT)).toInt());

    setWidth(value(KEY_WIDTH, defaultValue(KEY_WIDTH)).toInt());
    setHeight(value(KEY_HEIGHT, defaultValue(KEY_HEIGHT)).toInt());
    setPosX(value(KEY_POS_X, defaultValue(KEY_POS_X)).toInt());
    setPosY(value(KEY_POS_Y, defaultValue(KEY_POS_Y)).toInt());

#ifndef Q_OS_MAC
    setTrayEnabled(value(KEY_TRAY_ENABLED, defaultValue(KEY_TRAY_ENABLED)).toBool());
    setHideToTray(value(KEY_HIDE_TO_TRAY, defaultValue(KEY_HIDE_TO_TRAY)).toBool());
#endif
    setMouseWheel(value(KEY_MOUSE_WHEEL, defaultValue(KEY_MOUSE_WHEEL)).toString());

    setVout(value(KEY_VOUT, defaultValue(KEY_VOUT)).toInt());
#ifdef Q_OS_WIN
    setYuvToRgb(value(KEY_YUV_TO_RGB, defaultValue(KEY_MUTE_ON_MINIMIZE)).toBool());
#endif
    setSpdif(value(KEY_SPDIF, defaultValue(KEY_SPDIF)).toBool());
#ifdef Q_OS_LINUX
    setTeletext(value(KEY_TELETEXT, defaultValue(KEY_TELETEXT)).toBool());
#endif

    setRememberVideoSettings(value(KEY_REMEMBER_VIDEO_SETTINGS, defaultValue(KEY_REMEMBER_VIDEO_SETTINGS)).toBool());
    setRememberVideoPerChannel(value(KEY_REMEMBER_VIDEO_PER_CHANNEL, defaultValue(KEY_REMEMBER_VIDEO_PER_CHANNEL)).toBool());
    setAspectRatio(value(KEY_ASPECT_RATIO, defaultValue(KEY_ASPECT_RATIO)).toInt());
    setCropRatio(value(KEY_CROP_RATIO, defaultValue(KEY_CROP_RATIO)).toInt());
    setDeinterlacing(value(KEY_DEINTERLACING, defaultValue(KEY_DEINTERLACING)).toInt());
    setAudioLanguage(value(KEY_AUDIO_LANGUAGE, defaultValue(KEY_AUDIO_LANGUAGE)).toString());
    setSubtitleLanguage(value(KEY_SUBTITLE_LANGUAGE, defaultValue(KEY_SUBTITLE_LANGUAGE)).toString());

    setXmltvUpdateLocation(value(KEY_XMLTV_UPDATE_LOCATION, defaultValue(KEY_XMLTV_UPDATE_LOCATION)).toString());
    setXmltvUpdateRemote(value(KEY_XMLTV_UPDATE_REMOTE, defaultValue(KEY_XMLTV_UPDATE_REMOTE)).toBool());
    setXmltvHoursOffset(value(KEY_XMLTV_HOURS_OFFSET, defaultValue(KEY_XMLTV_HOURS_OFFSET)).toInt());
}

QVariant Settings::defaultValue(const QString &key) const
{
    return _map.value(key);
}
