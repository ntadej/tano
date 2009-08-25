#include <QDebug>

#include "VlcControl.h"
#include "VlcInstance.h"

VlcControl::VlcControl()
{
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateActions()));
	timer->start(5000);
}

VlcControl::~VlcControl()
{

}

void VlcControl::updateActions() {
	QList<QAction*> audioListT;
	QMap<int,QString> audioMapT;
	audioGroup = new QActionGroup(this);
	audioMenu = new QMenu(tr("Audio channel"));

    if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
    	libvlc_track_description_t *desc = libvlc_audio_get_track_description(_vlcCurrentMediaPlayer, _vlcException);
    	audioMapT.insert(desc->i_id, tr("Disable"));
    	audioListT << new QAction(tr("Disable"), this);
    	if(libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
    		for(int i = libvlc_audio_get_track_count(_vlcCurrentMediaPlayer, _vlcException); i > 1; i--) {
    			desc = desc->p_next;
    	    	audioMapT.insert(desc->i_id, desc->psz_name);
    	    	audioListT << new QAction(desc->psz_name, this);
    		}
    	}
    }

    for (int i = 0; i < audioListT.size(); ++i) {
    	audioListT.at(i)->setCheckable(true);
    	audioGroup->addAction(audioListT.at(i));
        audioMenu->addAction(audioListT.at(i));
        connect(audioListT.at(i), SIGNAL(triggered()), this, SLOT(updateAudio()));
    }
    audioListT.at(1)->setChecked(true);

    audioList = audioListT;
    audioMap = audioMapT;
    emit vlcAction("audio", audioMenu);


    QList<QAction*> subListT;
	QMap<int,QString> subMapT;
	subGroup = new QActionGroup(this);
	subMenu = new QMenu(tr("Subtitle"));

    if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) != 0) {
      	libvlc_track_description_t *descs = libvlc_video_get_spu_description(_vlcCurrentMediaPlayer, _vlcException);
    	subMapT.insert(descs->i_id, tr("Disable"));
    	subListT << new QAction(tr("Disable"), this);
       	if(libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException) > 1) {
       		for(int i = libvlc_video_get_spu_count(_vlcCurrentMediaPlayer, _vlcException); i > 1; i--) {
       			descs = descs->p_next;
    	    	subMapT.insert(descs->i_id, descs->psz_name);
    	    	subListT << new QAction(descs->psz_name, this);
       		}
       	}
    }

    for (int i = 0; i < subListT.size(); ++i) {
    	subListT.at(i)->setCheckable(true);
    	subGroup->addAction(subListT.at(i));
        subMenu->addAction(subListT.at(i));
        connect(subListT.at(i), SIGNAL(triggered()), this, SLOT(updateSub()));
    }
    subListT.at(0)->setChecked(true);

    subList = subListT;
    subMap = subMapT;
    emit vlcAction("sub", subMenu);

    timer->stop();
}

void VlcControl::updateAudio()
{
	qDebug() << "click" << "audio";
}

void VlcControl::updateSub()
{
	qDebug() << "click" << "sub";
}
