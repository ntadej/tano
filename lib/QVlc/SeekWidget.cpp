/****************************************************************************
* QVlc - Qt and libVLC connector library
* SeekWidget.cpp: Seek widget
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

#include <QtCore/QTime>
#include <QtGui/QHBoxLayout>

#include "Instance.h"
#include "SeekWidget.h"

QVlc::SeekWidget::SeekWidget(QWidget *parent)
	:QWidget(parent)
{
	_seek = new QSlider(this);
	_seek->setOrientation(Qt::Horizontal);
	_seek->setMaximum(0);
	_seek->setPageStep(1000);

	_labelElapsed = new QLabel(this);
	_labelElapsed->setText("00:00:00");

	_labelFull = new QLabel(this);
	_labelFull->setText("00:00:00");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(_labelElapsed);
	layout->addWidget(_seek);
	layout->addWidget(_labelFull);
	setLayout(layout);

	_timer = new QTimer(this);

	connect(_timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	connect(_seek, SIGNAL(sliderReleased()), this, SLOT(changeTime()));

	_timer->start(400);
}

QVlc::SeekWidget::~SeekWidget()
{
	delete _seek;
	delete _labelElapsed;
	delete _labelFull;
	delete _timer;
}

void QVlc::SeekWidget::updateTime()
{
	if(!_vlcCurrentMediaPlayer)
		return;

	// It's possible that the vlc doesn't play anything
	// so check before
	libvlc_media_t *curMedia;
#if VLC_1_1
	curMedia = libvlc_media_player_get_media(_vlcCurrentMediaPlayer);
#else
	curMedia = libvlc_media_player_get_media(_vlcCurrentMediaPlayer, _vlcException);
	libvlc_exception_clear(_vlcException);
#endif

	if (curMedia == 0)
		return;

	libvlc_state_t state;
#if VLC_1_1
	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
#else
	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
#endif

	if(state != 0 && state != 6 && state != 7) {
		libvlc_time_t fullTime;
		libvlc_time_t currentTime;
#if VLC_1_1
		fullTime = libvlc_media_player_get_length(_vlcCurrentMediaPlayer);
		currentTime = libvlc_media_player_get_time(_vlcCurrentMediaPlayer);
#else
		fullTime = libvlc_media_player_get_length(_vlcCurrentMediaPlayer, _vlcException);
		currentTime = libvlc_media_player_get_time(_vlcCurrentMediaPlayer, _vlcException);
#endif

		_labelFull->setText(QTime(0,0,0,0).addMSecs(fullTime).toString("hh:mm:ss"));
		_seek->setMaximum(fullTime);

		_labelElapsed->setText(QTime(0,0,0,0).addMSecs(currentTime).toString("hh:mm:ss"));
		_seek->setValue(currentTime);
	} else {
		_labelFull->setText("00:00:00");
		_seek->setMaximum(0);

		_labelElapsed->setText("00:00:00");
		_seek->setValue(0);
	}

	Instance::checkError();
}

void QVlc::SeekWidget::changeTime()
{
	if(!_vlcCurrentMediaPlayer)
		return;

	_labelElapsed->setText(QTime(0,0,0,0).addMSecs(_seek->value()).toString("hh:mm:ss"));

#if VLC_1_1
	libvlc_media_player_set_time(_vlcCurrentMediaPlayer, _seek->value());
#else
	libvlc_exception_clear(_vlcException);
	libvlc_media_player_set_time(_vlcCurrentMediaPlayer, _seek->value(), _vlcException);
#endif

	Instance::checkError();
}
