#ifndef QVLC_VLCSEEKWIDGET_H_
#define QVLC_VLCSEEKWIDGET_H_

#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QSlider>
#include <QtGui/QWidget>

class VlcSeekWidget : public QWidget
{
Q_OBJECT
public:
	VlcSeekWidget(QWidget *parent = 0);
	virtual ~VlcSeekWidget();

private slots:
	void changeTime();
	void updateTime();

private:
	QSlider *seek;
	QLabel *labelElapsed;
	QLabel *labelFull;
	QTimer *timer;
};

#endif // QVLC_VLCSEEKWIDGET_H_
