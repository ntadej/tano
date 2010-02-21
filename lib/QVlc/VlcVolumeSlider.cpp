/****************************************************************************
* QVlc - Qt and libVLC connector library
* VlcVolumeSlider.cpp: Volume manager and slider
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

#include "VlcInstance.h"
#include "VlcVolumeSlider.h"

VlcVolumeSlider::VlcVolumeSlider(QWidget *parent)
	: QWidget(parent)
{
	_slider = new QSlider(this);
	_slider->setOrientation(Qt::Horizontal);
	_slider->setMaximum(200);

	_label = new QLabel(this);
	_label->setMinimumWidth(20);
	_label->setText(QString().number(50));

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(_slider);
	layout->addWidget(_label);
	setLayout(layout);

	_timer = new QTimer(this);

	connect(_timer, SIGNAL(timeout()), this, SLOT(updateVolume()));
	connect(_slider, SIGNAL(valueChanged(int)), this, SLOT(changeVolume(int)));

	_timer->start(100);
}

VlcVolumeSlider::~VlcVolumeSlider() {
	delete _slider;
	delete _label;
	delete _timer;
}

void VlcVolumeSlider::changeVolume(const int &newVolume)
{
	if(!_vlcCurrentMediaPlayer)
		return;

	_label->setText(QString().number(newVolume));

#if VLC_TRUNK
	libvlc_audio_set_volume (_vlcCurrentMediaPlayer, newVolume);
#else
	libvlc_exception_clear(_vlcException);
	libvlc_audio_set_volume (_vlcInstance, newVolume, _vlcException);
#endif

	VlcInstance::checkError();
}

void VlcVolumeSlider::updateVolume()
{
	if(!_vlcCurrentMediaPlayer)
		return;

	// It's possible that the vlc doesn't play anything
	// so check before
	libvlc_media_t *curMedia;
#if VLC_TRUNK
	curMedia = libvlc_media_player_get_media(_vlcCurrentMediaPlayer);
#else
	curMedia = libvlc_media_player_get_media(_vlcCurrentMediaPlayer, _vlcException);
	libvlc_exception_clear(_vlcException);
#endif

	if (curMedia == NULL)
		return;

	int volume;
#if VLC_TRUNK
	volume = libvlc_audio_get_volume(_vlcCurrentMediaPlayer);
#else
	volume = libvlc_audio_get_volume(_vlcInstance, _vlcException);
#endif
	_slider->setValue(volume);
	_label->setText(QString().number(volume));

	VlcInstance::checkError();
}

void VlcVolumeSlider::setVolume(const int &volume)
{
	_slider->setValue(volume);
	_label->setText(QString().number(volume));
}

void VlcVolumeSlider::volumeControl(const bool &direction)
{
	if(direction)
		vup();
	else
		vdown();
}

void VlcVolumeSlider::vup()
{
	if(_slider->value() != 200)
		changeVolume(_slider->value()+1);
}

void VlcVolumeSlider::vdown()
{
	if(_slider->value() != 0)
		changeVolume(_slider->value()-1);
}

void VlcVolumeSlider::mute() {
#if VLC_TRUNK
	libvlc_audio_toggle_mute(_vlcCurrentMediaPlayer);
#else
	libvlc_audio_toggle_mute(_vlcInstance, _vlcException);
#endif

	VlcInstance::checkError();
}
