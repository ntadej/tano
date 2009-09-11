#include <QHBoxLayout>
#include <QTime>
#include <QDebug>

#include "VlcSeekWidget.h"
#include "VlcInstance.h"

VlcSeekWidget::VlcSeekWidget(QWidget *parent)
	:QWidget(parent)
{
	seek = new QSlider(this);
	seek->setOrientation(Qt::Horizontal);
	//seek->setMaximum(200);

	labelElapsed = new QLabel(this);
	labelElapsed->setText("00:00:00");

	labelFull = new QLabel(this);
	labelFull->setText("00:00:00");

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(labelElapsed);
	layout->addWidget(seek);
	layout->addWidget(labelFull);
	setLayout(layout);

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	connect(seek, SIGNAL(valueChanged(int)), this, SLOT(changeTime(int)));

	timer->start(100);
}

VlcSeekWidget::~VlcSeekWidget() {

}

void VlcSeekWidget::updateTime()
{
	if(!_vlcCurrentMediaPlayer)
	        return;

	// It's possible that the vlc doesn't play anything
	// so check before
	libvlc_media_t *curMedia = libvlc_media_player_get_media(_vlcCurrentMediaPlayer, _vlcException);
	libvlc_exception_clear(_vlcException);
	if (curMedia == NULL)
		return;

	if(libvlc_media_player_is_playing(_vlcCurrentMediaPlayer, _vlcException) == 1) {
		qint64 fullTime = libvlc_media_player_get_length(_vlcCurrentMediaPlayer, _vlcException);
		qint64 currentTime = libvlc_media_player_get_time(_vlcCurrentMediaPlayer, _vlcException);

		labelFull->setText(QTime(0,0,0,0).addMSecs(fullTime).toString("hh:mm:ss"));
		seek->setMaximum(fullTime);

		labelElapsed->setText(QTime(0,0,0,0).addMSecs(currentTime).toString("hh:mm:ss"));
		seek->setValue(currentTime);
	} else {
		labelFull->setText("00:00:00");
		seek->setMaximum(0);

		labelElapsed->setText("00:00:00");
		seek->setValue(0);
	}

	VlcInstance::checkException();
}

void VlcSeekWidget::changeTime(int newTime)
{
	labelElapsed->setText(QTime(0,0,0,0).addMSecs(newTime).toString("hh:mm:ss"));

    libvlc_exception_clear(_vlcException);
    libvlc_media_player_set_time(_vlcCurrentMediaPlayer, newTime, _vlcException);
    VlcInstance::checkException();
}
