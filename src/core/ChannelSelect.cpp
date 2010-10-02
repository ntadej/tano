/****************************************************************************
* ChannelSelect.cpp: Channel selector
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

#include "core/ChannelSelect.h"

ChannelSelect::ChannelSelect(QWidget *parent,
							 QLCDNumber *number,
							 const QList<int> &list)
	: _lcd(number),
	_channels(list),
	_num(1)
{
	_timer = new QTimer(parent);

	_number[0] = 0;
	_number[1] = 0;
	_number[2] = 0;

	for(int i=0; i<10; i++) {
		_key.append(new QShortcut(QKeySequence(QString().number(i)), parent, 0, 0, Qt::ApplicationShortcut));
		connect(_key[i], SIGNAL(activated()), this, SLOT(keyPressed()));
	}

	connect(_timer, SIGNAL(timeout()), this, SLOT(display()));
}

ChannelSelect::~ChannelSelect() {
	delete _timer;
	for(int i=0; i<10; i++)
		delete _key[i];
}

void ChannelSelect::process(const int &key)
{
	if(_num == 1) {
		_old = _lcd->intValue();
		_number[0] = key;
	} else if(_num == 2) {
		_number[1] = _number[0];
		_number[0] = key;
	} else if(_num == 3) {
		_number[2] = _number[1];
		_number[1] = _number[0];
		_number[0] = key;
	}

	_full = _number[2]*100 + _number[1]*10 + _number[0];
	_lcd->display(_full);

	if(_num < 3) {
		_timer->start(1000);
		_num++;
	} else {
		display();
	}
}

void ChannelSelect::display()
{
	if(_channels.contains(_full))
		emit channelSelect(_full);
	else
		_lcd->display(_old);

	_number[0] = 0;
	_number[1] = 0;
	_number[2] = 0;
	_num = 1;
	_timer->stop();
}

void ChannelSelect::channel(const bool &direction)
{
	int i = 0;

	_old = _lcd->intValue();
	if(direction) {
		_full = _old + 1;
		i = 1;
	} else {
		_full = _old - 1;
		i = -1;
	}

	while(!_channels.contains(_full) && _full>0 && _full<=_channels.at(_channels.size()-1))
		_full+=i;

	display();
}

void ChannelSelect::back()
{
	channel(false);
}

void ChannelSelect::next()
{
	channel(true);
}

void ChannelSelect::keyPressed()
{
	QShortcut *tmp = qobject_cast<QShortcut *>(sender());
	process(tmp->key().toString().toInt());
}
