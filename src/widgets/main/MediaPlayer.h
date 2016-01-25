#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QWidget>

#include <VLCQtCore/Enums.h>

class QLCDNumber;

class VlcInstance;
class VlcMedia;
class VlcMediaPlayer;

class VlcControlAudio;
class VlcControlVideo;
class VlcWidgetVideo;

class MenuAspectRatio;
class MenuCore;
class MenuCropRatio;
class MenuDeinterlacing;
class MenuScale;
class MenuTrackAudio;
class MenuTrackSubtitles;
class MenuTrackVideo;
class OsdWidget;
class SettingsChannel;

class MediaPlayer : public QWidget
{
Q_OBJECT
public:
    explicit MediaPlayer(QWidget *parent = 0);
    ~MediaPlayer();

    void createSession(bool valid);
    void createSettings();
    inline OsdWidget *osd() { return _osd; }
    inline VlcInstance *mediaInstance() { return _mediaInstance; }
    QList<MenuCore *> menus() const;
    void restoreOsd();
    inline bool teletextEnabled() const { return _teletext; }
    inline VlcWidgetVideo *video() { return _videoWidget; }
    void writeSession();

signals:
    void stateChanged(const Vlc::State &);
    void vout(int count);
    void sessionChannel(int);
    
public slots:
    void openFile();
    void openUrl();
    void play();
    void playLocal(const QString &path);
    void playUrl(const QString &url,
                 bool channel = false);
    QString recordNow(const QString &name,
                      const QString &output) const;
    void recordNowStop();
    void stop();
    void takeSnapshot();
    void teletext(bool enabled);
    void teletext(int page);
    void togglePause();

private slots:
    void saveChannelSettings(int value);
    void saveChannelSettings(const QString &value);
    void startSession();
    void stateUpdate();
    void stateUpdateVideo(int count);

private:
    void createConnections();
    void createSettingsStartup();

    // Backend
    VlcControlAudio *_audioController;
    VlcInstance *_mediaInstance;
    VlcMedia *_mediaItem;
    VlcMediaPlayer *_mediaPlayer;
    VlcControlVideo *_videoController;
    VlcWidgetVideo *_videoWidget;

    // Main
    SettingsChannel *_settingsChannel;
    QTimer *_startTimer;
    
    // GUI
    MenuAspectRatio *_menuAspectRatio;
    MenuCropRatio *_menuCropRatio;
    MenuDeinterlacing *_menuDeinterlacing;
    MenuScale *_menuScale;
    MenuTrackAudio *_menuTrackAudio;
    MenuTrackSubtitles *_menuTrackSubtitles;
    MenuTrackVideo *_menuTrackVideo;
    OsdWidget *_osd;

    // Common
    bool _channelPlayback;
    int _defaultAspectRatio;
    QString _defaultAudioLanguage;
    int _defaultCropRatio;
    int _defaultDeinterlacing;
    QString _defaultSnapshot;
    QString _defaultSubtitleLanguage;
    int _sessionChannel;
    bool _sessionVolumeEnabled;
    bool _sessionAutoplayEnabled;
    int _sessionVolume;
    bool _sessionGui;
    bool _teletext;
    bool _videoSettings;
    bool _videoSettingsChannel;
};

#endif // MEDIAPLAYER_H
