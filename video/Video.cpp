#include "Video.h"

Video::Video(QWidget *parent)
    : VideoWidget(parent)
{
	channel = new Phonon::MediaObject(parent);
	audio = new Phonon::AudioOutput(Phonon::VideoCategory, parent);
	aslider = new Phonon::VolumeSlider;
	aslider->setIconSize(QSize(32,32));
	aslider->setAudioOutput(audio);
	Phonon::createPath(channel, this);
	Phonon::createPath(channel, audio);

	shortcutFull = new QShortcut(QKeySequence(tr("Esc")),
	                          this);
	connect(shortcutFull, SIGNAL(activated()), this, SLOT(controlFull()));
}


Video::~Video()
{

}

void Video::playTv(QString fileName, QString channelName)
{
	channel->setCurrentSource(fileName);
	channel->play();

	currentChannel = channelName;
	emit playing(currentChannel);
}

void Video::controlPlay()
{
	channel->play();
	emit playing(currentChannel);
}

void Video::controlStop()
{
	channel->stop();
	emit stopped();
}

void Video::controlFull()
{
	if (this->isFullScreen() == false) {
		this->enterFullScreen();
	}
	else {
		this->exitFullScreen();
	}
}

Phonon::VolumeSlider *Video::slider()
{
	return aslider;
}

QString Video::currentMedia()
{
	return channel->currentSource().fileName();
}
