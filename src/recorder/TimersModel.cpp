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

#include <QtCore/QFile>
#include <QtXml/QXmlSimpleReader>
#include <QtXml/QXmlInputSource>

#include "container/core/Timer.h"
#include "core/Common.h"
#include "recorder/TimersGenerator.h"
#include "recorder/TimersHandler.h"
#include "recorder/TimersModel.h"

TimersModel::TimersModel(QObject *parent)
    : ListModel(new Timer, parent)
{
    readTimers();
}

TimersModel::~TimersModel() { }

Timer *TimersModel::find(const QString &id) const
{
    return qobject_cast<Timer *>(ListModel::find(id));
}

Timer *TimersModel::row(const int &row)
{
    return qobject_cast<Timer *>(ListModel::row(row));
}

Timer *TimersModel::takeRow(const int &row)
{
    return qobject_cast<Timer *>(ListModel::takeRow(row));
}

Timer *TimersModel::createTimer(const QString &name,
                                const QString &channel,
                                const QString &url,
                                const Tano::TimerType &type)
{
    Timer *newTimer = new Timer(name, channel, url, type, this);
    appendRow(newTimer);

    return newTimer;
}

void TimersModel::deleteTimer(Timer *timer)
{
    removeRow(indexFromItem(timer).row());
}

Timer *TimersModel::duplicateTimer(Timer *timer)
{
    Timer *newTimer = new Timer(timer);
    appendRow(newTimer);

    return newTimer;
}

void TimersModel::readTimers()
{
    QScopedPointer<TimersHandler> handler(new TimersHandler());

    QXmlSimpleReader reader;
    reader.setContentHandler(handler.data());
    reader.setErrorHandler(handler.data());

    QFile f(Tano::recordings());
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return;

    QXmlInputSource xmlInputSource(&f);
    if (!reader.parse(xmlInputSource))
        return;

    QList<Timer *> timers = handler->timersList();
    foreach (Timer* timer, timers) {
        appendRow(timer);
    }
}

void TimersModel::writeTimers()
{
    QFile f(Tano::recordings());
    if (!f.open(QFile::WriteOnly | QFile::Text))
        return;

    QScopedPointer<TimersGenerator> generator(new TimersGenerator(Tano::recordings()));
    generator->write(this);
}
