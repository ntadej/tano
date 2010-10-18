/****************************************************************************
* CurrentTime.h: Displays time
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

#ifndef TANO_CURRENTTIME_H_
#define TANO_CURRENTTIME_H_

#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

class CurrentTime : public QWidget
{
Q_OBJECT
public:
	CurrentTime(QWidget *parent = 0);
	~CurrentTime();

private slots:
	void updateTime();

private:
	QLabel *_time;
	QTimer *_timer;
};

#endif // TANO_CURRENTTIME_H_
