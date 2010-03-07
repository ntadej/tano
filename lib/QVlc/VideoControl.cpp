/****************************************************************************
* QVlc - Qt and libVLC connector library
* VideoControl.h: Video and subtitles controller
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

#include "Instance.h"
#include "VideoControl.h"

QVlc::VideoControl::VideoControl(const QString &lang, QObject *parent)
	: QObject(parent), _actionGroup(0), _manualLanguage(false), _preferedLanguage(lang.split(" / "))
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateActions()));

	_timer->start(2000);
}

QVlc::VideoControl::~VideoControl()
{
	delete _timer;

	for(int i=0; i<_actionList.size(); i++)
		delete _actionList[i];
}

void QVlc::VideoControl::updateActions() {
	for(int i=0; i<_actionList.size(); i++)
		delete _actionList[i];
	_actionList.clear();
	_map.clear();

	if(_actionGroup != 0)
		delete _actionGroup;
	_actionGroup = new QActionGroup(this);

	if(!_vlcCurrentMediaPlayer)
		return;

	libvlc_state_t state;
	#if VLC_1_1
		state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
	#else
		state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
	#endif

	if(state == 0 || state == 6 || state == 7) {
		emit subtitlesActions("sub", _actionList);
		return;
	}

#if VLC_1_1
	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer) != 0) {
#else
	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
#endif
		libvlc_track_description_t *desc;
#if VLC_1_1
		desc = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer);
#else
		desc = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer, _vlcException);
#endif
		_map.insert(QString().fromUtf8(desc->psz_name), 0);
		_actionList << new QAction(QString().fromUtf8(desc->psz_name), this);
#if VLC_1_1
		if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer) > 1) {
			for(int i = 1; i < libvlc_video_get_spu_count(_vlcCurrentMediaPlayer); i++) {
				desc = desc->p_next;
				_map.insert(QString().fromUtf8(desc->psz_name), i);
				_actionList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#else
		if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
			for(int i = 1; i < libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
				desc = desc->p_next;
				_map.insert(QString().fromUtf8(desc->psz_name), i);
				_actionList << new QAction(QString().fromUtf8(desc->psz_name), this);
			}
		}
#endif
	} else {
		emit subtitlesActions("sub", _actionList);
		return;
	}

	VlcInstance::checkError();

	for (int i = 0; i < _actionList.size(); ++i) {
		_actionList[i]->setCheckable(true);
		_actionGroup->addAction(_actionList[i]);
		connect(_actionList[i], SIGNAL(triggered()), this, SLOT(update()));

		if(!_manualLanguage)
			for(int j = 0; j < _preferedLanguage.size(); ++j)
				if(_actionList[i]->text().contains(_preferedLanguage[j],Qt::CaseInsensitive)) {
					_actionList[i]->trigger();
					_manualLanguage = true;
				}
	}
#if VLC_1_1
	_actionList[libvlc_video_get_spu(_vlcCurrentMediaPlayer)]->setChecked(true);
#else
	_actionList[libvlc_video_get_spu(_vlcCurrentMediaPlayer, _vlcException)]->setChecked(true);
#endif
	VlcInstance::checkError();

	emit subtitlesActions("sub", _actionList);
}

void QVlc::VideoControl::update()
{
	int id = _map.value(_actionGroup->checkedAction()->text());

#if VLC_1_1
	libvlc_video_set_spu(_vlcCurrentMediaPlayer, id);
#else
	libvlc_video_set_spu(_vlcCurrentMediaPlayer, id ,_vlcException);
#endif

	VlcInstance::checkError();
}

void QVlc::VideoControl::mediaChange()
{
	_manualLanguage = false;
}
