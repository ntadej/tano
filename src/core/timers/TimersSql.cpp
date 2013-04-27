/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtXml/QXmlSimpleReader>
#include <QtXml/QXmlInputSource>

#include "core/Resources.h"
#include "timers/TimersSql.h"
#include "timers/containers/Timer.h"
#include "timers/old/TimersHandler.h"

TimersSql::TimersSql()
{
    _db = QSqlDatabase::addDatabase("QSQLITE", "Timers");
    _db.setDatabaseName(Tano::Resources::settingsPath() + "/recordings.db");
}

TimersSql::~TimersSql()
{
    close();
}

void TimersSql::close()
{
    _db.close();
}

bool TimersSql::open()
{
    if (!_db.open()) {
        qCritical() << "Cannot open database:" << "Timers";
        return false;
    } else {
        qDebug() << "Connected to database:" << "Timers";
    }

    QSqlQuery q = query();
    q.exec("CREATE TABLE IF NOT EXISTS `timers` ("
           "`id` INTEGER PRIMARY KEY AUTOINCREMENT, "
           "`name` varchar(100), "
           "`channel` varchar(100), "
           "`url` varchar(100), "
           "`file` varchar(100), "
           "`date` varchar(100), "
           "`start` varchar(100), "
           "`end` varchar(100), "
           "`type` int, "
           "`state` int"
           ")");

    qDebug() << "Timers:" << "Available tables:" << _db.tables();

    readTimersOld();

    return true;
}

QSqlQuery TimersSql::query()
{
    return QSqlQuery(_db);
}

int TimersSql::addTimer(Timer *timer)
{
    QSqlQuery q = query();
    q.prepare("INSERT INTO `timers` (`name`, `channel`, `url`, `file`, `date`, `start`, `end`, `type`, `state`)"
              "VALUES (:name, :channel, :url, :file, :date, :start, :end, :type, :state)");
    q.bindValue(":name", timer->name());
    q.bindValue(":channel", timer->channel());
    q.bindValue(":url", timer->url());
    q.bindValue(":file", timer->file());
    q.bindValue(":date", timer->date().toString());
    q.bindValue(":start", timer->startTime().toString());
    q.bindValue(":end", timer->endTime().toString());
    q.bindValue(":type", timer->type());
    q.bindValue(":state", timer->state());
    q.exec();

    return q.lastInsertId().toInt();
}

Timer *TimersSql::createTimer(const QString &name,
                              const QString &channel,
                              const QString &url,
                              const Timer::Type &type)
{
    Timer *timer = new Timer(name, channel, url, type);
    timer->setId(addTimer(timer));

    return timer;
}

void TimersSql::updateTimer(Timer *timer)
{
   // QSqlQuery q = query();
   // q.exec("UPDATE `channels` SET `" + column + "` = '" + data + "' WHERE `id` = '" + id + "'");
}

void TimersSql::deleteTimer(Timer *timer)
{
   // QSqlQuery q = query();
   // q.exec("UPDATE `channels` SET `" + column + "` = '" + data + "' WHERE `id` = '" + id + "'");
}

Timer *TimersSql::timer(const QString &id)
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `timers` WHERE `id` = " + id);
    if (q.next()) {
        Timer *timer = new Timer();
        timer->setName(q.value(1).toString());
        timer->setChannel(q.value(2).toString());
        timer->setUrl(q.value(3).toString());
        timer->setFile(q.value(4).toString());
        timer->setDate(QDate::fromString(q.value(5).toString()));
        timer->setStartTime(QTime::fromString(q.value(6).toString()));
        timer->setEndTime(QTime::fromString(q.value(7).toString()));
        timer->setType(Timer::Type(q.value(8).toInt()));
        timer->setState(Timer::State(q.value(9).toInt()));

        return timer;
    }

    return 0;
}

Timer *TimersSql::timerToRecord()
{
    QSqlQuery q = query();
    q.exec("SELECT * FROM `timers` WHERE `state` = " + QString::number(Timer::Enabled) + " ORDER BY `date`, `start`");
    if (q.next()) {
        Timer *timer = new Timer();
        timer->setName(q.value(1).toString());
        timer->setChannel(q.value(2).toString());
        timer->setUrl(q.value(3).toString());
        timer->setFile(q.value(4).toString());
        timer->setDate(QDate::fromString(q.value(5).toString()));
        timer->setStartTime(QTime::fromString(q.value(6).toString()));
        timer->setEndTime(QTime::fromString(q.value(7).toString()));
        timer->setType(Timer::Type(q.value(8).toInt()));
        timer->setState(Timer::State(q.value(9).toInt()));

        return timer;
    }

    return 0;
}

bool TimersSql::startTransaction()
{
    return _db.transaction();
}

bool TimersSql::endTransaction()
{
    return _db.commit();
}

void TimersSql::readTimersOld()
{
    QScopedPointer<TimersHandler> handler(new TimersHandler());

    QXmlSimpleReader reader;
    reader.setContentHandler(handler.data());
    reader.setErrorHandler(handler.data());

    QFile f(Tano::Resources::recordings());
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return;

    QXmlInputSource xmlInputSource(&f);
    if (!reader.parse(xmlInputSource))
        return;

    QList<Timer *> timers = handler->timersList();
    foreach (Timer* timer, timers) {
        addTimer(timer);
    }
}
