#ifndef QVLC_VLCVOLUMESLIDER_H_
#define QVLC_VLCVOLUMESLIDER_H_

#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

class VlcVolumeSlider : public QWidget {
Q_OBJECT
public:
	VlcVolumeSlider(QWidget *parent = 0);
	virtual ~VlcVolumeSlider();

	void setVolume(int volume);
	int volume();

public slots:
	void mute();
	void vup();
	void vdown();

private slots:
	void changeVolume(int newVolume);
	void updateVolume();

private:
	QSlider *slider;
	QLabel *label;
	QTimer *timer;
};

#endif // QVLC_VLCVOLUMESLIDER_H_
