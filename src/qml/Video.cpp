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

#include <QtCore/QDebug>

#include "core/Common.h"
#include "qml/Video.h"

VideoDisplay::VideoDisplay(QGraphicsItem* parent)
	: QGraphicsProxyWidget(parent)
{
	_width = 640;
	_height = 480;
	// This is the widget where the video will be drawn
	// The advantage of a QLabel over a QWidget is that
	// we can easily push a QPixmap into.
	_video = new QLabel();
	_video->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setWidget(_video);

	connect(this, SIGNAL(frameReady(struct ctx*)), this, SLOT(processFrame(struct ctx*)));

	// Init libvlc and play
	init();
	play();
}

VideoDisplay::~VideoDisplay()
{
	delete _video;
}

void VideoDisplay::setHeight(const int height)
{
	_height = height;
	//processSize();
}

void VideoDisplay::setWidth(const int width)
{
	_width = width;
	//processSize();
}

void VideoDisplay::processSize()
{
	// We now need a struct for storing the video buffer
	// and a mutex to protect it.
	struct ctx* context;
	// Allocating the space for the structure
	context = ( struct ctx* )malloc( sizeof( *context ) );
	// Allocating the video buffer
	context->pixels = ( uchar* )malloc( ( sizeof( *( context->pixels ) ) * _width * _height ) * 4 );
	// Allocating the mutex
	context->mutex = new QMutex();
	context->display = this;

	libvlc_media_player_pause(_vlcMediaplayer);
	libvlc_video_set_callbacks(_vlcMediaplayer, lock, unlock, display, context);
	libvlc_video_set_format(_vlcMediaplayer, "RV32", _width, _height, _width*4);
	libvlc_media_player_play(_vlcMediaplayer);
}

void VideoDisplay::init()
{
	// List of parameters used to initialize libvlc.
	// These arguments are same as those you can pass
	// the the VLC command line.
	char const* vlc_argv[] = { };
	int vlc_argc = sizeof(vlc_argv) / sizeof(*vlc_argv);

	// Create a libvlc instance
	_vlcInstance = libvlc_new(vlc_argc, vlc_argv);
	catchException();
	// Create the mediaplayer used to play a media
	_vlcMediaplayer = libvlc_media_player_new(_vlcInstance);
	catchException();
}

void VideoDisplay::play()
{
	QString mrl;
	mrl = "udp://@232.4.1.1:5002";

	// Create a new media from the Media Resource Locator
	_vlcMedia = libvlc_media_new_location(_vlcInstance, mrl.toAscii());
	catchException();

	// Put the media into the mediaplayer
	libvlc_media_player_set_media(_vlcMediaplayer, _vlcMedia);
	catchException();

	// Start the playback.
	processSize();
	libvlc_media_player_play(_vlcMediaplayer);
	catchException();
}

void *VideoDisplay::lock(void *data, void **p_pixels)
{
	struct ctx *ctx = (struct ctx*) data;
	// Lock the buffer (to avoid concurrent access and data corruption)
	ctx->mutex->lock();
	// Tell libvlc to write the next frame into our pre-allocated buffer
	*p_pixels = ctx->pixels;

	return NULL;
}

void VideoDisplay::unlock(void *data, void *id, void *const*p_pixels)
{
	struct ctx *ctx = (struct ctx*) data;
	// As we are in a static method we don't have an instance
	// of the VideoDisplay here. Fortunately we created a copy
	// of our instance pointer into the ctx structure
	ctx->display->emit frameReady( ctx );
}

void VideoDisplay::display(void *data, void *id)
{
	// VLC wants to display the video
	(void) data;
	Q_ASSERT(id == NULL);
}

void VideoDisplay::processFrame(struct ctx *data)
{
	// WARNING:
	// This part of the code is extremely slow and inefficient

	// Conversion of the frame from our pixel buffer to a QImage
	QImage px(data->pixels, _width, _height, QImage::Format_RGB32);
	// Then we're creating a new QPixmap from our QImage
	QPixmap pix = QPixmap::fromImage(px);

	// Set the current frame into the video container
	_video->setPixmap(pix);

	// Releasing the mutex for the upcoming frame.
	data->mutex->unlock();
}

void VideoDisplay::catchException()
{
	qDebug() << libvlc_errmsg();
}
