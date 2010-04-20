/****************************************************************************
* QVlc - Qt and libVLC connector library
* Instance.h: Main libVLC instance
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
#include <QtCore/QTimer>
#include <QtGui/QWidget>

#include "Config.h"

#include <vlc/vlc.h>

#if VLC_1_0
extern libvlc_exception_t *_vlcException;
#endif

extern libvlc_instance_t *_vlcInstance;
extern libvlc_media_player_t *_vlcCurrentMediaPlayer;

namespace QVlc
{
	class Instance : public QObject
	{
	Q_OBJECT
	public:
		Instance(const QList<const char *> &args, const WId &widget = NULL, QObject *parent = NULL);
		~Instance();

		void openMedia(const QString &media);
		static bool isActive();
		static void checkError();
		static QString version();
		static QString libVlcVersion();

	public slots:
		void init();
		void play();
		void pause();
		void stop();

	signals:
		void state(const bool, const bool, const bool);

	private slots:
		void checkPlayingState();

	private:
		int fatalError() const;
		void unloadMedia();

		libvlc_media_t * _vlcMedia;
		WId _widgetId;
		QList<const char *> _args;

		QTimer *_check;
	};
};

#endif // QVLC_VLCINSTANCE_H_
