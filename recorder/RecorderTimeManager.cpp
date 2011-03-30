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

#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include "container/Timer.h"
#include "core/Settings.h"
#include "recorder/RecorderTimeManager.h"
#include "xml/TimersHandler.h"

RecorderTimeManager::RecorderTimeManager(QObject *parent)
	: QObject(parent),
	_timersHandler(0)
{
	Settings *settings = new Settings(this);
	_path = settings->path();
	delete settings;

	_timer = new QTimer();
	connect(_timer, SIGNAL(timeout()), this, SLOT(check()));
	_timer->start(3000);

	updateTimers();
}

RecorderTimeManager::~RecorderTimeManager()
{
	delete _timer;
	delete _timersHandler;
}

void RecorderTimeManager::check()
{
	for(int i = 0; i < _timersList.size(); i++) {
		if(_timersList[i]->startTime() <= QTime::currentTime() && _timersList[i]->endTime() >= QTime::currentTime()) {
			if(!_timersList[i]->isDisabled() && !_timersList[i]->isRecording()) {
				emit timer(_timersList[i]);
				qDebug() << "Timer" << _timersList[i]->name() << "started";
			}
		}
	}
}

void RecorderTimeManager::readTimers()
{
	if(_timersHandler)
		delete _timersHandler;

	_timersHandler = new TimersHandler();
	QString fileName = _path + "timers.tano.xml";
	_timersHandler->clear();

	QXmlSimpleReader reader;
	reader.setContentHandler(_timersHandler);
	reader.setErrorHandler(_timersHandler);

	if(!QFileInfo(fileName).exists())
		return;

	QFile file(fileName);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;

	QXmlInputSource xmlInputSource(&file);
	if (!reader.parse(xmlInputSource))
		return;

	file.close();
}

void RecorderTimeManager::updateTimers()
{
	readTimers();
	_timersList.clear();
	_timersList = _timersHandler->timersList();
	qDebug() << _timersList.size() << "timers loaded";
	for(int i = 0; i < _timersHandler->timersList().size(); i++)
		qDebug() << _timersHandler->timersList()[i]->name();
}
