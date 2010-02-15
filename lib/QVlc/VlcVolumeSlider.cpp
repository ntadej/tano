#include <QtGui/QHBoxLayout>

#include "VlcInstance.h"
#include "VlcVolumeSlider.h"

VlcVolumeSlider::VlcVolumeSlider(QWidget *parent)
	: QWidget(parent)
{
	slider = new QSlider(this);
	slider->setOrientation(Qt::Horizontal);
	slider->setMaximum(200);

	label = new QLabel(this);
	label->setMinimumWidth(20);
	label->setText(QString().number(50));

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(slider);
	layout->addWidget(label);
	setLayout(layout);

	timer = new QTimer(this);

	connect(timer, SIGNAL(timeout()), this, SLOT(updateVolume()));
	connect(slider, SIGNAL(valueChanged(int)), this, SLOT(changeVolume(int)));

	timer->start(100);
}

VlcVolumeSlider::~VlcVolumeSlider() {
	delete slider;
	delete label;
	delete timer;
}

void VlcVolumeSlider::changeVolume(const int &newVolume)
{
	if(!_vlcCurrentMediaPlayer)
		return;

	label->setText(QString().number(newVolume));

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
	slider->setValue(volume);
	label->setText(QString().number(volume));

	VlcInstance::checkError();
}

void VlcVolumeSlider::setVolume(const int &volume)
{
	slider->setValue(volume);
	label->setText(QString().number(volume));
}

int VlcVolumeSlider::volume()
{
	return slider->value();
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
	if(slider->value() != 200)
		changeVolume(slider->value()+1);
}

void VlcVolumeSlider::vdown()
{
	if(slider->value() != 0)
		changeVolume(slider->value()-1);
}

void VlcVolumeSlider::mute() {
#if VLC_TRUNK
	libvlc_audio_toggle_mute(_vlcCurrentMediaPlayer);
#else
	libvlc_audio_toggle_mute(_vlcInstance, _vlcException);
#endif

	VlcInstance::checkError();
}
