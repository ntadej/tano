#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "TanoOsd.h"

TanoOsd::TanoOsd(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	enabled = true;

	this->setWindowFlags(Qt::ToolTip);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(hideOsd()));

	connect(ui.buttonPlay, SIGNAL(clicked()), this, SIGNAL(play()));
	connect(ui.buttonStop, SIGNAL(clicked()), this, SIGNAL(stop()));
	connect(ui.buttonBack, SIGNAL(clicked()), this, SIGNAL(back()));
	connect(ui.buttonNext, SIGNAL(clicked()), this, SIGNAL(next()));
	connect(ui.buttonMute, SIGNAL(clicked()), this, SIGNAL(mute()));
	connect(ui.volumeSlider, SIGNAL(valueChanged(int)), this, SIGNAL(volume(int)));
	connect(ui.durationSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(seek(int)));
}

TanoOsd::~TanoOsd()
{

}

void TanoOsd::showOsd()
{
	int w = QApplication::desktop()->width();
	int h = QApplication::desktop()->height();
	timer->start(1000);
	this->resize(w*0.75,this->height());
	this->move(w/2-this->width()/2,h-this->height());
	this->show();
}

void TanoOsd::hideOsd()
{
	if(enabled) {
		timer->stop();
		this->hide();
	}
}

void TanoOsd::setNumber(int n)
{
	ui.channelNumber->display(n);
}

void TanoOsd::setVolume(int v)
{
	ui.volumeSlider->setValue(v);
}

void TanoOsd::setMuted(bool mute)
{
	ui.buttonMute->setChecked(mute);
	ui.volumeSlider->setDisabled(mute);
}

void TanoOsd::setStatus(bool status)
{
	enabled = status;
}

void TanoOsd::setDuration(qint64 d)
{
	int tm = d*1;
	timeNow = QTime();
	timeNow = timeNow.addMSecs(tm);
	ui.labelDuration->setText(timeNow.toString("hh:mm:ss"));
	ui.durationSlider->setValue(tm);
}

void TanoOsd::setLenght(qint64 l)
{
	int tm = l*1;
	ui.durationSlider->setMaximum(tm);
	timeNow = QTime();
	timeNow = timeNow.addMSecs(tm);
	ui.labelLenght->setText(timeNow.toString("hh:mm:ss"));
}
