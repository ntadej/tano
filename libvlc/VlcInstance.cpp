#include <QDebug>

#include "VlcInstance.h"

libvlc_instance_t * _vlcInstance = NULL;
libvlc_exception_t * _vlcException = new libvlc_exception_t();
libvlc_media_player_t * _vlcCurrentMediaPlayer = NULL;

VlcInstance::VlcInstance(WId widget, QString iface)
{
	int argsVlc;

	_vlcInstance = NULL;
	_vlcException = new libvlc_exception_t();

	_vlcMediaPlayer = NULL;
	_vlcMedia = NULL;

	_widgetId = widget;

	const char * vlcArgs[12];

#ifdef Q_WS_X11
	argsVlc = 11;
	vlcArgs[7] = "--vout-event";
	vlcArgs[8] = "3";
	vlcArgs[9] = "-V";
	vlcArgs[10] = "x11";
#else
	argsVlc = 7;
#endif
	vlcArgs[0] = "--intf=dummy";
	vlcArgs[1] = "--no-media-library";
	vlcArgs[2] = "--reset-plugins-cache";
	vlcArgs[3] = "--no-stats";
	vlcArgs[4] = "--no-osd";
	vlcArgs[5] = "--no-video-title-show";
	vlcArgs[6] = "--ignore-config";

	if(iface != "")
		vlcArgs[argsVlc] = iface.toLocal8Bit().data();

	libvlc_exception_init(_vlcException);

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
	_vlcCurrentMediaPlayer = _vlcMediaPlayer;

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

void VlcInstance::mute() {
	libvlc_audio_toggle_mute(_vlcInstance, _vlcException);
	checkException();
}

void VlcInstance::checkException() {
	if (libvlc_exception_raised(_vlcException)) {
		qDebug() << "libvlc exception:" << libvlc_exception_get_message(_vlcException);
		libvlc_exception_clear(_vlcException);
	}
}
