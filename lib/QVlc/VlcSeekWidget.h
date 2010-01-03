#ifndef VLCSEEKWIDGET_H_
#define VLCSEEKWIDGET_H_

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QTimer>

class VlcSeekWidget : public QWidget {
Q_OBJECT
public:
	VlcSeekWidget(QWidget *parent = 0);
	virtual ~VlcSeekWidget();

private slots:
	void changeTime(int newTime);
	void updateTime();

private:
	QSlider *seek;
	QLabel *labelElapsed;
	QLabel *labelFull;
	QTimer *timer;
};

#endif /* VLCSEEKWIDGET_H_ */
