#ifndef TANO_OSD_H
#define TANO_OSD_H

#include <QtGui/QWidget>
#include <QTime>

#include "ui_Osd.h"

class Osd : public QWidget
{
    Q_OBJECT

public:
    Osd(QWidget *parent = 0);
    ~Osd();

public slots:
	void showOsd();
	void hideOsd();

	void setNumber(int n);
	void setMuted(bool mute);
	void setStatus(bool status);

	void disableRecorder();

	void setInfo(QString channel, QString language = "");
	void setEpg(bool status, QString now = "", QString next = "");

signals:
	void linkActivated(QString);

	void play();
	void stop();
	void back();
	void next();
	void mute();

private:
    Ui::Osd ui;

    QTime timeNow;
};

#endif // TANO_OSD_H
