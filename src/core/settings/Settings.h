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

#ifndef TANO_SETTINGS_H_
#define TANO_SETTINGS_H_

#include <QtCore/QSettings>

#include "CoreSharedExport.h"

class TANO_CORE_EXPORT Settings : public QSettings
{
public:
    Settings(QObject *parent = 0);
    ~Settings();

    void readSettings();
    void writeSettings();

    // General
    inline QString language() const { return _language; }
    inline void setLanguage(const QString &s) { _language = s; }
    static const QString DEFAULT_LANGUAGE;

    // Playlist
    inline QString playlist() const { return _playlist; }
    inline void setPlaylist(const QString &s) { _playlist = s; }
    static const QString DEFAULT_PLAYLIST;
    inline bool playlistUpdate() const { return _playlistUpdate; }
    inline void setPlaylistUpdate(bool b) { _playlistUpdate = b; }
    static const bool DEFAULT_PLAYLIST_UPDATE;
    inline QString playlistUpdateUrl() const { return _playlistUpdateUrl; }
    inline void setPlaylistUpdateUrl(const QString &s) { _playlistUpdateUrl = s; }
    static const QString DEFAULT_PLAYLIST_UPDATE_URL;
    inline QString radioCategory() const { return _radioCategory; }
    inline void setRadioCategory(const QString &s) { _radioCategory = s; }
    static const QString DEFAULT_RADIO_CATEGORY;
    inline QString hdCategory() const { return _hdCategory; }
    inline void setHdCategory(const QString &s) { _hdCategory = s; }
    static const QString DEFAULT_HD_CATEGORY;
    inline bool udpxy() const { return _udpxy; }
    inline void setUdpxy(bool b) { _udpxy = b; }
    static const bool DEFAULT_UDPXY;
    inline QString udpxyUrl() const { return _udpxyUrl; }
    inline void setUdpxyUrl(const QString &s) { _udpxyUrl = s; }
    static const QString DEFAULT_UDPXY_URL;
    inline int udpxyPort() const { return _udpxyPort; }
    inline void setUdpxyPort(int i) { _udpxyPort = i; }
    static const int DEFAULT_UDPXY_PORT;

    // GUI
    inline bool osd() const { return _osd; }
    inline void setOsd(bool b) { _osd = b; }
    static const bool DEFAULT_OSD;
    inline bool trayEnabled() const { return _trayEnabled; }
    inline void setTrayEnabled(bool b) { _trayEnabled = b; }
    static const bool DEFAULT_TRAY_ENABLED;
    inline bool hideToTray() const { return _hideToTray; }
    inline void setHideToTray(bool b) { _hideToTray = b; }
    static const bool DEFAULT_HIDE_TO_TRAY;
    inline QString mouseWheel() const { return _mouseWheel; }
    inline void setMouseWheel(const QString &s) { _mouseWheel = s; }
    static const QString DEFAULT_MOUSE_WHEEL;
    inline bool rememberGuiSession() const { return _rememberGuiSession; }
    inline void setRememberGuiSession(bool b) { _rememberGuiSession = b; }
    static const bool DEFAULT_REMEMBER_GUI_SESSION;
    inline QString icons() const { return _icons; }
    inline void setIcons(const QString &s) { _icons = s; }
    static const QString DEFAULT_ICONS;

    // GUI - session
    inline int width() const { return _width; }
    inline void setWidth(int i) { _width = i; }
    static const int DEFAULT_WIDTH;
    inline int height() const { return _height; }
    inline void setHeight(int i) { _height = i; }
    static const int DEFAULT_HEIGHT;
    inline int posX() const { return _posX; }
    inline void setPosX(int i) { _posX = i; }
    static const int DEFAULT_POS_X;
    inline int posY() const { return _posY; }
    inline void setPosY(int i) { _posY = i; }
    static const int DEFAULT_POS_Y;

    // Backend - core
    inline int vout() const { return _vout; }
    inline void setVout(int i) { _vout = i; }
    static const int DEFAULT_VOUT;
    inline int aout() const { return _aout; }
    inline void setAout(int i) { _aout = i; }
    static const int DEFAULT_AOUT;
#if defined(Q_OS_WIN32)
    inline bool yuvToRgb() const { return _yuvToRgb; }
    inline void setYuvToRgb(bool b) { _yuvToRgb = b; }
    static const bool DEFAULT_YUV_TO_RGB;
#endif
    inline bool spdif() const { return _spdif; }
    inline void setSpdif(bool b) { _spdif = b; }
    static const bool DEFAULT_SPDIF;

    // Backend - video settings
    inline bool rememberVideoSettings() const { return _rememberVideoSettings; }
    inline void setRememberVideoSettings(bool b) { _rememberVideoSettings = b; }
    static const bool DEFAULT_REMEMBER_VIDEO_SETTINGS;
    inline bool rememberVideoPerChannel() const { return _rememberVideoPerChannel; }
    inline void setRememberVideoPerChannel(bool b) { _rememberVideoPerChannel = b; }
    static const bool DEFAULT_REMEMBER_VIDEO_PER_CHANNEL;
    inline int aspectRatio() const { return _aspectRatio; }
    inline void setAspectRatio(int i) { _aspectRatio = i; }
    static const int DEFAULT_ASPECT_RATIO;
    inline int cropRatio() const { return _cropRatio; }
    inline void setCropRatio(int i) { _cropRatio = i; }
    static const int DEFAULT_CROP_RATIO;
    inline int deinterlacing() const { return _deinterlacing; }
    inline void setDeinterlacing(int i) { _deinterlacing = i; }
    static const int DEFAULT_DEINTERLACING;
    inline QString audioLanguage() const { return _audioLanguage; }
    inline void setAudioLanguage(const QString &s) { _audioLanguage = s; }
    static const QString DEFAULT_AUDIO_LANGUAGE;
    inline QString subtitleLanguage() const { return _subtitleLanguage; }
    inline void setSubtitleLanguage(const QString &s) { _subtitleLanguage = s; }
    static const QString DEFAULT_SUBTITLE_LANGUAGE;

    // Backend - misc
    inline bool muteOnMinimize() const { return _muteOnMinimize; }
    inline void setMuteOnMinimize(bool b) { _muteOnMinimize = b; }
    static const bool DEFAULT_MUTE_ON_MINIMIZE;
    inline bool teletext() const { return _teletext; }
    inline void setTeletext(bool b) { _teletext = b; }
    static const bool DEFAULT_TELETEXT;

    // Recorder
    inline QString recorderDirectory() const { return _recorderDirectory; }
    inline void setRecorderDirectory(const QString &s) { _recorderDirectory = s; }
    static const QString DEFAULT_RECORDER_DIRECTORY;
    inline QString snapshotsDirectory() const { return _snapshotsDirectory; }
    inline void setSnapshotsDirectory(const QString &s) { _snapshotsDirectory = s; }
    static const QString DEFAULT_SNAPSHOTS_DIRECTORY;

    // Session
    inline bool sessionAutoplay() const { return _sessionAutoplay; }
    inline void setSessionAutoplay(bool b) { _sessionAutoplay = b; }
    static const bool DEFAULT_SESSION_AUTOPLAY;
    inline int sessionChannel() const { return _sessionChannel; }
    inline void setSessionChannel(int i) { _sessionChannel = i; }
    static const int DEFAULT_SESSION_CHANNEL;
    inline bool sessionRememberVolume() const { return _sessionRememberVolume; }
    inline void setSessionRememberVolume(bool b) { _sessionRememberVolume = b; }
    static const bool DEFAULT_SESSION_REMEMBER_VOLUME;
    inline int sessionVolume() const { return _sessionVolume; }
    inline void setSessionVolume(int i) { _sessionVolume = i; }
    static const int DEFAULT_SESSION_VOLUME;

    // Schedule
    inline bool xmltvUpdate() const { return _xmltvUpdate; }
    inline void setXmltvUpdate(bool b) { _xmltvUpdate = b; }
    static const bool DEFAULT_XMLTV_UPDATE;
    inline QString xmltvUpdateLocation() const { return _xmltvUpdateLocation; }
    inline void setXmltvUpdateLocation(const QString &s) { _xmltvUpdateLocation = s; }
    static const QString DEFAULT_XMLTV_UPDATE_LOCATION;
    inline bool xmltvUpdateRemote() const { return _xmltvUpdateRemote; }
    inline void setXmltvUpdateRemote(bool b) { _xmltvUpdateRemote = b; }
    static const bool DEFAULT_XMLTV_UPDATE_REMOTE;

private:
    // General variables
    QString _language;

    // Channels
    QString _playlist;
    bool _playlistUpdate;
    QString _playlistUpdateUrl;
    QString _radioCategory;
    QString _hdCategory;
    bool _udpxy;
    QString _udpxyUrl;
    int _udpxyPort;

    // GUI
    bool _osd;
    bool _trayEnabled;
    bool _hideToTray;
    QString _mouseWheel;
    bool _rememberGuiSession;
    QString _icons;

    // GUI - session
    int _width;
    int _height;
    int _posX;
    int _posY;

    // Backend - core
    int _vout;
    int _aout;
#if defined(Q_OS_WIN32)
    bool _yuvToRgb;
#endif
    bool _spdif;

    // Backend - video settings
    bool _rememberVideoSettings;
    bool _rememberVideoPerChannel;
    int _aspectRatio;
    int _cropRatio;
    int _deinterlacing;
    QString _audioLanguage;
    QString _subtitleLanguage;

    // Backend - misc
    bool _muteOnMinimize;
    bool _teletext;

    // Recorder
    QString _recorderDirectory;
    QString _snapshotsDirectory;

    // Session
    bool _sessionAutoplay;
    int _sessionChannel;
    bool _sessionRememberVolume;
    int _sessionVolume;

    // Schedule
    bool _xmltvUpdate;
    QString _xmltvUpdateLocation;
    bool _xmltvUpdateRemote;

    // Disables
    bool _disableInterface;
    bool _disablePlayback;
    bool _disablePlaylist;
    bool _disableRecorder;
    bool _disableSchedule;
};

#endif // TANO_SETTINGS_H_
