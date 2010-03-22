/****************************************************************************
* ChannelSelect.h: Channel selector
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

#ifndef TANO_CHANNELSELECT_H_
#define TANO_CHANNELSELECT_H_

#include <QtCore/QTimer>
#include <QtGui/QLCDNumber>
#include <QtGui/QShortcut>

class ChannelSelect : public QObject
{
Q_OBJECT
public:
	ChannelSelect(QWidget *parent, QLCDNumber *number, const QList<int> &list);
	virtual ~ChannelSelect();

public slots:
	void back();
	void next();
	void channel(const bool &direction);

private slots:
	void keyPressed();
	void display();

signals:
	void channelSelect(const int);

private:
	void process(const int &key);

	QList<QShortcut*> _key;
	QList<int> _channels;
	QLCDNumber *_lcd;
	QTimer *_timer;
	int _num;
	int _number[3];
	int _full;
	int _old;
};

#endif // TANO_CHANNELSELECT_H_
