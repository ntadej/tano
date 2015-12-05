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

#ifndef TANO_SETTINGS_H_
#define TANO_SETTINGS_H_

#include <QtCore/QSettings>

class Settings : public QSettings
{
public:
    Settings(QObject *parent = 0);
    ~Settings();

    void readSettings();
    void writeSettings();

    QVariant defaultValue(const QString &key) const;

    // General
    inline QString locale() const { return _locale; }
    inline void setLocale(const QString &s) { _locale = s; }
    static const QString KEY_LOCALE;
    static const QString DEFAULT_LOCALE;

    inline bool autoplayLast() const { return _autoplayLast; }
    inline void setAutoplayLast(bool b) { _autoplayLast = b; }
    static const QString KEY_AUTOPLAY_LAST;
    static const bool DEFAULT_AUTOPLAY_LAST;

    inline bool muteOnMinimize() const { return _muteOnMinimize; }
    inline void setMuteOnMinimize(bool b) { _muteOnMinimize = b; }
    static const QString KEY_MUTE_ON_MINIMIZE;
    static const bool DEFAULT_MUTE_ON_MINIMIZE;

    inline QString recordingsDirectory() const { return _recordingsDirectory; }
    inline void setRecordingsDirectory(const QString &s) { _recordingsDirectory = s; }
    static const QString KEY_RECORDINGS_DIRECTORY;
    static const QString DEFAULT_RECORDINGS_DIRECTORY;

    inline QString snapshotsDirectory() const { return _snapshotsDirectory; }
    inline void setSnapshotsDirectory(const QString &s) { _snapshotsDirectory = s; }
    static const QString KEY_SNAPSHOTS_DIRECTORY;
    static const QString DEFAULT_SNAPSHOTS_DIRECTORY;


    // Session
    inline int sessionChannel() const { return _sessionChannel; }
    inline void setSessionChannel(int i) { _sessionChannel = i; }
    static const QString KEY_SESSION_CHANNEL;
    static const int DEFAULT_SESSION_CHANNEL;

    inline int sessionVolume() const { return _sessionVolume; }
    inline void setSessionVolume(int i) { _sessionVolume = i; }
    static const QString KEY_SESSION_VOLUME;
    static const int DEFAULT_SESSION_VOLUME;


    // Channels
    inline QString channelsList() const { return _channelsList; }
    inline void setChannelsList(const QString &s) { _channelsList = s; }
    static const QString KEY_CHANNELS_LIST;
    static const QString DEFAULT_CHANNELS_LIST;

    inline QString radioCategory() const { return _radioCategory; }
    inline void setRadioCategory(const QString &s) { _radioCategory = s; }
    static const QString KEY_RADIO_CATEGORY;
    static const QString DEFAULT_RADIO_CATEGORY;

    inline QString hdCategory() const { return _hdCategory; }
    inline void setHdCategory(const QString &s) { _hdCategory = s; }
    static const QString KEY_HD_CATEGORY;
    static const QString DEFAULT_HD_CATEGORY;

    inline bool udpxy() const { return _udpxy; }
    inline void setUdpxy(bool b) { _udpxy = b; }
    static const QString KEY_UDPXY;
    static const bool DEFAULT_UDPXY;

    inline QString udpxyUrl() const { return _udpxyUrl; }
    inline void setUdpxyUrl(const QString &s) { _udpxyUrl = s; }
    static const QString KEY_UDPXY_URL;
    static const QString DEFAULT_UDPXY_URL;

    inline int udpxyPort() const { return _udpxyPort; }
    inline void setUdpxyPort(int i) { _udpxyPort = i; }
    static const QString KEY_UDPXY_PORT;
    static const int DEFAULT_UDPXY_PORT;


    // GUI
#ifndef Q_OS_MAC
    inline bool trayEnabled() const { return _trayEnabled; }
    inline void setTrayEnabled(bool b) { _trayEnabled = b; }
    static const QString KEY_TRAY_ENABLED;
    static const bool DEFAULT_TRAY_ENABLED;

    inline bool hideToTray() const { return _hideToTray; }
    inline void setHideToTray(bool b) { _hideToTray = b; }
    static const QString KEY_HIDE_TO_TRAY;
    static const bool DEFAULT_HIDE_TO_TRAY;
#endif
    inline QString mouseWheel() const { return _mouseWheel; }
    inline void setMouseWheel(const QString &s) { _mouseWheel = s; }
    static const QString KEY_MOUSE_WHEEL;
    static const QString DEFAULT_MOUSE_WHEEL;


    // GUI - session
    inline int width() const { return _width; }
    inline void setWidth(int i) { _width = i; }
    static const QString KEY_WIDTH;
    static const int DEFAULT_WIDTH;

    inline int height() const { return _height; }
    inline void setHeight(int i) { _height = i; }
    static const QString KEY_HEIGHT;
    static const int DEFAULT_HEIGHT;

    inline int posX() const { return _posX; }
    inline void setPosX(int i) { _posX = i; }
    static const QString KEY_POS_X;
    static const int DEFAULT_POS_X;

    inline int posY() const { return _posY; }
    inline void setPosY(int i) { _posY = i; }
    static const QString KEY_POS_Y;
    static const int DEFAULT_POS_Y;


    // Backend - core
    inline int vout() const { return _vout; }
    inline void setVout(int i) { _vout = i; }
    static const QString KEY_VOUT;
    static const int DEFAULT_VOUT;

#ifdef Q_OS_WIN
    inline bool yuvToRgb() const { return _yuvToRgb; }
    inline void setYuvToRgb(bool b) { _yuvToRgb = b; }
    static const QString KEY_YUV_TO_RGB;
    static const bool DEFAULT_YUV_TO_RGB;
#endif

    inline bool spdif() const { return _spdif; }
    inline void setSpdif(bool b) { _spdif = b; }
    static const QString KEY_SPDIF;
    static const bool DEFAULT_SPDIF;

#ifdef Q_OS_LINUX
    inline bool teletext() const { return _teletext; }
    inline void setTeletext(bool b) { _teletext = b; }
    static const QString KEY_TELETEXT;
    static const bool DEFAULT_TELETEXT;
#endif


    // Backend - video settings
    inline bool rememberVideoSettings() const { return _rememberVideoSettings; }
    inline void setRememberVideoSettings(bool b) { _rememberVideoSettings = b; }
    static const QString KEY_REMEMBER_VIDEO_SETTINGS;
    static const bool DEFAULT_REMEMBER_VIDEO_SETTINGS;

    inline bool rememberVideoPerChannel() const { return _rememberVideoPerChannel; }
    inline void setRememberVideoPerChannel(bool b) { _rememberVideoPerChannel = b; }
    static const QString KEY_REMEMBER_VIDEO_PER_CHANNEL;
    static const bool DEFAULT_REMEMBER_VIDEO_PER_CHANNEL;

    inline int aspectRatio() const { return _aspectRatio; }
    inline void setAspectRatio(int i) { _aspectRatio = i; }
    static const QString KEY_ASPECT_RATIO;
    static const int DEFAULT_ASPECT_RATIO;

    inline int cropRatio() const { return _cropRatio; }
    inline void setCropRatio(int i) { _cropRatio = i; }
    static const QString KEY_CROP_RATIO;
    static const int DEFAULT_CROP_RATIO;

    inline int deinterlacing() const { return _deinterlacing; }
    inline void setDeinterlacing(int i) { _deinterlacing = i; }
    static const QString KEY_DEINTERLACING;
    static const int DEFAULT_DEINTERLACING;

    inline QString audioLanguage() const { return _audioLanguage; }
    inline void setAudioLanguage(const QString &s) { _audioLanguage = s; }
    static const QString KEY_AUDIO_LANGUAGE;
    static const QString DEFAULT_AUDIO_LANGUAGE;

    inline QString subtitleLanguage() const { return _subtitleLanguage; }
    inline void setSubtitleLanguage(const QString &s) { _subtitleLanguage = s; }
    static const QString KEY_SUBTITLE_LANGUAGE;
    static const QString DEFAULT_SUBTITLE_LANGUAGE;


    // Schedule
    inline QString xmltvUpdateLocation() const { return _xmltvUpdateLocation; }
    inline void setXmltvUpdateLocation(const QString &s) { _xmltvUpdateLocation = s; }
    static const QString KEY_XMLTV_UPDATE_LOCATION;
    static const QString DEFAULT_XMLTV_UPDATE_LOCATION;

    inline bool xmltvUpdateRemote() const { return _xmltvUpdateRemote; }
    inline void setXmltvUpdateRemote(bool b) { _xmltvUpdateRemote = b; }
    static const QString KEY_XMLTV_UPDATE_REMOTE;
    static const bool DEFAULT_XMLTV_UPDATE_REMOTE;

    inline int xmltvHoursOffset() const { return _xmltvHoursOffset; }
    inline void setXmltvHoursOffset(int i) { _xmltvHoursOffset = i; }
    static const QString KEY_XMLTV_HOURS_OFFSET;
    static const int DEFAULT_XMLTV_HOURS_OFFSET;

private:
    QVariantMap _map;

    // General variables
    QString _locale;
    bool _autoplayLast;
    bool _muteOnMinimize;
    QString _recordingsDirectory;
    QString _snapshotsDirectory;

    // Session
    int _sessionChannel;
    int _sessionVolume;

    // Channels
    QString _channelsList;
    QString _radioCategory;
    QString _hdCategory;
    bool _udpxy;
    QString _udpxyUrl;
    int _udpxyPort;

    // GUI
#ifndef Q_OS_MAC
    bool _trayEnabled;
    bool _hideToTray;
#endif
    QString _mouseWheel;

    // GUI - session
    int _width;
    int _height;
    int _posX;
    int _posY;

    // Backend - core
    int _vout;
#ifdef Q_OS_WIN
    bool _yuvToRgb;
#endif
    bool _spdif;
#ifdef Q_OS_LINUX
    bool _teletext;
#endif

    // Backend - video settings
    bool _rememberVideoSettings;
    bool _rememberVideoPerChannel;
    int _aspectRatio;
    int _cropRatio;
    int _deinterlacing;
    QString _audioLanguage;
    QString _subtitleLanguage;

    // Schedule
    QString _xmltvUpdateLocation;
    bool _xmltvUpdateRemote;
    int _xmltvHoursOffset;
};

#endif // TANO_SETTINGS_H_
