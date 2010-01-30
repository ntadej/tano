#include <QtCore/QDebug>
#include <QtGui/QMessageBox>

#include "VlcInstance.h"

libvlc_instance_t * _vlcInstance = NULL;
libvlc_exception_t * _vlcException = new libvlc_exception_t();
libvlc_media_player_t * _vlcCurrentMediaPlayer = NULL;

VlcInstance::VlcInstance(QStringList args, WId widget)
	:_widgetId(widget), _args(args)
{
	_vlcMediaPlayer = NULL;
	_vlcMedia = NULL;
}

VlcInstance::~VlcInstance()
{
	libvlc_release(_vlcInstance);
}

QString VlcInstance::version()
{
	return QString(libvlc_get_version());
}

void VlcInstance::init()
{
	const char *vlcArgs[_args.size()];
	for(int i=0; i<_args.size(); i++)
		vlcArgs[i] = _args[i].toUtf8();

	libvlc_exception_init(_vlcException);

	_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs, _vlcException);
	checkException();

	if(_vlcInstance) {
		qDebug() << "Using libVLC version:" << libvlc_get_version();
		qDebug() << "libVLC loaded";
	} else {
		qDebug() << "QVlc Error: libVLC failed to load!";
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

    qDebug() << "libVlc" <<"Media:" << media;

    _vlcMediaPlayer = libvlc_media_player_new_from_media (_vlcMedia, _vlcException);
    checkException();

	/* Get our media instance to use our window */
	if (_vlcMediaPlayer) {
#if defined(Q_OS_WIN)
	#if VLC_TRUNK
		libvlc_media_player_set_hwnd(_vlcMediaPlayer, _widgetId);
	#else
		libvlc_media_player_set_hwnd(_vlcMediaPlayer, _widgetId, _vlcException);
	#endif
#elif defined(Q_OS_MAC)
	#if VLC_TRUNK
		libvlc_media_player_set_agl (_vlcMediaPlayer, _widgetId);
	#else
		libvlc_media_player_set_agl (_vlcMediaPlayer, _widgetId, _vlcException);
	#endif
#else
	#if VLC_TRUNK
		libvlc_media_player_set_xwindow(_vlcMediaPlayer, _widgetId);
	#else
		libvlc_media_player_set_xwindow(_vlcMediaPlayer, _widgetId, _vlcException);
	#endif
#endif
		checkException();
	}

	_vlcCurrentMediaPlayer = _vlcMediaPlayer;

	play();
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

void VlcInstance::play() {
	libvlc_media_player_play(_vlcMediaPlayer, _vlcException);
	checkException();
}

void VlcInstance::pause() {
	if(libvlc_media_player_can_pause(_vlcMediaPlayer, _vlcException) == 1)
		libvlc_media_player_pause(_vlcMediaPlayer, _vlcException);
	checkException();
}

void VlcInstance::stop() {
#if VLC_TRUNK
	libvlc_media_player_stop(_vlcMediaPlayer);
#else
	libvlc_media_player_stop(_vlcMediaPlayer, _vlcException);
#endif
	checkException();
}

void VlcInstance::mute() {
#if VLC_TRUNK
	libvlc_audio_toggle_mute(_vlcInstance);
#else
	libvlc_audio_toggle_mute(_vlcInstance, _vlcException);
#endif
	checkException();
}

void VlcInstance::checkException() {
	if (libvlc_exception_raised(_vlcException)) {
#if VLC_TRUNK
		qDebug() << "libVLC" << "Exception:" << libvlc_errmsg();
#else
		qDebug() << "libVLC" << "Exception:" << libvlc_exception_get_message(_vlcException);
#endif
		libvlc_exception_clear(_vlcException);
	}
}

int VlcInstance::fatalError() {
	 int ret = QMessageBox::critical(0, "libVLC Initialization",
	                                "libVLC could not be initialized successfully.\n"
	                                   "The application will now exit.",
	                                QMessageBox::Ok | QMessageBox::Cancel,
	                                QMessageBox::Ok);
	 return ret;
}
