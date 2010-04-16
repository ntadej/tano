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

#include <QtGui/QHBoxLayout>
#include <QtGui/QScrollBar>

#include "InfoBar.h"

InfoBar::InfoBar(QWidget *parent) :
	QScrollArea(parent),
	_direction(true),
	_image(new GetImage())
{
	_timer = new QTimer(this);

	QWidget *widget = new QWidget(this);
	_labelChannel = new QLabel(widget);
	_labelLanguage = new QLabel(widget);
	_labelLogo = new QLabel(widget);
	_labelNext = new QLabel(widget);
	_labelNow = new QLabel(widget);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setContentsMargins(4,0,4,0);
	layout->setSpacing(10);
	layout->addWidget(_labelLogo);
	layout->addWidget(_labelChannel);
	layout->addWidget(_labelLanguage);
	layout->addWidget(_labelNow);
	layout->addWidget(_labelNext);
	layout->addSpacerItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Fixed));
	widget->setLayout(layout);

	setWidget(widget);
	setWidgetResizable(true);
	setMaximumHeight(35);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	connect(_labelNow, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(_labelNext, SIGNAL(linkActivated(QString)), this, SIGNAL(open(QString)));
	connect(_image, SIGNAL(image(QString)), this, SLOT(image(QString)));
	connect(_timer, SIGNAL(timeout()), this, SLOT(scroll()));

	_timer->start(50);
}

InfoBar::~InfoBar()
{
	delete _image;
	delete _timer;
}

void InfoBar::scroll()
{
	if(horizontalScrollBar()->maximum() != 0) {
		if(_direction && horizontalScrollBar()->value() < horizontalScrollBar()->maximum()) {
			horizontalScrollBar()->setValue(horizontalScrollBar()->value()+1);
			if(horizontalScrollBar()->maximum() == horizontalScrollBar()->value())
				_direction = false;
		} else if(!_direction && horizontalScrollBar()->value() > horizontalScrollBar()->minimum()) {
			horizontalScrollBar()->setValue(horizontalScrollBar()->value()-1);
			if(horizontalScrollBar()->value() == 0)
				_direction = true;
		}
	}
}

void InfoBar::clear()
{
	_labelNow->setText("");
	_labelNext->setText("");
	_labelChannel->setText("");
	_labelLanguage->setText("");
	_labelLogo->clear();
	_labelLogo->hide();
}

void InfoBar::setInfo(const QString &channel, const QString &language)
{
	_labelChannel->setText("<b>"+channel+"</b>");
	_labelLanguage->setText(tr("Language:")+" "+language);
}

void InfoBar::setEpg(const QString &now, const QString &next)
{
	_labelNow->setText(tr("Now:")+" "+now);
	_labelNext->setText(tr("Next:")+" "+next);
}

void InfoBar::setLogo(const QString &logo)
{
	if(logo.isEmpty())
		return;

	if(logo.contains("http"))
		_image->getImage(logo);
	else
		image(logo);
}

void InfoBar::image(const QString &image)
{
	_labelLogo->setPixmap(QPixmap(image));
	_labelLogo->show();
}
