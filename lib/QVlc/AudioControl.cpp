/****************************************************************************
* QVlc - Qt and libVLC connector library
* AudioControl.h: Audio controller
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

#include "AudioControl.h"
#include "Instance.h"

QVlc::AudioControl::AudioControl(QObject *parent)
	: QObject(parent), _actionList(QList<QAction*>()), _map(QMap<QString,int>()), _actionGroup(0)
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateActions()));

	_timer->start(2000);
}


QVlc::AudioControl::~AudioControl()
{
	delete _timer;

	for(int i=0; i<_actionList.size(); i++)
		delete _actionList[i];
	if(_actionGroup != 0)
		delete _actionGroup;
}

void QVlc::AudioControl::updateActions()
{
	for(int i=0; i<_actionList.size(); i++)
		delete _actionList[i];
	_actionList.clear();
	_map.clear();

	if(_actionGroup != 0)
		delete _actionGroup;
	_actionGroup = new QActionGroup(this);

	if(!Instance::isActive()) {
		emit actions("audio", _actionList);
		return;
	}

#if VLC_1_1
	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer) != 0) {
		libvlc_track_description_t *desc;
		desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer);
#else
	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
		libvlc_track_description_t *desc;
		desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer, _vlcException);
#endif
		_map.insert(QString().fromUtf8(desc->psz_name), 0);
		_actionList << new QAction(QString().fromUtf8(desc->psz_name), this);
#if VLC_1_1
		if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer) > 1) {
			for(int i = 1; i < libvlc_audio_get_track_count(_vlcCurrentMediaPlayer); i++) {
				desc = desc->p_next;
				_map.insert(QString().fromUtf8(desc->psz_name), i);
				_actionList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#else
		if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
			for(int i = 1; i < libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
				desc = desc->p_next;
				_map.insert(QString().fromUtf8(desc->psz_name), i);
				_actionList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#endif
	} else {
		emit actions("audio", _actionList);
		return;
	}

	Instance::checkError();

	for (int i = 0; i < _actionList.size(); ++i) {
		_actionList[i]->setCheckable(true);
		_actionGroup->addAction(_actionList[i]);
		connect(_actionList[i], SIGNAL(triggered()), this, SLOT(update()));
	}

#if VLC_1_1
	_actionList[libvlc_audio_get_track(_vlcCurrentMediaPlayer)]->setChecked(true);
#else
	_actionList[libvlc_audio_get_track(_vlcCurrentMediaPlayer, _vlcException)]->setChecked(true);
#endif
	Instance::checkError();

	emit actions("audio", _actionList);

	_timer->start(60000);
}

void QVlc::AudioControl::update()
{
	int id = _map.value(_actionGroup->checkedAction()->text());

#if VLC_1_1
	libvlc_audio_set_track(_vlcCurrentMediaPlayer, id);
#else
	libvlc_audio_set_track(_vlcCurrentMediaPlayer, id ,_vlcException);
#endif

	Instance::checkError();
}

void QVlc::AudioControl::mediaChange()
{
	_timer->start(2000);
}
