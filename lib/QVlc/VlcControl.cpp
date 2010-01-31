#include "VlcControl.h"
#include "VlcInstance.h"

VlcControl::VlcControl()
{
	audioGroup = 0;
	subGroup = 0;

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateActionsAudio()));
	connect(timer, SIGNAL(timeout()), this, SLOT(updateActionsVideo()));

	check = new QTimer(this);
	connect(check, SIGNAL(timeout()), this, SLOT(checkPlayingState()));

	check->start(300);
	timer->start(2000);
}

VlcControl::~VlcControl()
{
	delete timer;
	delete check;
}

void VlcControl::updateActionsAudio() {
	for(int i=0; i<audioList.size(); i++)
		delete audioList.at(i);
	audioList.clear();
	audioMap.clear();

	if(audioGroup != 0)
		delete audioGroup;
	audioGroup = new QActionGroup(this);

    if(!_vlcCurrentMediaPlayer)
    	return;

    libvlc_state_t state;
    #if VLC_TRUNK
    	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
    #else
    	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
    #endif

    if(state == 0 || state == 6 || state == 7) {
		emit vlcAction("audio", audioList);
		return;
	}

    if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
    	libvlc_track_description_t *desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer, _vlcException);
    	audioMap.insert(QString().fromUtf8(desc->psz_name), 0);
    	audioList << new QAction(QString().fromUtf8(desc->psz_name), this);
    	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
    		for(int i = 1; i < libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
    			desc = desc->p_next;
    	    	audioMap.insert(QString().fromUtf8(desc->psz_name), i);
    	    	audioList << new QAction(QString().fromUtf8(desc->psz_name), this);
    		}
    	}
    } else {
		emit vlcAction("audio", audioList);
    	return;
    }
	VlcInstance::checkException();

    for (int i = 0; i < audioList.size(); ++i) {
    	audioList.at(i)->setCheckable(true);
    	audioGroup->addAction(audioList.at(i));
        connect(audioList.at(i), SIGNAL(triggered()), this, SLOT(updateAudio()));
    }
    audioList.at(libvlc_audio_get_track(_vlcCurrentMediaPlayer, _vlcException))->setChecked(true);
	VlcInstance::checkException();

    emit vlcAction("audio", audioList);
}

void VlcControl::updateActionsVideo() {
	for(int i=0; i<subList.size(); i++)
		delete subList.at(i);
	subList.clear();
	subMap.clear();

	if(subGroup != 0)
		delete subGroup;
    subGroup = new QActionGroup(this);

    if(!_vlcCurrentMediaPlayer)
    	return;

    libvlc_state_t state;
    #if VLC_TRUNK
    	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
    #else
    	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
    #endif

    if(state == 0 || state == 6 || state == 7) {
		emit vlcAction("sub", subList);
		return;
    }

    if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
      	libvlc_track_description_t *descs = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer, _vlcException);
    	subMap.insert(QString().fromUtf8(descs->psz_name), 0);
    	subList << new QAction(QString().fromUtf8(descs->psz_name), this);
       	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
       		for(int i = 1; i < libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
       			descs = descs->p_next;
    	    	subMap.insert(QString().fromUtf8(descs->psz_name), i);
    	    	subList << new QAction(QString().fromUtf8(descs->psz_name), this);
       		}
       	}
	} else {
		emit vlcAction("sub", subList);
	    return;
	}
	VlcInstance::checkException();

    for (int i = 0; i < subList.size(); ++i) {
    	subList.at(i)->setCheckable(true);
    	subGroup->addAction(subList.at(i));
        connect(subList.at(i), SIGNAL(triggered()), this, SLOT(updateSub()));
	}
    subList.at(libvlc_video_get_spu(_vlcCurrentMediaPlayer, _vlcException))->setChecked(true);
	VlcInstance::checkException();

    emit vlcAction("sub", subList);
}

void VlcControl::updateAudio()
{
	int id = audioMap.value(audioGroup->checkedAction()->text());
	libvlc_audio_set_track(_vlcCurrentMediaPlayer, id ,_vlcException);
	VlcInstance::checkException();
}

void VlcControl::updateSub()
{
	int id = subMap.value(subGroup->checkedAction()->text());
	libvlc_video_set_spu(_vlcCurrentMediaPlayer, id ,_vlcException);
	VlcInstance::checkException();
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
