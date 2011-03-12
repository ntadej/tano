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

#include <vlc-qt/Instance.h>
#include <vlc-qt/MediaPlayer.h>

#include "container/Timer.h"
#include "core/Common.h"
#include "recorder/RecorderCore.h"

RecorderCore::RecorderCore(QObject *parent)
	: QObject(parent),
	_isRecording(false),
	_isTimer(false),
	_instance(0),
	_player(0)
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(time()));
}

RecorderCore::~RecorderCore()
{
	stop();

	delete _player;
	delete _instance;
	delete _timer;
}

QString RecorderCore::output() const
{
	if(isRecording())
		return _output;
	else
		return "error";
}

void RecorderCore::record(const QString &channel,
						  const QString &url,
						  const QString &path)
{
	QString fileName = QString(path);
	fileName.append("/");
	fileName.append(QString(channel).replace(" ","_"));
	fileName.append(QDateTime::currentDateTime().toString("-yyyyMMdd-hhmmss"));
	fileName.append(".ts");

	_output = fileName;

	if(_player)
		delete _player;
	if(_instance)
		delete _instance;

	_instance = new VlcInstance(Tano::vlcQtRecorderArgs(_output));
	_player = new VlcMediaPlayer();

	_player->open(url);
	_player->play();

	_isRecording = true;
	_isTimer = false;

	_time = 0;
	_timer->start(500);
}

void RecorderCore::record(Timer *timer)
{

}

void RecorderCore::stop()
{
	_player->stop();

	_isRecording = false;
	_isTimer = false;

	_timer->stop();
	emit elapsed(0);
}

void RecorderCore::time()
{
	_time += 500;
	emit elapsed(_time);
}
