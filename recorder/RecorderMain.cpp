/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#include "core/ConsoleOutput.h"
#include "recorder/RecorderMain.h"

RecorderMain::RecorderMain(QObject *parent)
	: QObject(parent)
{
	ConsoleOutput::welcomeMessage();

	_core = new RecorderCore(this);
	connect(_core, SIGNAL(elapsed(int)), this, SIGNAL(elapsed(int)));
	connect(_core, SIGNAL(timerStop()), this, SIGNAL(timerStop()));
	connect(_core, SIGNAL(timer(QString, QString)), this, SIGNAL(timer(QString, QString)));

	_time = new RecorderTimeManager(this);
	connect(_time, SIGNAL(timer(Timer *)), _core, SLOT(record(Timer *)));
}

RecorderMain::~RecorderMain()
{
	delete _core;
}
