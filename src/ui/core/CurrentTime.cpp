/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QDateTime>
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

	_timer->start(500);
}

CurrentTime::~CurrentTime()
{
	delete _time;
	delete _timer;
}

void CurrentTime::updateTime()
{
	_time->setText("<b>"+QDateTime::currentDateTime().toString("dddd, d.M.yyyy, hh:mm:ss")+"</b>");
}
