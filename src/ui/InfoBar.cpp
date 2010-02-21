/****************************************************************************
* InfoBar.cpp: InfoBar for Channel information display
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtGui/QScrollBar>

#include "InfoBar.h"

InfoBar::InfoBar(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.buttonRefresh->setEnabled(false);

	_timer = new QTimer(this);
	_direction = true;

	connect(ui.labelNow, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(ui.labelNext, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(ui.buttonRefresh, SIGNAL(clicked()), this, SIGNAL(refresh()));
	connect(_timer, SIGNAL(timeout()), this, SLOT(scroll()));

	_timer->start(50);
}

InfoBar::~InfoBar()
{
	delete _timer;
}

void InfoBar::scroll()
{
	if(ui.scrollArea->horizontalScrollBar()->maximum() != 0) {
		if(_direction && ui.scrollArea->horizontalScrollBar()->value()<ui.scrollArea->horizontalScrollBar()->maximum()) {
			ui.scrollArea->horizontalScrollBar()->setValue(ui.scrollArea->horizontalScrollBar()->value()+1);
			if(ui.scrollArea->horizontalScrollBar()->maximum() == ui.scrollArea->horizontalScrollBar()->value())
				_direction = false;
		} else if(!_direction && ui.scrollArea->horizontalScrollBar()->value()>ui.scrollArea->horizontalScrollBar()->minimum()) {
			ui.scrollArea->horizontalScrollBar()->setValue(ui.scrollArea->horizontalScrollBar()->value()-1);
			if(ui.scrollArea->horizontalScrollBar()->value() == 0)
				_direction = true;
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

void InfoBar::setInfo(const QString &channel, const QString &language)
{
	ui.labelChannel->setText("<b>"+channel+"</b>");
	ui.labelLanguage->setText(tr("Language:")+" "+language);
}

void InfoBar::setEpg(const QString &now, const QString &next)
{
	ui.buttonRefresh->setEnabled(true);
	ui.labelNow->setText(tr("Now:")+" "+now);
	ui.labelNext->setText(tr("Next:")+" "+next);
}
