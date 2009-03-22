#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "TanoOsd.h"

TanoOsd::TanoOsd(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	moveEnabled = true;

	this->setWindowFlags(Qt::ToolTip);

	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(hideOsd()));

	connect(ui.buttonPlay, SIGNAL(clicked()), this, SIGNAL(play()));
	connect(ui.buttonStop, SIGNAL(clicked()), this, SIGNAL(stop()));
	connect(ui.buttonBack, SIGNAL(clicked()), this, SIGNAL(back()));
	connect(ui.buttonNext, SIGNAL(clicked()), this, SIGNAL(next()));
	connect(ui.buttonMute, SIGNAL(clicked()), this, SIGNAL(mute()));
	connect(ui.volumeSlider, SIGNAL(valueChanged(int)), this, SIGNAL(volume(int)));
}

TanoOsd::~TanoOsd()
{

}

void TanoOsd::showOsd()
{
	int w = QApplication::desktop()->width();
	int h = QApplication::desktop()->height();
	timer->start(1000);
	this->show();
	this->move(w/2-this->width()/2,h-this->height());
}

void TanoOsd::hideOsd()
{
	timer->stop();
	this->hide();
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
