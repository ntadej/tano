#ifndef VIDEO_H
#define VIDEO_H

#include <phonon/videowidget.h>
#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/volumeslider.h>

class Video : public Phonon::VideoWidget
{
    Q_OBJECT

public:
	Video(QWidget *parent = 0);
	~Video();

	void playTv(QString fileName, QString channelName);
	void setVolume(qreal volume);

	Phonon::VolumeSlider *slider();
	QString currentMedia();
	qreal volume();

protected:
	void mouseDoubleClickEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

public slots:
	void controlPlay();
	void controlStop();
	void controlFull();
	void controlMute();
	void controlVUp();
	void controlVDown();

	void ratioOriginal();
	void ratio43();
	void ratio169();
	void ratioDinamic();
	void cropOriginal();
	void cropFit();

signals:
	void playing(QString);
	void stopped();
	void full();

	void rightClick(QPoint);
	void wheel(bool);

private:
	Phonon::AudioOutput *audio;
	Phonon::MediaObject *channel;
	Phonon::VolumeSlider *aslider;

	QString currentChannel;
};

#endif // VIDEO_H
