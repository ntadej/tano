#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>

#include "Osd.h"

Osd::Osd(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	this->setWindowFlags(Qt::ToolTip);

	dWidth = QApplication::desktop()->width();
	dHeight = QApplication::desktop()->height();
	this->resize(dWidth*0.75,this->height());

	connect(ui.buttonPlay, SIGNAL(clicked()), this, SIGNAL(play()));
	connect(ui.buttonStop, SIGNAL(clicked()), this, SIGNAL(stop()));
	connect(ui.buttonBack, SIGNAL(clicked()), this, SIGNAL(back()));
	connect(ui.buttonNext, SIGNAL(clicked()), this, SIGNAL(next()));
	connect(ui.buttonMute, SIGNAL(clicked()), this, SIGNAL(mute()));

	connect(ui.infoBarWidget, SIGNAL(open(QString)), this, SIGNAL(linkActivated(QString)));
}

Osd::~Osd()
{

}

void Osd::showOsd()
{
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
	if(!channel.isNull())
		ui.infoBarWidget->setInfo(channel,language);
	else
		ui.infoBarWidget->clear();
}

void Osd::setEpg(bool status, QString now, QString next)
{
	if(status)
		ui.infoBarWidget->setEpg(now, next);
	else
		ui.infoBarWidget->clear();
}

void Osd::disableRecorder()
{
	ui.buttonRecord->hide();
}

void Osd::playingState(int status)
{
	if(status == 1) {
		ui.buttonPlay->setEnabled(true);
		ui.buttonPlay->setIcon(QIcon(":/icons/images/player_pause.png"));
		ui.buttonPlay->setToolTip(tr("Pause"));
		ui.buttonPlay->setStatusTip(tr("Pause"));
	} else if(status == 0) {
		ui.buttonPlay->setEnabled(true);
		ui.buttonPlay->setIcon(QIcon(":/icons/images/player_play.png"));
		ui.buttonPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setStatusTip(tr("Play"));
	} else if(status == -1) {
		ui.buttonPlay->setEnabled(false);
		ui.buttonPlay->setToolTip(tr("Play"));
		ui.buttonPlay->setStatusTip(tr("Play"));
	}
}
