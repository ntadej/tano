#ifndef TANO_OSD_H_
#define TANO_OSD_H_

#include <QtGui/QWidget>
#include <QTime>

#include <ui_Osd.h>

class Osd : public QWidget
{
    Q_OBJECT

public:
    Osd(QWidget *parent = 0);
    ~Osd();

public slots:
	void showOsd();
	void hideOsd();

	QLCDNumber *lcd();
	void setMuted(bool mute);
	void setStatus(bool status);

	void disableRecorder();

	void setInfo(QString channel = 0, QString language = "");
	void setEpg(bool status, QString now = "", QString next = "");

	void playingState(int status);

signals:
	void linkActivated(QString);

	void play();
	void stop();
	void back();
	void next();
	void mute();

private:
    Ui::Osd ui;

    int dWidth;
    int dHeight;

    QTime timeNow;
};

#endif // TANO_OSD_H_
