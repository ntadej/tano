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

#include "container/core/Timer.h"
#include "core/Common.h"
#include "core/Settings.h"
#include "daemon/recorder/RecorderCore.h"
#include "daemon/recorder/RecorderTimeManager.h"

RecorderCore::RecorderCore(QObject *parent)
    : QObject(parent),
    _isRecording(false),
    _isTimer(false),
    _instance(0),
    _player(0)
{
    settings();

    _manager = new RecorderTimeManager(this);
    connect(_manager, SIGNAL(timer(Timer *)), this, SLOT(record(Timer *)));

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

QString RecorderCore::fileName(const QString &channel,
                               const QString &path,
                               const QString &name) const
{
    QString f = QString(path);
    f.append("/");
    if(!name.isEmpty()) {
        f.append(QString(name).replace(" ", "_"));
        f.append("-");
    }
    f.append(QString(channel).replace(" ", "_"));
    f.append(QDateTime::currentDateTime().toString("-yyyyMMdd-hhmmss"));
    f.append(".ts");

    return f;
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
    _output = fileName(channel, path);

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

void RecorderCore::record(Timer *t)
{
    stop();

    _output = fileName(t->channel(), _defaultPath, t->name());

    if(_player)
        delete _player;
    if(_instance)
        delete _instance;

    _instance = new VlcInstance(Tano::vlcQtRecorderArgs(_output));
    _player = new VlcMediaPlayer();

    _player->open(t->url());
    _player->play();

    _isRecording = true;
    _isTimer = true;
    _currentEndTime = t->endTime().time().toString("hh:mm");
    _currentName = t->name() + QString(" (%1)").arg(t->channel());
    _currentUrl = t->url();
    t->setRecording(true);

    _time = 0;
    _timer->start(500);

    emit timer(_currentName, _currentUrl);
}

void RecorderCore::settings()
{
    Settings *settings = new Settings(this);
    _defaultPath = settings->recorderDirectory();
    delete settings;
}

void RecorderCore::stop()
{
    _player->stop();

    _currentEndTime = "";
    _currentName = "";
    _currentUrl = "";
    _isRecording = false;
    _isTimer = false;

    _timer->stop();
    emit elapsed(0);
}

void RecorderCore::time()
{
    _time += 500;
    emit elapsed(_time);

    if(QTime::currentTime() >= QTime::fromString(_currentEndTime, "hh:mm") && isTimer()) {
        emit timerStop();
    }
}

void RecorderCore::timerInfo()
{
    if(!_currentName.isEmpty())
        emit timer(_currentName, _currentUrl);
}

void RecorderCore::updateTimers()
{
    _manager->updateTimers();
}
