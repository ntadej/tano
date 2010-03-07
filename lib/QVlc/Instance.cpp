/****************************************************************************
* QVlc - Qt and libVLC connector library
* Instance.cpp: Main libVLC instance
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtCore/QDebug>
#include <QtGui/QMessageBox>

#include "Config.h"
#include "Instance.h"

libvlc_instance_t * _vlcInstance = NULL;

#if VLC_1_0
libvlc_exception_t * _vlcException = new libvlc_exception_t();
#endif

libvlc_media_player_t * _vlcCurrentMediaPlayer = NULL;

QVlc::Instance::Instance(const QList<const char *> &args, const WId &widget, QObject *parent)
	: QObject(parent), _widgetId(widget), _args(args)
{
	_vlcMedia = NULL;

	_check = new QTimer(this);
	connect(_check, SIGNAL(timeout()), this, SLOT(checkPlayingState()));
	_check->start(300);
}

QVlc::Instance::~Instance()
{
	delete _check;

	libvlc_release(_vlcInstance);
}

QString QVlc::Instance::version()
{
#ifdef LIBQVLC_VERSION
#if LIBQVLC_REVISION
	return QString(QString(LIBQVLC_VERSION)+"-r"+QString().number(LIBQVLC_REVISION));
#endif //LIBQVLC_REVISION
#else
	return QString("Unknown");
#endif //LIBQVLC_VERSION
}

QString QVlc::Instance::libVlcVersion()
{
	return QString(libvlc_get_version());
}

void QVlc::Instance::init()
{
	const char *vlcArgs[_args.size()];
	for(int i=0; i<_args.size(); i++)
		vlcArgs[i] = _args[i];

#if VLC_1_1
	_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs);
#else
	libvlc_exception_init(_vlcException);
	_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs, _vlcException);
#endif
	checkError();

	if(_vlcInstance) {
		qDebug() << "libQVlc" << version() << "initialised";
		qDebug() << "Using libVLC version:" << libvlc_get_version();
		qDebug() << "libVLC loaded";
	} else {
		qDebug() << "libQVlc Error: libVLC failed to load!";
		switch(fatalError()) {
		case QMessageBox::Ok:
			exit(-100);
			break;
		default:
			break;
		};
	}
}

void QVlc::Instance::openMedia(const QString &media)
{
	unloadMedia();

#if VLC_1_1
	/* Create a new LibVLC media descriptor */
	_vlcMedia = libvlc_media_new_location(_vlcInstance, media.toAscii());
	checkError();
	qDebug() << "libVlc" <<"Media:" << media;

	_vlcCurrentMediaPlayer = libvlc_media_player_new_from_media(_vlcMedia);
	checkError();
#else
	/* Create a new LibVLC media descriptor */
	_vlcMedia = libvlc_media_new(_vlcInstance, media.toAscii(), _vlcException);
	checkError();

	qDebug() << "libVlc" <<"Media:" << media;

	_vlcCurrentMediaPlayer = libvlc_media_player_new_from_media(_vlcMedia, _vlcException);
	checkError();
#endif

	/* Get our media instance to use our window */
	if (_vlcCurrentMediaPlayer) {
#if defined(Q_WS_WIN)
#if VLC_1_1
		libvlc_media_player_set_hwnd(_vlcCurrentMediaPlayer, _widgetId);
#else //VLC_1_1
		libvlc_media_player_set_hwnd(_vlcCurrentMediaPlayer, _widgetId, _vlcException);
#endif //VLC_1_1
#elif defined(Q_WS_MAC)
#if VLC_1_1
		libvlc_media_player_set_agl(_vlcCurrentMediaPlayer, _widgetId);
#else //VLC_1_1
		libvlc_media_player_set_agl(_vlcCurrentMediaPlayer, _widgetId, _vlcException);
#endif //VLC_1_1
#else // Q_WS_X11
#if VLC_1_1
		libvlc_media_player_set_xwindow(_vlcCurrentMediaPlayer, _widgetId);
#else //VLC_1_1
		libvlc_media_player_set_xwindow(_vlcCurrentMediaPlayer, _widgetId, _vlcException);
#endif //VLC_1_1
#endif // Q_WS_*

		checkError();
	}

	play();
}

void QVlc::Instance::unloadMedia()
{
	if (_vlcCurrentMediaPlayer) {
		libvlc_media_player_release(_vlcCurrentMediaPlayer);
		_vlcCurrentMediaPlayer = NULL;
	}

	if (_vlcMedia) {
		libvlc_media_release(_vlcMedia);
		_vlcMedia = NULL;
	}

	checkError();
}

void QVlc::Instance::play()
{
	if(_vlcCurrentMediaPlayer == NULL)
		return;

#if VLC_1_1
	libvlc_media_player_play(_vlcCurrentMediaPlayer);
#else
	libvlc_media_player_play(_vlcCurrentMediaPlayer, _vlcException);
#endif
	checkError();
}

void QVlc::Instance::pause()
{
	if(_vlcCurrentMediaPlayer == NULL)
		return;

#if VLC_1_1
	if(libvlc_media_player_can_pause(_vlcCurrentMediaPlayer) == 1)
		libvlc_media_player_pause(_vlcCurrentMediaPlayer);
#else
	if(libvlc_media_player_can_pause(_vlcCurrentMediaPlayer, _vlcException) == 1)
		libvlc_media_player_pause(_vlcCurrentMediaPlayer, _vlcException);
#endif
	checkError();
}

void QVlc::Instance::stop()
{
	if(_vlcCurrentMediaPlayer == NULL)
		return;

#if VLC_1_1
	libvlc_media_player_stop(_vlcCurrentMediaPlayer);
#else
	libvlc_media_player_stop(_vlcCurrentMediaPlayer, _vlcException);
#endif
	unloadMedia();

	checkError();
}

void QVlc::Instance::checkPlayingState()
{
	if(!_vlcCurrentMediaPlayer) {
		emit stateChanged(0);
		return;
	}

	libvlc_state_t state;
#if VLC_1_1
	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
#else
	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
#endif

	if(state == 3) {
		emit stateChanged(1);
	} else {
		emit stateChanged(0);
	}
}

void QVlc::Instance::checkError()
{

#if VLC_1_1
	if(libvlc_errmsg() != NULL) {
		qDebug() << "libVLC" << "Error:" << libvlc_errmsg();
		libvlc_clearerr();
	}
#else
	if (libvlc_exception_raised(_vlcException)) {
		qDebug() << "libVLC" << "Error:" << libvlc_exception_get_message(_vlcException);
		libvlc_exception_clear(_vlcException);
	}
#endif
}

int QVlc::Instance::fatalError() const
{
	 int ret = QMessageBox::critical(0, "libVLC Initialization",
									"libVLC could not be initialized successfully.\n"
									"The application will now exit.",
									QMessageBox::Ok | QMessageBox::Cancel,
									QMessageBox::Ok);
	 return ret;
}
