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
