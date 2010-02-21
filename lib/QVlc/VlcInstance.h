/****************************************************************************
* QVlc - Qt and libVLC connector library
* VlcInstance.h: Main libVLC instance
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

#ifndef QVLC_VLCINSTANCE_H_
#define QVLC_VLCINSTANCE_H_

#include <QtCore/QObject>
#include <QtGui/QWidget>

#include <vlc/vlc.h>

#if VLC_TRUNK
#else
extern libvlc_exception_t *_vlcException;
#endif

extern libvlc_instance_t *_vlcInstance;
extern libvlc_media_player_t *_vlcCurrentMediaPlayer;

class VlcInstance : public QObject
{
Q_OBJECT
public:
	VlcInstance(const QList<const char *> &args, const WId &widget = NULL);
	~VlcInstance();

	void openMedia(const QString &media);
	static void checkError();
	static QString version();

public slots:
	void init();
	void play();
	void pause();
	void stop();

private:
	int fatalError();
	void unloadMedia();

	libvlc_media_t * _vlcMedia;
	WId _widgetId;
	QList<const char *> _args;
};

#endif // QVLC_VLCINSTANCE_H_
