/****************************************************************************
* QVlc - Qt and libVLC connector library
* VlcInstance.cpp: Main libVLC instance
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

#include "VlcInstance.h"

libvlc_instance_t * _vlcInstance = NULL;

#if VLC_TRUNK
#else
libvlc_exception_t * _vlcException = new libvlc_exception_t();
#endif

libvlc_media_player_t * _vlcCurrentMediaPlayer = NULL;

VlcInstance::VlcInstance(const QList<const char *> &args, const WId &widget)
	:_widgetId(widget), _args(args)
{
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
		vlcArgs[i] = _args[i];

#if VLC_TRUNK
	_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs);
#else
	libvlc_exception_init(_vlcException);
	_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(*vlcArgs), vlcArgs, _vlcException);
#endif
	checkError();

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

void VlcInstance::openMedia(const QString &media)
{
	unloadMedia();

#if VLC_TRUNK
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
#if defined(Q_OS_WIN)
	#if VLC_TRUNK
		libvlc_media_player_set_hwnd(_vlcCurrentMediaPlayer, _widgetId);
	#else
		libvlc_media_player_set_hwnd(_vlcCurrentMediaPlayer, _widgetId, _vlcException);
	#endif
#elif defined(Q_OS_MAC)
	#if VLC_TRUNK
		libvlc_media_player_set_agl(_vlcCurrentMediaPlayer, _widgetId);
	#else
		libvlc_media_player_set_agl(_vlcCurrentMediaPlayer, _widgetId, _vlcException);
	#endif
#else
	#if VLC_TRUNK
		libvlc_media_player_set_xwindow(_vlcCurrentMediaPlayer, _widgetId);
	#else
		libvlc_media_player_set_xwindow(_vlcCurrentMediaPlayer, _widgetId, _vlcException);
	#endif
#endif

		checkError();
	}

	play();
}

void VlcInstance::unloadMedia() {
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

void VlcInstance::play() {
#if VLC_TRUNK
	libvlc_media_player_play(_vlcCurrentMediaPlayer);
#else
	libvlc_media_player_play(_vlcCurrentMediaPlayer, _vlcException);
#endif
	checkError();
}

void VlcInstance::pause() {
#if VLC_TRUNK
	if(libvlc_media_player_can_pause(_vlcCurrentMediaPlayer) == 1)
		libvlc_media_player_pause(_vlcCurrentMediaPlayer);
#else
	if(libvlc_media_player_can_pause(_vlcCurrentMediaPlayer, _vlcException) == 1)
		libvlc_media_player_pause(_vlcCurrentMediaPlayer, _vlcException);
#endif
	checkError();
}

void VlcInstance::stop() {
	if(!_vlcCurrentMediaPlayer)
		return;

#if VLC_TRUNK
	libvlc_media_player_stop(_vlcCurrentMediaPlayer);
#else
	libvlc_media_player_stop(_vlcCurrentMediaPlayer, _vlcException);
#endif
	unloadMedia();

	checkError();
}

void VlcInstance::checkError() {

#if VLC_TRUNK
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

int VlcInstance::fatalError() {
	 int ret = QMessageBox::critical(0, "libVLC Initialization",
									"libVLC could not be initialized successfully.\n"
									"The application will now exit.",
									QMessageBox::Ok | QMessageBox::Cancel,
									QMessageBox::Ok);
	 return ret;
}
