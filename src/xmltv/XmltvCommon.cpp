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

#include <QtCore/QDateTime>

#include "settings/Settings.h"
#include "xmltv/XmltvCommon.h"

QString Tano::Xmltv::commaSeparator()
{
    QString s = ";";
    return s;
}

QString Tano::Xmltv::dateFormat()
{
    QString format = "yyyyMMddHHmmss";
    return format;
}

QString Tano::Xmltv::dateFormatDisplay()
{
    QString format = "dd.MM.yyyy";
    return format;
}

int Tano::Xmltv::parse(const QString &data)
{
    QScopedPointer<Settings> settings(new Settings());

    QString d = data;
    QRegExp exp = QRegExp(" .[0-9][0-9][0-9][0-9]");
    QDateTime current = QDateTime::currentDateTime();

    exp.indexIn(d);
    QString zone = exp.cap();
    zone.remove(" +");
    d.remove(exp);

    QDateTime date = QDateTime::fromString(d, Tano::Xmltv::dateFormat());
    date.addSecs(- zone.left(2).toInt() * 3600 - zone.right(2).toInt() * 60);

    return date.toTime_t() + timeZoneDiff() + settings->xmltvHoursOffset() * 3600;
}

QString Tano::Xmltv::timeFormatDisplay()
{
    QString format = "HH:mm";
    return format;
}

int Tano::Xmltv::timeZoneDiff()
{
    QDateTime local = QDateTime::currentDateTime();
    local.setTime(QTime(0, 0));
    QDateTime utc = local.toUTC();
    local.setTimeSpec(Qt::LocalTime);
    QDateTime offset = local.toUTC();
    QTime properTimeOffset = QTime(offset.time().hour(), offset.time().minute());
    offset.setTimeSpec(Qt::LocalTime);
    utc.setTimeSpec(Qt::UTC);

    bool isNegative;
    if (offset.secsTo(utc) < 0) {
        isNegative = true;
    } else {
        isNegative = false;
        properTimeOffset.setHMS(24 - properTimeOffset.hour() - (properTimeOffset.minute()/60.0) - (properTimeOffset.second()/3600.0), properTimeOffset.minute() - (properTimeOffset.second()/60.0), properTimeOffset.second());
        if (!properTimeOffset.isValid()) { //Midnight case
            properTimeOffset.setHMS(0,0,0);
        }
    }

    if (isNegative)
        return properTimeOffset.secsTo(QTime(0, 0));
    else
        return - properTimeOffset.secsTo(QTime(0, 0));
}
