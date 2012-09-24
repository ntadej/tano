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

#include <vlc-qt/Instance.h>
#include <vlc-qt/Media.h>
#include <vlc-qt/MediaPlayer.h>

#include "core/Common.h"
#include "core/timers/containers/Timer.h"

#include "recorder/RecorderCore.h"

RecorderCore::RecorderCore(QObject *parent)
    : QObject(parent),
      _isRecording(false),
      _isTimer(false),
      _instance(0),
      _media(0),
      _player(0)
{
    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(time()));
}

RecorderCore::~RecorderCore()
{
    stop();

    delete _media;
    delete _player;
    delete _timer;
}

void RecorderCore::record(Timer *t)
{
    stop();

    _outputName = Tano::recordingFileName(t->name(), t->channel(), t->date(), t->startTime());
    _outputPath = _defaultPath;
    _currentEndTime = t->endDateTime();

    if (_media)
        delete _media;

    _media = new VlcMedia(t->url(), _instance);
    _output = _media->record(_outputName, _outputPath, Vlc::TS);
    t->setFile(_output);

    _player->open(_media);
    _player->play();

    _isRecording = true;
    _isTimer = (t->type() != Timer::Instant);
    t->setState(Timer::Recording);

    _time = 0;
    _timer->start(500);
}

void RecorderCore::setMediaInstance(VlcInstance *instance)
{
    _instance = instance;

    if (_player)
        delete _player;
    _player = new VlcMediaPlayer(_instance);
}

void RecorderCore::stop()
{
    if (_player)
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

    if (isTimer() && QDateTime::currentDateTime() >= _currentEndTime) {
        emit timerStop();
    }
}
