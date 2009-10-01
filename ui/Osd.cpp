#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "Osd.h"

Osd::Osd(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	this->setWindowFlags(Qt::ToolTip);

	connect(ui.buttonPlay, SIGNAL(clicked()), this, SIGNAL(play()));
	connect(ui.buttonStop, SIGNAL(clicked()), this, SIGNAL(stop()));
	connect(ui.buttonBack, SIGNAL(clicked()), this, SIGNAL(back()));
	connect(ui.buttonNext, SIGNAL(clicked()), this, SIGNAL(next()));
	connect(ui.buttonMute, SIGNAL(clicked()), this, SIGNAL(mute()));

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), this, SIGNAL(linkActivated(QString)));
	connect(ui.labelNext, SIGNAL(linkActivated(QString)), this, SIGNAL(linkActivated(QString)));
}

Osd::~Osd()
{

}

void Osd::showOsd()
{
	dWidth = QApplication::desktop()->width();
	dHeight = QApplication::desktop()->height();
	this->resize(dWidth*0.75,75);
	this->move(dWidth/2-this->width()/2,dHeight-this->height());
	this->show();
}

void Osd::hideOsd()
{
	this->hide();
}

void Osd::setNumber(int n)
{
	ui.channelNumber->display(n);
}


void Osd::setMuted(bool mute)
{
	ui.buttonMute->setChecked(mute);
	ui.volumeSlider->setDisabled(mute);
}

void Osd::setStatus(bool status)
{
	if(status)
		hideOsd();
	else
		showOsd();
}

void Osd::setInfo(QString channel, QString language)
{
	if(!channel.isNull()) {
		ui.widgetInfo->show();
		ui.labelChannel->setText("<h3>"+channel+"</h3>");
		ui.labelLanguage->setText("<h3>"+language+"</h3>");
	} else {
		ui.widgetInfo->hide();
	}
}

void Osd::setEpg(bool status, QString now, QString next)
{
	if(status) {
		ui.labelNow->show();
		ui.labelNext->show();
		ui.labelS2->show();
		ui.labelS3->show();
		ui.labelNow->setText("<h3>"+now+"</h3>");
		ui.labelNext->setText("<h3>"+next+"</h3>");
	} else {
		ui.labelNow->hide();
		ui.labelNext->hide();
		ui.labelS2->hide();
		ui.labelS3->hide();
	}
}

void Osd::disableRecorder()
{
	ui.buttonRecord->hide();
}
