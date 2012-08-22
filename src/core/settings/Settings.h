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

    // General
    inline QString language() const { return _language; }
    inline void setLanguage(const QString &s) { _language = s; }
    static const QString DEFAULT_LANGUAGE;

    // Playlist
    inline QString playlist() const { return _playlist; }
    inline void setPlaylist(const QString &s) { _playlist = s; }
    static const QString DEFAULT_PLAYLIST;
    inline bool playlistUpdate() const { return _playlistUpdate; }
    inline void setPlaylistUpdate(const bool &b) { _playlistUpdate = b; }
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
    inline void setUdpxy(const bool &b) { _udpxy = b; }
    static const bool DEFAULT_UDPXY;
    inline QString udpxyUrl() const { return _udpxyUrl; }
    inline void setUdpxyUrl(const QString &s) { _udpxyUrl = s; }
    static const QString DEFAULT_UDPXY_URL;
    inline int udpxyPort() const { return _udpxyPort; }
    inline void setUdpxyPort(const int &i) { _udpxyPort = i; }
    static const int DEFAULT_UDPXY_PORT;

    // GUI - start
    inline bool startLite() const { return _startLite; }
    inline void setStartLite(const bool &b) { _startLite = b; }
    static const bool DEFAULT_START_LITE;
    inline bool startOnTop() const { return _startOnTop; }
    inline void setStartOnTop(const bool &b) { _startOnTop = b; }
    static const bool DEFAULT_START_ON_TOP;

    // GUI
    inline bool osd() const { return _osd; }
    inline void setOsd(const bool &b) { _osd = b; }
    static const bool DEFAULT_OSD;
    inline bool info() const { return _info; }
    inline void setInfo(const bool &b) { _info = b; }
    static const bool DEFAULT_INFO;
    inline bool trayEnabled() const { return _trayEnabled; }
    inline void setTrayEnabled(const bool &b) { _trayEnabled = b; }
    static const bool DEFAULT_TRAY_ENABLED;
    inline bool hideToTray() const { return _hideToTray; }
    inline void setHideToTray(const bool &b) { _hideToTray = b; }
    static const bool DEFAULT_HIDE_TO_TRAY;
    inline QString mouseWheel() const { return _mouseWheel; }
    inline void setMouseWheel(const QString &s) { _mouseWheel = s; }
    static const QString DEFAULT_MOUSE_WHEEL;
    inline int toolbarLook() const { return _toolbarLook; }
    inline void setToolbarLook(const int &i) { _toolbarLook = i; }
    static const int DEFAULT_TOOLBAR_LOOK;
    inline bool rememberGuiSession() const { return _rememberGuiSession; }
    inline void setRememberGuiSession(const bool &b) { _rememberGuiSession = b; }
    static const bool DEFAULT_REMEMBER_GUI_SESSION;
    inline bool filtersVisible() const { return _filtersVisible; }
    inline void setFiltersVisible(const bool &b) { _filtersVisible = b; }
    static const bool DEFAULT_FILTERS_VISIBLE;

    // GUI - session
    inline int width() const { return _width; }
    inline void setWidth(const int &i) { _width = i; }
    static const int DEFAULT_WIDTH;
    inline int height() const { return _height; }
    inline void setHeight(const int &i) { _height = i; }
    static const int DEFAULT_HEIGHT;
    inline int posX() const { return _posX; }
    inline void setPosX(const int &i) { _posX = i; }
    static const int DEFAULT_POS_X;
    inline int posY() const { return _posY; }
    inline void setPosY(const int &i) { _posY = i; }
    static const int DEFAULT_POS_Y;
    inline bool startControls() const { return _startControls; }
    inline void setStartControls(const bool &b) { _startControls = b; }
    static const bool DEFAULT_START_CONTROLS;
    inline bool startInfo() const { return _startInfo; }
    inline void setStartInfo(const bool &b) { _startInfo = b; }
    static const bool DEFAULT_START_INFO;

    // Backend - core
    inline int vout() const { return _vout; }
    inline void setVout(const int &i) { _vout = i; }
    static const int DEFAULT_VOUT;
    inline int aout() const { return _aout; }
    inline void setAout(const int &i) { _aout = i; }
    static const int DEFAULT_AOUT;
#if defined(Q_OS_WIN32)
    inline bool yuvToRgb() const { return _yuvToRgb; }
    inline void setYuvToRgb(const bool &b) { _yuvToRgb = b; }
    static const bool DEFAULT_YUV_TO_RGB;
#endif
    inline bool spdif() const { return _spdif; }
    inline void setSpdif(const bool &b) { _spdif = b; }
    static const bool DEFAULT_SPDIF;
    inline int interfaceIndex() const { return _interfaceIndex; }
    inline void setInterfaceIndex(const int &i) { _interfaceIndex = i; }
    static const int DEFAULT_INTERFACE_INDEX;

    // Backend - video settings
    inline bool rememberVideoSettings() const { return _rememberVideoSettings; }
    inline void setRememberVideoSettings(const bool &b) { _rememberVideoSettings = b; }
    static const bool DEFAULT_REMEMBER_VIDEO_SETTINGS;
    inline bool rememberVideoPerChannel() const { return _rememberVideoPerChannel; }
    inline void setRememberVideoPerChannel(const bool &b) { _rememberVideoPerChannel = b; }
    static const bool DEFAULT_REMEMBER_VIDEO_PER_CHANNEL;
    inline int aspectRatio() const { return _aspectRatio; }
    inline void setAspectRatio(const int &i) { _aspectRatio = i; }
    static const int DEFAULT_ASPECT_RATIO;
    inline int cropRatio() const { return _cropRatio; }
    inline void setCropRatio(const int &i) { _cropRatio = i; }
    static const int DEFAULT_CROP_RATIO;
    inline int deinterlacing() const { return _deinterlacing; }
    inline void setDeinterlacing(const int &i) { _deinterlacing = i; }
    static const int DEFAULT_DEINTERLACING;
    inline QString audioLanguage() const { return _audioLanguage; }
    inline void setAudioLanguage(const QString &s) { _audioLanguage = s; }
    static const QString DEFAULT_AUDIO_LANGUAGE;
    inline QString subtitleLanguage() const { return _subtitleLanguage; }
    inline void setSubtitleLanguage(const QString &s) { _subtitleLanguage = s; }
    static const QString DEFAULT_SUBTITLE_LANGUAGE;

    // Backend - misc
    inline bool muteOnMinimize() const { return _muteOnMinimize; }
    inline void setMuteOnMinimize(const bool &b) { _muteOnMinimize = b; }
    static const bool DEFAULT_MUTE_ON_MINIMIZE;
    inline bool teletext() const { return _teletext; }
    inline void setTeletext(const bool &b) { _teletext = b; }
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
    inline void setSessionAutoplay(const bool &b) { _sessionAutoplay = b; }
    static const bool DEFAULT_SESSION_AUTOPLAY;
    inline int sessionChannel() const { return _sessionChannel; }
    inline void setSessionChannel(const int &i) { _sessionChannel = i; }
    static const int DEFAULT_SESSION_CHANNEL;
    inline bool sessionRememberVolume() const { return _sessionRememberVolume; }
    inline void setSessionRememberVolume(const bool &b) { _sessionRememberVolume = b; }
    static const bool DEFAULT_SESSION_REMEMBER_VOLUME;
    inline int sessionVolume() const { return _sessionVolume; }
    inline void setSessionVolume(const int &i) { _sessionVolume = i; }
    static const int DEFAULT_SESSION_VOLUME;

    // Schedule
    inline QString xmltvLocation() const { return _xmltvLocation; }
    inline void setXmltvLocation(const QString &s) { _xmltvLocation = s; }
    static const QString DEFAULT_XMLTV_LOCATION;
    inline bool xmltvUpdate() const { return _xmltvUpdate; }
    inline void setXmltvUpdate(const bool &b) { _xmltvUpdate = b; }
    static const bool DEFAULT_XMLTV_UPDATE;
    inline bool xmltvUpdateGrabber() const { return _xmltvUpdateGrabber; }
    inline void setXmltvUpdateGrabber(const bool &b) { _xmltvUpdateGrabber = b; }
    static const bool DEFAULT_XMLTV_UPDATE_GRABBER;
    inline QString xmltvUpdateUrl() const { return _xmltvUpdateUrl; }
    inline void setXmltvUpdateUrl(const QString &s) { _xmltvUpdateUrl = s; }
    static const QString DEFAULT_XMLTV_UPDATE_URL;
    inline QString xmltvGrabber() const { return _xmltvGrabber; }
    inline void setXmltvGrabber(const QString &s) { _xmltvGrabber = s; }
    static const QString DEFAULT_XMLTV_GRABBER;
    inline QString xmltvGrabberPath() const { return _xmltvGrabberPath; }
    inline void setXmltvGrabberPath(const QString &s) { _xmltvGrabberPath = s; }
    static const QString DEFAULT_XMLTV_GRABBER_PATH;

    // Constants
    static const QString SUPPORT_ADDRESS;

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

    // GUI - start
    bool _splash;
    bool _startLite;
    bool _startOnTop;

    // GUI
    bool _osd;
    bool _info;
    bool _trayEnabled;
    bool _hideToTray;
    QString _mouseWheel;
    int _toolbarLook;
    bool _rememberGuiSession;
    bool _filtersVisible;

    // GUI - session
    int _width;
    int _height;
    int _posX;
    int _posY;
    bool _startControls;
    bool _startInfo;

    // Backend - core
    int _vout;
    int _aout;
#if defined(Q_OS_WIN32)
    bool _yuvToRgb;
#endif
    bool _spdif;
    int _interfaceIndex;

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
    QString _xmltvLocation;
    bool _xmltvUpdate;
    bool _xmltvUpdateGrabber;
    QString _xmltvUpdateUrl;
    QString _xmltvGrabber;
    QString _xmltvGrabberPath;
};

#endif // TANO_SETTINGS_H_
