#include <QtCore/QDebug>

#include <QLCDNumber>
#include <QVBoxLayout>

#include <VLCQtCore/Common.h>
#include <VLCQtCore/Config.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <VLCQtCore/Video.h>

#include <VLCQtWidgets/ControlAudio.h>
#include <VLCQtWidgets/ControlVideo.h>
#include <VLCQtWidgets/WidgetVideo.h>

#include "settings/Settings.h"
#include "settings/SettingsChannel.h"

#include "common/Backend.h"
#include "common/FileDialogs.h"
#include "common/OsdWidget.h"
#include "menu/MenuAspectRatio.h"
#include "menu/MenuCropRatio.h"
#include "menu/MenuDeinterlacing.h"
#include "menu/MenuScale.h"
#include "menu/MenuTrackAudio.h"
#include "menu/MenuTrackSubtitles.h"
#include "menu/MenuTrackVideo.h"
#include "style/VolumeSlider.h"

#include "MediaPlayer.h"

MediaPlayer::MediaPlayer(QWidget *parent)
    : QWidget(parent),
      _settingsChannel(new SettingsChannel(this)),
      _startTimer(new QTimer(this))
{
    createSettingsStartup();

    _mediaInstance = new VlcInstance(Tano::Backend::args(), this);
    _mediaItem = 0;
    _mediaPlayer = new VlcMediaPlayer(_mediaInstance);
    _videoWidget = new VlcWidgetVideo(this);
    _videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    _mediaPlayer->setVideoWidget(_videoWidget);

    _audioController = new VlcControlAudio(_mediaPlayer, "", this);
    _videoController = new VlcControlVideo(_mediaPlayer, "", this);

    _videoWidget->setMediaPlayer(_mediaPlayer);
    _videoWidget->setStyleSheet("background-color: rgb(0,0,0);");
    _videoWidget->initDefaultSettings();

    _menuTrackAudio = new MenuTrackAudio();
    _menuTrackVideo = new MenuTrackVideo();
    _menuTrackSubtitles = new MenuTrackSubtitles();
    _menuAspectRatio = new MenuAspectRatio(_videoWidget);
    _menuCropRatio = new MenuCropRatio(_videoWidget);
    _menuScale = new MenuScale(_videoWidget);
    _menuDeinterlacing = new MenuDeinterlacing(_videoWidget);

    _osd = new OsdWidget(this);
    _osd->setBackend(_mediaPlayer);
    _osd->setProperty("topBorder", true);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(_videoWidget);
    layout->addWidget(_osd);
    setLayout(layout);

#if !defined(Q_OS_LINUX)
    _osd->toggleTeletext(false);
#else
    _osd->toggleTeletext(_teletext);
#endif

    createSettings();
    createConnections();
}

MediaPlayer::~MediaPlayer() { }

void MediaPlayer::createConnections()
{
    connect(_startTimer, SIGNAL(timeout()), this, SLOT(startSession()));

    connect(_mediaPlayer, SIGNAL(stateChanged()), this, SLOT(stateUpdate()));
    connect(_mediaPlayer, SIGNAL(vout(int)), this, SLOT(stateUpdateVideo(int)));

    connect(_audioController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackAudio, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackSubtitles, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_videoController, SIGNAL(actions(QList<QAction*>, Vlc::ActionsType)), _menuTrackVideo, SLOT(setActions(QList<QAction*>, Vlc::ActionsType)));
    connect(_menuTrackSubtitles, SIGNAL(subtitles(QString)), _videoController, SLOT(loadSubtitle(QString)));

    connect(_menuAspectRatio, SIGNAL(value(int)), this, SLOT(saveChannelSettings(int)));
    connect(_menuCropRatio, SIGNAL(value(int)), this, SLOT(saveChannelSettings(int)));
    connect(_menuDeinterlacing, SIGNAL(value(int)), this, SLOT(saveChannelSettings(int)));

    connect(_osd, SIGNAL(teletextPage(int)), this, SLOT(teletext(int)));
}

void MediaPlayer::createSettings()
{
    QScopedPointer<Settings> settings(new Settings(this));
    //Playback settings
    _defaultAspectRatio = settings->aspectRatio();
    _defaultCropRatio = settings->cropRatio();
    _defaultDeinterlacing = settings->deinterlacing();

    _videoWidget->setDefaultAspectRatio(Vlc::Ratio(_defaultAspectRatio));
    _videoWidget->setDefaultCropRatio(Vlc::Ratio(_defaultCropRatio));
    _videoWidget->setDefaultDeinterlacing(Vlc::Deinterlacing(_defaultDeinterlacing));
    _menuAspectRatio->setDefault(Vlc::Ratio(_defaultAspectRatio));
    _menuCropRatio->setDefault(Vlc::Ratio(_defaultCropRatio));
    _menuDeinterlacing->setDefault(Vlc::Deinterlacing(_defaultDeinterlacing));
    _menuScale->setDefault(Vlc::Scale(0));

    _defaultAudioLanguage = settings->audioLanguage();
    _defaultSubtitleLanguage = settings->subtitleLanguage();
    _videoSettings = settings->rememberVideoSettings();
    _videoSettingsChannel = settings->rememberVideoPerChannel();
    if (_audioController)
        _audioController->setDefaultAudioLanguage(_defaultAudioLanguage);
    if (_videoController)
        _videoController->setDefaultSubtitleLanguage(_defaultSubtitleLanguage);

    _settingsChannel->setDefaults(_defaultAspectRatio, _defaultCropRatio, _defaultDeinterlacing, _defaultAudioLanguage, _defaultSubtitleLanguage);

    _sessionVolumeEnabled = settings->sessionVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();

    _defaultSnapshot = settings->snapshotsDirectory();
}

void MediaPlayer::createSettingsStartup()
{
    QScopedPointer<Settings> settings(new Settings(this));
    //Session
    _sessionVolumeEnabled = settings->sessionRememberVolume();
    _sessionAutoplayEnabled = settings->sessionAutoplay();
    _sessionVolume = settings->sessionVolume();
    _sessionChannel = settings->sessionChannel();
    _sessionGui = settings->rememberGuiSession();

    _teletext = settings->teletext();
}

void MediaPlayer::createSession(bool valid)
{
    _osd->volumeSlider()->setVolume(_sessionVolume);

    if (_sessionAutoplayEnabled && valid) {
        _startTimer->start(100);
    }
}

void MediaPlayer::startSession()
{
    if (!isVisible())
        return;

    emit sessionChannel(_sessionChannel);

    _startTimer->stop();
}

void MediaPlayer::writeSession()
{
    QScopedPointer<Settings> settings(new Settings(this));

    if (_sessionVolumeEnabled)
        settings->setSessionVolume(_osd->volumeSlider()->volume());
    else
        settings->setSessionVolume(settings->defaultValue(Settings::KEY_SESSION_VOLUME).toInt());

    if (_sessionAutoplayEnabled)
        settings->setSessionChannel(_osd->lcd()->value());

    if (_sessionGui) {
        settings->setWidth(parentWidget()->size().width());
        settings->setHeight(parentWidget()->size().height());
        settings->setPosX(parentWidget()->x());
        settings->setPosY(parentWidget()->y());
    }

    settings->writeSettings();

    qDebug() << "Session written";
}

QList<MenuCore *> MediaPlayer::menus() const
{
    QList<MenuCore *> list;
    list << _menuTrackAudio
         << _menuTrackVideo
         << _menuTrackSubtitles
         << _menuAspectRatio
         << _menuCropRatio
         << _menuScale
         << _menuDeinterlacing;

    return list;
}

void MediaPlayer::play()
{
    _mediaPlayer->open(_mediaItem);

    if (_channelPlayback && _videoSettingsChannel) {
        _videoWidget->setCurrentAspectRatio(Vlc::Ratio(_settingsChannel->aspectRatio(_mediaItem->currentLocation())));
        _videoWidget->setCurrentCropRatio(Vlc::Ratio(_settingsChannel->cropRatio(_mediaItem->currentLocation())));
        _videoWidget->setCurrentDeinterlacing(Vlc::Deinterlacing(_settingsChannel->deinterlacing(_mediaItem->currentLocation())));

        _menuAspectRatio->setDefault(Vlc::Ratio(_settingsChannel->aspectRatio(_mediaItem->currentLocation())));
        _menuCropRatio->setDefault(Vlc::Ratio(_settingsChannel->cropRatio(_mediaItem->currentLocation())));
        _menuDeinterlacing->setDefault(Vlc::Deinterlacing(_settingsChannel->deinterlacing(_mediaItem->currentLocation())));
        _menuScale->setDefault(Vlc::Scale(0));
    }

    if (_videoSettingsChannel)
        _videoWidget->enablePreviousSettings();
    else if (_videoSettings)
        _videoWidget->enablePreviousSettings();
    else
        _videoWidget->enableDefaultSettings();
}

void MediaPlayer::playLocal(const QString &path)
{
    if (path.isEmpty())
        return;

    stop();

    if (_mediaItem)
        delete _mediaItem;
    _mediaItem = new VlcMedia(path, true, _mediaInstance);

    play();
}

void MediaPlayer::playUrl(const QString &url,
                          bool channel)
{
    if (url.isEmpty())
        return;

    stop();

    _channelPlayback = channel;

    if (_mediaItem)
        delete _mediaItem;
    _mediaItem = new VlcMedia(url, _mediaInstance);

    play();
}

QString MediaPlayer::recordNow(const QString &name,
                               const QString &output) const
{
    _mediaPlayer->stop();
    QString file = _mediaItem->duplicate(name, output, Vlc::TS);
    _mediaPlayer->play();

    return file;
}

void MediaPlayer::recordNowStop()
{
    _mediaPlayer->stop();
    QString media = _mediaItem->currentLocation();
    delete _mediaItem;
    _mediaItem = new VlcMedia(media, _mediaInstance);
    play();
}

void MediaPlayer::restoreOsd()
{
    layout()->addWidget(_osd);
}

void MediaPlayer::stateUpdate()
{
    switch(_mediaPlayer->state())
    {
    case Vlc::Playing:
        _osd->setPlaying(true);
        break;
    case Vlc::Idle:
    case Vlc::Opening:
    case Vlc::Buffering:
    case Vlc::Paused:
    case Vlc::Stopped:
    case Vlc::Ended:
    case Vlc::Error:
        _osd->setPlaying(false);
    default:
        break;
    }

    emit stateChanged(_mediaPlayer->state());
}

void MediaPlayer::stateUpdateVideo(int count)
{
    _osd->setVideoState(count);

    emit vout(count);
}

void MediaPlayer::stop()
{
    _channelPlayback = false;
    _mediaPlayer->stop();

    if (!_videoSettings || _videoSettingsChannel) {
        _menuAspectRatio->setDefault(Vlc::Ratio(_defaultAspectRatio));
        _menuCropRatio->setDefault(Vlc::Ratio(_defaultCropRatio));
        _menuDeinterlacing->setDefault(Vlc::Deinterlacing(_defaultDeinterlacing));
        _menuScale->setDefault(Vlc::Scale(0));
    }

    _osd->setChannel();

    _audioController->reset();
    _videoController->reset();
}

void MediaPlayer::openFile()
{
    QString file = FileDialogs::openFile();

    if (file.isEmpty())
        return;

    playLocal(file);
}

void MediaPlayer::openUrl()
{
    QString url = FileDialogs::openUrl();

    if (url.isEmpty())
        return;

    playUrl(url);
}

void MediaPlayer::saveChannelSettings(int value)
{
    if (_mediaPlayer->state() != Vlc::Playing)
        return;

    MenuCore *menu = qobject_cast<MenuCore *>(sender());
    if (!menu)
        return;

    if (_channelPlayback) {
        if (menu == _menuAspectRatio)
            _settingsChannel->setAspectRatio(_mediaItem->currentLocation(), value);
        else if (menu == _menuCropRatio)
            _settingsChannel->setCropRatio(_mediaItem->currentLocation(), value);
        else if (menu == _menuDeinterlacing)
            _settingsChannel->setDeinterlacing(_mediaItem->currentLocation(), value);
    }
}

void MediaPlayer::saveChannelSettings(const QString &value)
{
    Q_UNUSED(value)
}

void MediaPlayer::takeSnapshot()
{
    _mediaPlayer->video()->takeSnapshot(_defaultSnapshot);
}

void MediaPlayer::teletext(bool enabled)
{
    if (enabled) {
        _mediaPlayer->video()->setTeletextPage(100);
        _osd->setTeletextPage(100);
    } else {
        _mediaPlayer->video()->setTeletextPage(99);
    }
}

void MediaPlayer::teletext(int page)
{
    if (page == _mediaPlayer->video()->teletextPage())
        return;

    _mediaPlayer->video()->setTeletextPage(page);
}

void MediaPlayer::togglePause()
{
    _mediaPlayer->togglePause();
}
