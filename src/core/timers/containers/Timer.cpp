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

#include <QtCore/qmath.h>

#include "Timer.h"

Timer::Timer(const QString &name,
             const QString &channel,
             const QString &url,
             const Type &type,
             QObject *parent)
    : ListItem(parent),
      _name(name),
      _channel(channel),
      _url(url),
      _type(type)
{
    _file = "";
    _date = QDate::currentDate();

    float min = QTime::currentTime().minute();
    min /= 15;
    _startTime = QTime(QTime::currentTime().hour() + qCeil(min)/4, (qCeil(min) * 15) % 60);
    _endTime = QTime(QTime::currentTime().hour() + 1 + qCeil(min)/4, (qCeil(min) * 15) % 60);
    _state = Enabled;
}

Timer::Timer(Timer *timer)
    : ListItem(timer->parent())
{
    _file = "";
    _state = Enabled;

    _name = timer->name();
    _channel = timer->channel();
    _url = timer->url();
    _date = timer->date();
    _startTime = timer->startTime();
    _endTime = timer->endTime();
    _type = timer->type();
}

Timer::Timer(QObject *parent)
    : ListItem(parent) { }

Timer::~Timer() { }

QHash<int, QByteArray> Timer::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DisplayIconRole] = "displayIcon";
    names[NameRole] = "name";
    names[ChannelRole] = "channel";
    names[UrlRole] = "url";
    names[FileRole] = "file";
    names[DateRole] = "date";
    names[StartTimeRole] = "start";
    names[EndTimeRole] = "end";
    names[StartDateTimeRole] = "startdate";
    names[EndDateTimeRole] = "enddate";
    names[TypeRole] = "type";
    names[StateRole] = "state";
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
    case UrlRole:
        return url();
    case DateRole:
        return date();
    case StartTimeRole:
        return startTime();
    case EndTimeRole:
        return endTime();
    case StartDateTimeRole:
        return startDateTime();
    case EndDateTimeRole:
        return endDateTime();
    case TypeRole:
        return type();
    case StateRole:
        return state();
    default:
        return QVariant();
    }
}

QString Timer::display() const
{
    if (type() != Once && type() != Instant)
        return QString("%1 (%2) - %3 - %4 %5 %6, %7")
            .arg(name(), states()[state()], channel(),
                 date().toString("dd.M.yyyy"), tr("at"), startTime().toString("hh:mm"), typesLong()[type()]);
    else
        return QString("%1 (%2) - %3 - %4 %5 %6")
            .arg(name(), states()[state()], channel(),
                 date().toString("dd.M.yyyy"), tr("at"), startTime().toString("hh:mm"));
}

QIcon Timer::displayIcon() const
{
    if (state() == Finished)
        return QIcon::fromTheme("video-x-generic");
    else if (state() == Recording)
        return QIcon::fromTheme("media-record");
    else
        return QIcon::fromTheme("time-admin");
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

void Timer::setUrl(const QString &url)
{
    if (_url != url) {
        _url = url;
        emit dataChanged();
    }
}

void Timer::setFile(const QString &file)
{
    if (_file != file) {
        _file = file;
        emit dataChanged();
    }
}

void Timer::setDate(const QDate &date)
{
    if (_date != date) {
        _date = date;
        emit dataChanged();
    }
}

void Timer::setStartTime(const QTime &startTime)
{
    if (_startTime != startTime) {
        _startTime = startTime;
        emit dataChanged();
    }
}

void Timer::setEndTime(const QTime &endTime)
{
    if (_endTime != endTime) {
        _endTime = endTime;
        emit dataChanged();
    }
}

void Timer::setType(const Type &type)
{
    if (_type != type) {
        _type = type;
        emit dataChanged();
    }
}

void Timer::setState(const State &state)
{
    if (_state != state) {
        _state = state;
        emit dataChanged();
    }
}

QDateTime Timer::startDateTime() const
{
    return QDateTime(date(), startTime());
}

QDateTime Timer::endDateTime() const
{
    if (startTime() > endTime())
        return QDateTime(date().addDays(1), endTime());
    else
        return QDateTime(date(), endTime());
}

QStringList Timer::states()
{
    QStringList list;
    list << QObject::tr("Enabled")
         << QObject::tr("Disabled")
         << QObject::tr("Recording")
         << QObject::tr("Finished")
         << QObject::tr("Expired");

    return list;
}

QStringList Timer::types()
{
    QStringList list;
    list << QObject::tr("Once")
         << QObject::tr("Daily")
         << QObject::tr("Weekly")
         << QObject::tr("Weekdays")
         << QObject::tr("Instant");

    return list;
}

QStringList Timer::typesLong()
{
    QStringList list;
    list << QObject::tr("repeats only once")
         << QObject::tr("repeats daily")
         << QObject::tr("repeats weekly")
         << QObject::tr("repeats on weekdays")
         << QObject::tr("instant recording");

    return list;
}
