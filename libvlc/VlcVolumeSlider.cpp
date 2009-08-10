#include <QHBoxLayout>
#include <QDebug>

#include "VlcVolumeSlider.h"
#include "VlcInstance.h"

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

}

void VlcVolumeSlider::changeVolume(int newVolume)
{
	label->setText(QString().number(newVolume));

    libvlc_exception_clear(_vlcException);
    libvlc_audio_set_volume (_vlcInstance, newVolume, _vlcException);
    VlcInstance::checkException();
}

void VlcVolumeSlider::updateVolume()
{
    if(!_vlcCurrentMediaPlayer)
        return;

    // It's possible that the vlc doesn't play anything
    // so check before
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_vlcCurrentMediaPlayer, _vlcException);
    libvlc_exception_clear(_vlcException);
    if (curMedia == NULL)
        return;

    int volume = libvlc_audio_get_volume (_vlcInstance, _vlcException);
    slider->setValue(volume);
    label->setText(QString().number(volume));
}

void VlcVolumeSlider::setValue(int volume)
{
    slider->setValue(volume);
    label->setText(QString().number(volume));
}

int VlcVolumeSlider::value()
{
	return slider->value();
}
