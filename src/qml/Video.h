/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak
*
* This file is based on vmem-example
* Copyright (c) 2009 Ludovic Fauvet
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANOQML_VIDEO_H_
#define TANOQML_VIDEO_H_

#include <QtCore/QMutex>
#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QLabel>

// libvlc include
#include <vlc/vlc.h>
#include <vlc/libvlc.h>

class VideoDisplay : public QGraphicsProxyWidget
{
Q_OBJECT

//Q_PROPERTY(int videoHeight READ height WRITE setHeight)
//Q_PROPERTY(int videoWidth READ width WRITE setWidth)

public:
	VideoDisplay(QGraphicsItem *parent = 0);
	~VideoDisplay();

	int height() const { return _height; }
	void setHeight(const int height);
	int width() const { return _width; }
	void setWidth(const int width);

signals:
	void frameReady(struct ctx*);

private slots:
	void play();
	void processFrame(struct ctx *data);

private:
	void catchException();
	void init();
	void processSize();

	// VLC stuff
	static void display(void *data, void *id);
	static void *lock(void *data, void **p_pixels);
	static void unlock(void *data, void *id, void *const *p_pixels);
	libvlc_instance_t *_vlcInstance;
	libvlc_media_t *_vlcMedia;
	libvlc_media_player_t *_vlcMediaplayer;

	// GUI
	QLabel* _video;

	int _width;
	int _height;
};

// Struct used for lock/unlock
struct ctx
{
	uchar *pixels;
	QMutex *mutex;
	VideoDisplay *display;
};

#endif // MAINWINDOW_H
