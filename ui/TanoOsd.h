#ifndef TANOOSD_H
#define TANOOSD_H

#include <QtGui/QWidget>
#include <QTimer>
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

signals:
	void play();
	void stop();
	void back();
	void next();
	void mute();
	void volume(int);

private:
    Ui::TanoOsdClass ui;

    QTimer *timer;
    bool moveEnabled;
};

#endif // TANOOSD_H
