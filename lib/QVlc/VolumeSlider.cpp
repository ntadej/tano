/****************************************************************************
* QVlc - Qt and libVLC connector library
* VVolumeSlider.cpp: Volume manager and slider
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

#include <QtGui/QHBoxLayout>

#include "Instance.h"
#include "VolumeSlider.h"

QVlc::VolumeSlider::VolumeSlider(QWidget *parent)
	: QWidget(parent)
{
	_slider = new QSlider(this);
	_slider->setOrientation(Qt::Horizontal);
	_slider->setMaximum(200);

	_label = new QLabel(this);
	_label->setMinimumWidth(20);
	_label->setText(QString().number(0));

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(_slider);
	layout->addWidget(_label);
	setLayout(layout);

	_timer = new QTimer(this);

	connect(_timer, SIGNAL(timeout()), this, SLOT(updateVolume()));
	connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(setVolume(int)));

	_timer->start(100);
}

QVlc::VolumeSlider::~VolumeSlider()
{
	delete _slider;
	delete _label;
	delete _timer;
}

void QVlc::VolumeSlider::setVolume(const int &volume)
{
	_currentVolume = volume;
	_slider->setValue(_currentVolume);
	_label->setText(QString().number(_currentVolume));
}

void QVlc::VolumeSlider::updateVolume()
{
	if(!Instance::isActive())
		return;

	int volume;
#if VLC_1_1
	volume = libvlc_audio_get_volume(_vlcCurrentMediaPlayer);
#else
	volume = libvlc_audio_get_volume(_vlcInstance, _vlcException);
#endif

	if(volume != _currentVolume) {
#if VLC_1_1
		libvlc_audio_set_volume (_vlcCurrentMediaPlayer, _currentVolume);
#else
		libvlc_exception_clear(_vlcException);
		libvlc_audio_set_volume (_vlcInstance, _currentVolume, _vlcException);
#endif
	}

	Instance::checkError();
}

void QVlc::VolumeSlider::volumeControl(const bool &up)
{
	if(up)
		if(_currentVolume != 200)
			_currentVolume+=1;
	else
		if(_currentVolume != 0)
			_currentVolume-=1;
}

void QVlc::VolumeSlider::mute()
{
	int isMute;
#if VLC_1_1
	isMute = libvlc_audio_get_mute(_vlcCurrentMediaPlayer);
#else
	isMute = libvlc_audio_get_mute(_vlcInstance, _vlcException);
#endif

	Instance::checkError();

	if(isMute == 1) {
		_timer->start(100);
		_slider->setDisabled(false);
		_label->setDisabled(false);
	} else {
		_timer->stop();
		_slider->setDisabled(true);
		_label->setDisabled(true);
	}

#if VLC_1_1
	libvlc_audio_toggle_mute(_vlcCurrentMediaPlayer);
#else
	libvlc_audio_toggle_mute(_vlcInstance, _vlcException);
#endif

	Instance::checkError();
}
