#ifndef VLCVOLUMESLIDER_H_
#define VLCVOLUMESLIDER_H_

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QTimer>

class VlcVolumeSlider : public QWidget {
Q_OBJECT
public:
	VlcVolumeSlider(QWidget *parent = 0);
	virtual ~VlcVolumeSlider();

	void setValue(int volume);
	int value();

private slots:
	void changeVolume(int newVolume);
	void updateVolume();

private:
	QSlider *slider;
	QLabel *label;
	QTimer *timer;
};

#endif /* VLCVOLUMESLIDER_H_ */
