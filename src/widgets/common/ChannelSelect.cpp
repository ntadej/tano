/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#if defined(Qt5)
    #include <QtWidgets/QLCDNumber>
    #include <QtWidgets/QShortcut>
#elif defined(Qt4)
    #include <QtGui/QLCDNumber>
    #include <QtGui/QShortcut>
#endif

#include "ChannelSelect.h"

ChannelSelect::ChannelSelect(QWidget *parent,
                             QLCDNumber *number,
                             const QList<int> &list)
    : QObject(parent),
      _channels(list),
      _lcd(number),
      _digit(1)
{
    _timer = new QTimer(parent);

    _number[0] = 0;
    _number[1] = 0;
    _number[2] = 0;

    for(int i = 0; i < 10; i++) {
        _key.append(new QShortcut(QKeySequence(QString().number(i)), parent, 0, 0, Qt::ApplicationShortcut));
        connect(_key[i], SIGNAL(activated()), this, SLOT(keyPressed()));
    }

    connect(_timer, SIGNAL(timeout()), this, SLOT(display()));
}

ChannelSelect::~ChannelSelect()
{
    delete _timer;
    qDeleteAll(_key);
}

void ChannelSelect::process(const int &key)
{
    if(_digit == 1) {
        _old = _lcd->intValue();
        _number[0] = key;
    } else if(_digit == 2) {
        _number[1] = _number[0];
        _number[0] = key;
    } else if(_digit == 3) {
        _number[2] = _number[1];
        _number[1] = _number[0];
        _number[0] = key;
    }

    _full = _number[2]*100 + _number[1]*10 + _number[0];
    _lcd->display(_full);

    if(_digit < 3) {
        _timer->start(1000);
        _digit++;
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
    _digit = 1;
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

    while(!_channels.contains(_full) && _full > 0 && _full <= _channels.at(_channels.size()-1))
        _full += i;

    display();
}

void ChannelSelect::keyPressed()
{
    QShortcut *tmp = qobject_cast<QShortcut *>(sender());
    process(tmp->key().toString().toInt());
}
