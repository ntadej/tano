#include <QDebug>

#include "InfoBar.h"

InfoBar::InfoBar(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(ui.labelNext, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(ui.buttonRefresh, SIGNAL(clicked()), this, SIGNAL(refresh()));
}

InfoBar::~InfoBar()
{

}

void InfoBar::clear()
{
	ui.buttonRefresh->setEnabled(false);
}

void InfoBar::setInfo(QString channel, QString language)
{
	ui.labelChannel->setText(tr("Playing:")+" "+channel);
	ui.labelLanguage->setText(tr("Language:")+" "+language);
}

void InfoBar::setEpg(QString now, QString next)
{
	ui.buttonRefresh->setEnabled(true);
	ui.labelNow->setText(tr("Now:")+" "+now);
	ui.labelNext->setText(tr("Next:")+" "+next);
}
