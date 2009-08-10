#include <QDebug>

#include "VlcInstance.h"

libvlc_instance_t * _vlcInstance = NULL;
libvlc_exception_t * _vlcException = new libvlc_exception_t();
libvlc_media_player_t * _vlcCurrentMediaPlayer = NULL;

VlcInstance::VlcInstance(WId widget)
{
	//Global variables
	_vlcInstance = NULL;
	_vlcException = new libvlc_exception_t();

	_vlcMediaPlayer = NULL;
	_vlcMedia = NULL;

	_widgetId = widget;

	//QString pluginPath("--plugin-path=" + getVLCPluginPath());

	//Complete list of VLC_Backend command line options:
	//http://wiki.videolan.org/VLC_command-line_help
	const char * vlcArgs[] = {
		//strdup(pluginPath.toUtf8().constData()),
		//"-vvv",			//Debug messages level: maximum
		"--intf=dummy",			//Don't use any interface
		"--no-media-library",
		//"--no-one-instance",
		"--reset-plugins-cache",
		"--no-stats",
		"--no-osd",
		"--no-video-title-show",
		"-V", "x11",
		"--vout-event", "3",
		//"--x11-event", "3", // VLC 0.9.*
		"--ignore-config"		//Don't use VLC_Backend's config
	};

	libvlc_exception_init(_vlcException);

	//Init VLC_Backend modules, should be done only once
	_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs, _vlcException);
	checkException();

	_isPlaying = false;

	qDebug() << "Using VLC version:" << libvlc_get_version();
	qDebug() << "VLC loaded";

}

VlcInstance::~VlcInstance()
{
	libvlc_release(_vlcInstance);
}

void VlcInstance::openMedia(QString media)
{
	unloadMedia();
    /* Create a new LibVLC media descriptor */
    _vlcMedia = libvlc_media_new (_vlcInstance, media.toAscii(), _vlcException);
    checkException();

    _vlcMediaPlayer = libvlc_media_player_new_from_media (_vlcMedia, _vlcException);
    checkException();

	/* Get our media instance to use our window */
	if (_vlcMediaPlayer) {
#if defined(Q_OS_WIN)
		libvlc_media_player_set_hwnd(_vlcMediaPlayer, _widgetId, _vlcException); // VLC 1.0.*
		//libvlc_media_player_set_drawable(_vlcMediaPlayer, reinterpret_cast<unsigned int>(_widgetId), _vlcException); // VLC 0.9.*
#elif defined(Q_OS_MAC)
		libvlc_media_player_set_agl (_vlcMediaPlayer, _widgetId, _vlcException); // VLC 1.0.*
		//libvlc_media_player_set_drawable(_vlcMediaPlayer, _widgetId, _vlcException); // VLC 0.9.*
#else
		libvlc_media_player_set_xwindow(_vlcMediaPlayer, _widgetId, _vlcException); // VLC 1.0.*
		//libvlc_media_player_set_drawable(_vlcMediaPlayer, _widgetId, _vlcException); // VLC 0.9.*
#endif
		checkException();
	}

    playInternal();
}

void VlcInstance::unloadMedia() {
	if (_vlcMediaPlayer) {
		libvlc_media_player_release(_vlcMediaPlayer);
		_vlcMediaPlayer = NULL;
	}

	if (_vlcMedia) {
		libvlc_media_release(_vlcMedia);
		_vlcMedia = NULL;
	}
}

void VlcInstance::playInternal() {
	//_playRequestReached = true;

	//Clear subtitles/chapters...
	//clearMediaController();

	_vlcCurrentMediaPlayer = _vlcMediaPlayer;

	//setVLCWidgetId();

	//Play
	libvlc_media_player_play(_vlcMediaPlayer, _vlcException);
	checkException();

    _isPlaying = true;
}

void VlcInstance::pause() {
	libvlc_media_player_pause(_vlcMediaPlayer, _vlcException);
	checkException();
}

void VlcInstance::stop() {
	libvlc_media_player_stop(_vlcMediaPlayer, _vlcException);
	checkException();
}

void VlcInstance::checkException() {
	if (libvlc_exception_raised(_vlcException)) {
		qDebug() << "libvlc exception:" << libvlc_exception_get_message(_vlcException);
		libvlc_exception_clear(_vlcException);
	}
}
