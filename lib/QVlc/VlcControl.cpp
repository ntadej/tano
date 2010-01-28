#include <QDebug>

#include "VlcControl.h"
#include "VlcInstance.h"

VlcControl::VlcControl()
{
	isPlaying = false;

	audioGroup = 0;
	subGroup = 0;

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateActions()));

	check = new QTimer(this);
	connect(check, SIGNAL(timeout()), this, SLOT(checkPlayingState()));

	check->start(300);
}

VlcControl::~VlcControl()
{
	delete timer;
	delete check;
}

void VlcControl::updateActions() {
	for(int i=0; i<audioList.size(); i++)
		delete audioList.at(i);
	audioList.clear();
	for(int i=0; i<subList.size(); i++)
		delete subList.at(i);
	subList.clear();
	audioMap.clear();
	subMap.clear();

	if(audioGroup != 0)
		delete audioGroup;
	if(subGroup != 0)
		delete subGroup;
	audioGroup = new QActionGroup(this);
    subGroup = new QActionGroup(this);

#if VLC_TRUNK
	if(libvlc_media_player_is_playing(_vlcCurrentMediaPlayer) != 1) {
#else
	if(libvlc_media_player_is_playing(_vlcCurrentMediaPlayer, _vlcException) != 1) {
#endif
		audioList << new QAction(tr("Disabled"), this);
		audioList.at(0)->setCheckable(true);
		audioList.at(0)->setChecked(true);
		audioList.at(0)->setEnabled(false);
		emit vlcAction("audio", audioList);
		subList << new QAction(tr("Disabled"), this);
		subList.at(0)->setCheckable(true);
		subList.at(0)->setChecked(true);
		subList.at(0)->setEnabled(false);
		emit vlcAction("sub", subList);
		timer->stop();
		return;
	}

    if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
    	libvlc_track_description_t *desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer, _vlcException);
    	audioMap.insert(tr("Disabled"), 0);
    	audioList << new QAction(tr("Disabled"), this);
    	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
    		for(int i = 1; i < libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
    			desc = desc->p_next;
       			//qDebug() << "Audio Candidate:" << i << desc->i_id;
    	    	audioMap.insert(desc->psz_name, i);
    	    	audioList << new QAction(desc->psz_name, this);
    		}
    	}
    } else {
		audioList << new QAction(tr("Disabled"), this);
		audioList.at(0)->setCheckable(true);
		audioList.at(0)->setChecked(true);
		audioList.at(0)->setEnabled(false);
		emit vlcAction("audio", audioList);
    	timer->start(300);
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


    if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
      	libvlc_track_description_t *descs = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer, _vlcException);
    	subMap.insert(tr("Disabled"), 0);
    	subList << new QAction(tr("Disabled"), this);
       	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
       		for(int i = 1; i < libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException); i++) {
       			descs = descs->p_next;
       			//qDebug() << "Subtitle Candidate:" << i << descs->i_id;
    	    	subMap.insert(descs->psz_name, i);
    	    	subList << new QAction(descs->psz_name, this);
       		}
       	}
	} else {
		subList << new QAction(tr("Disabled"), this);
		subList.at(0)->setCheckable(true);
		subList.at(0)->setChecked(true);
		subList.at(0)->setEnabled(false);
		emit vlcAction("sub", subList);
	    timer->stop();
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

	timer->start(5000);
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

void VlcControl::update()
{
	timer->start(1000);
}

void VlcControl::checkPlayingState()
{
	int state;
#if VLC_TRUNK
	//state = libvlc_media_player_is_playing(_vlcCurrentMediaPlayer); -> LibVLC bug
#else
	state = libvlc_media_player_is_playing(_vlcCurrentMediaPlayer, _vlcException);
#endif

#if VLC_TRUNK

#else
	if(state == 1 && isPlaying == 0) {
		isPlaying = 1;
		emit stateChanged(1);
	} else if(state == 0 && isPlaying == 1) {
		isPlaying = 0;
		emit stateChanged(0);
	}
#endif
}
