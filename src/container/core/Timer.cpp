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

#include "Timer.h"

Timer::Timer(const QString &name,
             const QString &channel,
             const QString &playlist,
             const int &num,
             const QString &url,
             QObject *parent)
    : ListItem(parent),
      _name(name),
     _channel(channel),
     _playlist(playlist),
     _num(num),
     _url(url)
{
    _startTime = QDateTime(QDate::currentDate(), QTime(QTime::currentTime().hour(), QTime::currentTime().minute() + 1, 0, 0));
    _endTime = _startTime.addSecs(3600);
    _disabled = false;
    _recording = false;
}

Timer::~Timer() { }

QHash<int, QByteArray> Timer::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DisplayIconRole] = "displayIcon";
    names[NameRole] = "name";
    names[ChannelRole] = "channel";
    names[PlaylistRole] = "playlist";
    names[UrlRole] = "url";
    names[NumRole] = "num";
    names[StartTimeRole] = "start";
    names[EndTimeRole] = "end";
    names[DisabledRole] = "disabled";
    names[TypeRole] = "type";
    names[RecordingRole] = "recording";
    return names;
}

QVariant Timer::data(int role) const
{
    switch (role)
    {
    case DisplayRole:
        return display();
    case DisplayIconRole:
        return displayIcon();
    case NameRole:
        return name();
    case ChannelRole:
        return channel();
    case PlaylistRole:
        return playlist();
    case UrlRole:
        return url();
    case NumRole:
        return num();
    case StartTimeRole:
        return startTime();
    case EndTimeRole:
        return endTime();
    case DisabledRole:
        return isDisabled();
    case TypeRole:
        return type();
    case RecordingRole:
        return isRecording();
    default:
        return QVariant();
    }
}

QString Timer::display() const
{
    return QString("%1 (%2)").arg(name(), (isDisabled() ? tr("Disabled or expired") : tr("Active")));
}

QIcon Timer::displayIcon() const
{
    return QIcon(":/icons/16x16/timer.png");
}

void Timer::setName(const QString &name)
{
    if (_name != name) {
        _name = name;
        emit dataChanged();
    }
}

void Timer::setChannel(const QString &channel)
{
    if (_channel != channel) {
        _channel = channel;
        emit dataChanged();
    }
}

void Timer::setPlaylist(const QString &playlist)
{
    if (_playlist != playlist) {
        _playlist = playlist;
        emit dataChanged();
    }
}

void Timer::setUrl(const QString &url)
{
    if (_url != url) {
        _url = url;
        emit dataChanged();
    }
}

void Timer::setNum(const int &num)
{
    if (_num != num) {
        _num = num;
        emit dataChanged();
    }
}

void Timer::setStartTime(const QDateTime &startTime)
{
    if (_startTime != startTime) {
        _startTime = startTime;
        emit dataChanged();
    }
}

void Timer::setEndTime(const QDateTime &endTime)
{
    if (_endTime != endTime) {
        _endTime = endTime;
        emit dataChanged();
    }
}

void Timer::setDisabled(const bool &disabled)
{
    if (_disabled != disabled) {
        _disabled = disabled;
        emit dataChanged();
    }
}

void Timer::setType(const Tano::TimerType &type)
{
    if (_type != type) {
        _type = type;
        emit dataChanged();
    }
}

void Timer::setRecording(const bool &recording)
{
    if (_recording != recording) {
        _recording = recording;
        emit dataChanged();
    }
}
