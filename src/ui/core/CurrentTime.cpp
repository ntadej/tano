/****************************************************************************
* CurrentTime.cpp: Displays time
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

#include <QtCore/QTime>
#include <QtGui/QHBoxLayout>

#include "ui/core/CurrentTime.h"

CurrentTime::CurrentTime(QWidget *parent)
	: QWidget(parent)
{
	_time = new QLabel(this);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(_time);
	layout->setSpacing(0);
	layout->setMargin(0);
	setLayout(layout);

	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(updateTime()));

	_timer->start(1000);
}

CurrentTime::~CurrentTime()
{
	delete _time;
	delete _timer;
}

void CurrentTime::updateTime()
{
	_time->setText("<b>"+QTime::currentTime().toString("hh:mm:ss")+"</b>");
}
