#include <QScrollBar>

#include "InfoBar.h"

InfoBar::InfoBar(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.buttonRefresh->setEnabled(false);

	timer = new QTimer(this);
	direction = true;

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(ui.labelNext, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(ui.buttonRefresh, SIGNAL(clicked()), this, SIGNAL(refresh()));
	connect(timer, SIGNAL(timeout()), this, SLOT(scroll()));

	timer->start(50);
}

InfoBar::~InfoBar()
{

}

void InfoBar::scroll()
{
	if(ui.scrollArea->horizontalScrollBar()->maximum() != 0) {
		if(direction && ui.scrollArea->horizontalScrollBar()->value()<ui.scrollArea->horizontalScrollBar()->maximum()) {
			ui.scrollArea->horizontalScrollBar()->setValue(ui.scrollArea->horizontalScrollBar()->value()+1);
			if(ui.scrollArea->horizontalScrollBar()->maximum() == ui.scrollArea->horizontalScrollBar()->value())
				direction = false;
		} else if(!direction && ui.scrollArea->horizontalScrollBar()->value()>ui.scrollArea->horizontalScrollBar()->minimum()) {
			ui.scrollArea->horizontalScrollBar()->setValue(ui.scrollArea->horizontalScrollBar()->value()-1);
			if(ui.scrollArea->horizontalScrollBar()->value() == 0)
				direction = true;
		}
	}
}

void InfoBar::clear()
{
	ui.buttonRefresh->setEnabled(false);
	ui.labelNow->setText("");
	ui.labelNext->setText("");
	ui.labelChannel->setText("");
	ui.labelLanguage->setText("");
}

void InfoBar::setInfo(QString channel, QString language)
{
	ui.labelChannel->setText("<b>"+channel+"</b>");
	ui.labelLanguage->setText(tr("Language:")+" "+language);
}

void InfoBar::setEpg(QString now, QString next)
{
	ui.buttonRefresh->setEnabled(true);
	ui.labelNow->setText(tr("Now:")+" "+now);
	ui.labelNext->setText(tr("Next:")+" "+next);
}
