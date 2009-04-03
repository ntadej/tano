#ifndef TANOOSD_H
#define TANOOSD_H

#include <QtGui/QWidget>
#include <QTimer>
#include <QTime>
#include <QMouseEvent>
#include <QPoint>

#include "ui_TanoOsd.h"

class TanoOsd : public QWidget
{
    Q_OBJECT

public:
    TanoOsd(QWidget *parent = 0);
    ~TanoOsd();

public slots:
	void showOsd();
	void hideOsd();

	void setNumber(int n);
	void setVolume(int v);
	void setMuted(bool mute);

	void setStatus(bool status);
	void setLenght(qint64 l);
	void setDuration(qint64 d);

	void disableRecorder();

signals:
	void play();
	void stop();
	void back();
	void next();
	void mute();
	void volume(int);
	void seek(int);

private:
    Ui::TanoOsdClass ui;

    QTimer *timer;
    QTime timeNow;
    bool enabled;
};

#endif // TANOOSD_H
