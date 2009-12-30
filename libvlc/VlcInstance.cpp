#include <QDebug>
#include <QMessageBox>

#include "VlcInstance.h"

libvlc_instance_t * _vlcInstance = NULL;
libvlc_exception_t * _vlcException = new libvlc_exception_t();
libvlc_media_player_t * _vlcCurrentMediaPlayer = NULL;

VlcInstance::VlcInstance(WId widget, QString iface)
{
	_vlcInstance = NULL;
	_vlcException = new libvlc_exception_t();

	_vlcMediaPlayer = NULL;
	_vlcMedia = NULL;

	_widgetId = widget;
	_isPlaying = false;
	_networkInterface = iface;
}

VlcInstance::~VlcInstance()
{
	libvlc_release(_vlcInstance);
}

void VlcInstance::init()
{
	const char * vlcArgs[11];
	int argsVlc;

#ifdef Q_WS_X11
	argsVlc = 9;
	vlcArgs[7] = "--vout-event";
	vlcArgs[8] = "3";
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

	if(_networkInterface != "") {
		vlcArgs[argsVlc] = "--miface-addr";
		vlcArgs[argsVlc+1] = _networkInterface.toUtf8().data();
		qDebug() << "Using network interface:" << vlcArgs[argsVlc+1];
	} else {
		vlcArgs[argsVlc] = "";
		vlcArgs[argsVlc+1] = "";
	}

	libvlc_exception_init(_vlcException);

	_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs, _vlcException);
	checkException();

	if(_vlcInstance) {
		qDebug() << "Using VLC version:" << libvlc_get_version();
		qDebug() << "VLC loaded";
	} else {
		qDebug() << "Error: VLC failed to load!!!";
		switch(fatalError()) {
		case QMessageBox::Ok:
			exit(-100);
			break;
		default:
			break;
		};
	}
}

void VlcInstance::openMedia(QString media)
{
	unloadMedia();
    /* Create a new LibVLC media descriptor */
    _vlcMedia = libvlc_media_new (_vlcInstance, media.toAscii(), _vlcException);
    checkException();

    qDebug() << "Media:" << media;

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
	if(libvlc_media_player_can_pause(_vlcMediaPlayer, _vlcException) == 1)
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

int VlcInstance::fatalError() {
	 int ret = QMessageBox::critical(0, tr("VLC Initialization"),
	                                tr("VLC could not be initialized successfully.\n"
	                                   "Tano Player will now exit."),
	                                QMessageBox::Ok | QMessageBox::Cancel,
	                                QMessageBox::Ok);
	 return ret;
}
