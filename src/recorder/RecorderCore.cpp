/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QPluginLoader>

#include "core/Common.h"
#include "core/Settings.h"
#include "core/PluginsLoader.h"
#include "recorder/RecorderCore.h"

RecorderCore::RecorderCore(QObject *parent)
	: QObject(parent),
	_coreBackend(true),
	_isRecording(false),
	_isTimer(false),
	_instance(0),
	_player(0),
	_plugin(0)
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(time()));
}

RecorderCore::~RecorderCore()
{
	stop();

	delete _player;
	delete _instance;
	delete _plugin;
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
	fileName.append(QDateTime::currentDateTime().toString("-dd_MM_yyyy-hh_mm_ss"));
	fileName.append(".ts");

	_output = fileName;

	if(_coreBackend) {
		if(_player)
			delete _player;
		if(_instance)
			delete _instance;

		_instance = new VlcInstance(Tano::vlcQtRecorderArgs(_output));
		_player = new VlcMediaPlayer();

		_player->open(url);
		_player->play();
	} else {
		_plugin->record(channel, url, fileName);
	}

	_isRecording = true;
	_isTimer = false;

	_time = QTime(0, 0);
	_timer->start(500);
}

void RecorderCore::record(Timer *timer)
{

}

void RecorderCore::setBackend(const QString &backend)
{
	if(backend == Settings::DEFAULT_RECORDER_BACKEND) {
		_coreBackend = true;
	} else {
		_coreBackend = false;

		if(_plugin)
			delete _plugin;

		PluginsLoader *loader = new PluginsLoader();
		for(int i = 0; i < loader->recorderPlugin().size(); i++)
			if(loader->recorderName()[i] == backend)
				_plugin = loader->recorder(loader->recorderPlugin()[i]);
		delete loader;
	}
}

void RecorderCore::stop()
{
	if(_coreBackend) {
		_player->stop();
	} else {
		_plugin->stop();
	}

	_isRecording = false;
	_isTimer = false;

	_timer->stop();
	emit elapsed(QTime(0, 0));
}

void RecorderCore::time()
{
	_time = _time.addMSecs(500);
	emit elapsed(_time);
}
