#include <QtCore/QTime>
#include <QtGui/QHBoxLayout>

#include "VlcInstance.h"
#include "VlcSeekWidget.h"

VlcSeekWidget::VlcSeekWidget(QWidget *parent)
	:QWidget(parent)
{
	seek = new QSlider(this);
	seek->setOrientation(Qt::Horizontal);
	seek->setMaximum(0);
	seek->setPageStep(1000);

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
	connect(seek, SIGNAL(sliderReleased()), this, SLOT(changeTime()));

	timer->start(400);
}

VlcSeekWidget::~VlcSeekWidget() {
	delete seek;
	delete labelElapsed;
	delete labelFull;
	delete timer;
}

void VlcSeekWidget::updateTime()
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
#endif
	libvlc_exception_clear(_vlcException);
	if (curMedia == 0)
		return;

	libvlc_state_t state;
#if VLC_TRUNK
	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer);
#else
	state = libvlc_media_player_get_state(_vlcCurrentMediaPlayer, _vlcException);
#endif

	if(state != 0 && state != 6 && state != 7) {
		libvlc_time_t fullTime = libvlc_media_player_get_length(_vlcCurrentMediaPlayer, _vlcException);
		libvlc_time_t currentTime = libvlc_media_player_get_time(_vlcCurrentMediaPlayer, _vlcException);

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

void VlcSeekWidget::changeTime()
{
    if(!_vlcCurrentMediaPlayer)
    	return;

	labelElapsed->setText(QTime(0,0,0,0).addMSecs(seek->value()).toString("hh:mm:ss"));

    libvlc_exception_clear(_vlcException);
    libvlc_media_player_set_time(_vlcCurrentMediaPlayer, seek->value(), _vlcException);
    VlcInstance::checkException();
}
