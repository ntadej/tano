/****************************************************************************
* QVlc - Qt and libVLC connector library
* VlcControl.cpp: Subtitles and audio channels controller
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

#include "VlcControl.h"
#include "VlcInstance.h"

VlcControl::VlcControl(const QString &lang)
	: _audioGroup(0), _subGroup(0), _manualLanguage(false), _preferedLanguage(lang.split(" / "))
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateActionsAudio()));
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateActionsVideo()));

	_check = new QTimer(this);
	connect(_check, SIGNAL(timeout()), this, SLOT(checkPlayingState()));

	_check->start(300);
	_timer->start(2000);
}

VlcControl::~VlcControl()
{
	delete _timer;
	delete _check;

	for(int i=0; i<_audioList.size(); i++)
		delete _audioList[i];

	for(int i=0; i<_subList.size(); i++)
		delete _subList[i];
}

void VlcControl::updateActionsAudio() {
	for(int i=0; i<_audioList.size(); i++)
		delete _audioList[i];
	_audioList.clear();
	_audioMap.clear();

	if(_audioGroup != 0)
		delete _audioGroup;
	_audioGroup = new QActionGroup(this);

	if(!_vlcCurrentMediaPlayer)
		return;

	libvlc_state_t state;
	#if VLC_TRUNK
		state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
	#else
		state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
	#endif

	if(state == 0 || state == 6 || state == 7) {
		emit vlcAction("audio", _audioList);
		return;
	}

#if VLC_TRUNK
	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer) != 0) {
#else
	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
#endif
		libvlc_track_description_t *desc;
#if VLC_TRUNK
		desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer);
#else
		desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer, _vlcException);
#endif
		_audioMap.insert(QString().fromUtf8(desc->psz_name), 0);
		_audioList << new QAction(QString().fromUtf8(desc->psz_name), this);
#if VLC_TRUNK
		if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer) > 1) {
			for(int i = 1; i < libvlc_audio_get_track_count(_vlcCurrentMediaPlayer); i++) {
				desc = desc->p_next;
				_audioMap.insert(QString().fromUtf8(desc->psz_name), i);
				_audioList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#else
		if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
			for(int i = 1; i < libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
				desc = desc->p_next;
				_audioMap.insert(QString().fromUtf8(desc->psz_name), i);
				_audioList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#endif
	} else {
		emit vlcAction("audio", _audioList);
		return;
	}

	VlcInstance::checkError();

	for (int i = 0; i < _audioList.size(); ++i) {
		_audioList[i]->setCheckable(true);
		_audioGroup->addAction(_audioList[i]);
		connect(_audioList[i], SIGNAL(triggered()), this, SLOT(updateAudio()));
	}

#if VLC_TRUNK
	_audioList[libvlc_audio_get_track(_vlcCurrentMediaPlayer)]->setChecked(true);
#else
	_audioList[libvlc_audio_get_track(_vlcCurrentMediaPlayer, _vlcException)]->setChecked(true);
#endif
	VlcInstance::checkError();

	emit vlcAction("audio", _audioList);
}

void VlcControl::updateActionsVideo() {
	for(int i=0; i<_subList.size(); i++)
		delete _subList[i];
	_subList.clear();
	_subMap.clear();

	if(_subGroup != 0)
		delete _subGroup;
	_subGroup = new QActionGroup(this);

	if(!_vlcCurrentMediaPlayer)
		return;

	libvlc_state_t state;
	#if VLC_TRUNK
		state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
	#else
		state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
	#endif

	if(state == 0 || state == 6 || state == 7) {
		emit vlcAction("sub", _subList);
		return;
	}

#if VLC_TRUNK
	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer) != 0) {
#else
	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
#endif
		libvlc_track_description_t *desc;
#if VLC_TRUNK
		desc = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer);
#else
		desc = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer, _vlcException);
#endif
		_subMap.insert(QString().fromUtf8(desc->psz_name), 0);
		_subList << new QAction(QString().fromUtf8(desc->psz_name), this);
#if VLC_TRUNK
		if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer) > 1) {
			for(int i = 1; i < libvlc_video_get_spu_count(_vlcCurrentMediaPlayer); i++) {
				desc = desc->p_next;
				_subMap.insert(QString().fromUtf8(desc->psz_name), i);
				_subList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#else
		if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
			for(int i = 1; i < libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
				desc = desc->p_next;
				_subMap.insert(QString().fromUtf8(desc->psz_name), i);
				_subList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#endif
	} else {
		emit vlcAction("sub", _subList);
		return;
	}

	VlcInstance::checkError();

	for (int i = 0; i < _subList.size(); ++i) {
		_subList[i]->setCheckable(true);
		_subGroup->addAction(_subList[i]);
		connect(_subList[i], SIGNAL(triggered()), this, SLOT(updateSub()));

		if(!_manualLanguage)
			for(int j = 0; j < _preferedLanguage.size(); ++j)
				if(_subList[i]->text().contains(_preferedLanguage[j],Qt::CaseInsensitive)) {
					_subList[i]->trigger();
					_manualLanguage = true;
				}
	}
#if VLC_TRUNK
	_subList[libvlc_video_get_spu(_vlcCurrentMediaPlayer)]->setChecked(true);
#else
	_subList[libvlc_video_get_spu(_vlcCurrentMediaPlayer, _vlcException)]->setChecked(true);
#endif
	VlcInstance::checkError();

	emit vlcAction("sub", _subList);
}

void VlcControl::updateAudio()
{
	int id = _audioMap.value(_audioGroup->checkedAction()->text());

#if VLC_TRUNK
	libvlc_audio_set_track(_vlcCurrentMediaPlayer, id);
#else
	libvlc_audio_set_track(_vlcCurrentMediaPlayer, id ,_vlcException);
#endif

	VlcInstance::checkError();
}

void VlcControl::updateSub()
{
	int id = _subMap.value(_subGroup->checkedAction()->text());

#if VLC_TRUNK
	libvlc_video_set_spu(_vlcCurrentMediaPlayer, id);
#else
	libvlc_video_set_spu(_vlcCurrentMediaPlayer, id ,_vlcException);
#endif

	VlcInstance::checkError();
}

void VlcControl::checkPlayingState()
{
	if(!_vlcCurrentMediaPlayer)
		return;

	libvlc_state_t state;
#if VLC_TRUNK
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

void VlcControl::update()
{
	_manualLanguage = false;
}
