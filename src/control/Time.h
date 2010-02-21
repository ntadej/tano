/****************************************************************************
* Time.h: Main time manager
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

#ifndef TANO_TIME_H_
#define TANO_TIME_H_

#include <QtCore/QList>
#include <QtCore/QTimer>

#include "../recorder/Timer.h"

class Time : public QObject
{
Q_OBJECT
public:
	Time();
	virtual ~Time();

	void addTimer(const Timer *t);
	void removeTimer(const Timer *t);

signals:
	void startTimer(const Timer*);
	void stopTimer(const Timer*);

private slots:
	void check();

private:
	QTimer *_timer;
	QList<const Timer*> _timersList;
};

#endif // TANO_TIME_H_
