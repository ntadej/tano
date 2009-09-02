#include <QDebug>

#include "VlcControl.h"
#include "VlcInstance.h"

VlcControl::VlcControl()
{
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateActions()));
}

VlcControl::~VlcControl()
{

}

void VlcControl::updateActions() {
	audioList.clear();
	subList.clear();

	audioGroup = new QActionGroup(this);

    if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
    	libvlc_track_description_t *desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer, _vlcException);
    	audioMap.insert(tr("Disabled"), 0);
    	audioList << new QAction(tr("Disabled"), this);
    	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
    		for(int i = libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException); i > 1; i--) {
    			desc = desc->p_next;
    	    	audioMap.insert(desc->psz_name, i-1);
    	    	audioList << new QAction(desc->psz_name, this);
    		}
    	}
    }

    for (int i = 0; i < audioList.size(); ++i) {
    	audioList.at(i)->setCheckable(true);
    	audioGroup->addAction(audioList.at(i));
        connect(audioList.at(i), SIGNAL(triggered()), this, SLOT(updateAudio()));
    }
    audioList.at(libvlc_audio_get_track(_vlcCurrentMediaPlayer, _vlcException))->setChecked(true);

    emit vlcAction("audio", audioList);


    subGroup = new QActionGroup(this);

    if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
      	libvlc_track_description_t *descs = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer, _vlcException);
    	subMap.insert(tr("Disabled"), 0);
    	subList << new QAction(tr("Disabled"), this);
       	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
       		for(int i = libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException); i > 1; i--) {
       			descs = descs->p_next;
    	    	subMap.insert(descs->psz_name, i-1);
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

    for (int i = 0; i < subList.size(); ++i) {
    	subList.at(i)->setCheckable(true);
    	subGroup->addAction(subList.at(i));
        connect(subList.at(i), SIGNAL(triggered()), this, SLOT(updateSub()));
	}
    subList.at(libvlc_video_get_spu(_vlcCurrentMediaPlayer, _vlcException))->setChecked(true);

    emit vlcAction("sub", subList);

    timer->stop();
}

void VlcControl::updateAudio()
{
	int id = audioMap.value(audioGroup->checkedAction()->text());
	libvlc_audio_set_track(_vlcCurrentMediaPlayer, id ,_vlcException);
}

void VlcControl::updateSub()
{
	int id = subMap.value(subGroup->checkedAction()->text());
	libvlc_video_set_spu(_vlcCurrentMediaPlayer, id ,_vlcException);
}

void VlcControl::update()
{
	timer->start(500);
}
