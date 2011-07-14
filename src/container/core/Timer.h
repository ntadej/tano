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

#ifndef TANO_TIMER_H_
#define TANO_TIMER_H_

#include <QtCore/QDateTime>
#include <QtCore/QString>

#include "core/Enums.h"

class Timer
{
public:
    Timer(const QString &name,
          const QString &channel = 0,
          const QString &playlist = 0,
          const int &num = 0,
          const QString &url = 0);
    ~Timer();

public:
    QString name() const { return _name; }
    void setName(const QString &name) { _name = name; }
    QString channel() const { return _channel; }
    void setChannel(const QString &channel) { _channel = channel; }
    QString playlist() const { return _playlist; }
    void setPlaylist(const QString &playlist) { _playlist = playlist; }
    QString url() const { return _url; }
    void setUrl(const QString &url) { _url = url; }
    int num() const { return _num; }
    void setNum(const int &num) { _num = num; }
    QDateTime startTime() const { return _startTime; }
    void setStartTime(const QDateTime &startTime) { _startTime = startTime; }
    QDateTime endTime() const { return _endTime; }
    void setEndTime(const QDateTime &endTime) { _endTime = endTime; }
    bool isDisabled() const { return _disabled; }
    void setDisabled(const bool &disabled) { _disabled = disabled; }
    Tano::TimerType type() const { return _type; }
    void setType(const Tano::TimerType &type) { _type = type; }
    bool isRecording() const { return _recording; }
    void setRecording(const bool &recording) { _recording = recording; }

private:
    QString _name;
    QString _channel;
    QString _playlist;
    QString _url;
    int _num;
    QDateTime _startTime;
    QDateTime _endTime;
    bool _disabled;
    Tano::TimerType _type;
    bool _recording;
};

#endif // TANO_TIMER_H_
