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

#include <QtCore/QObject>

#include "XmltvEnums.h"

Tano::Xmltv::LenghtUnits Tano::Xmltv::lenghtUnits(const QString &type)
{
    if(type == "seconds")
        return Tano::Xmltv::Seconds;
    else if(type == "minutes")
        return Tano::Xmltv::Minutes;
    else if(type == "hours")
        return Tano::Xmltv::Hours;
    else
        return Tano::Xmltv::Seconds;
}

QString Tano::Xmltv::lenghtUnits(const LenghtUnits &type)
{
    if(type == Tano::Xmltv::Seconds)
        return QString(QObject::tr("seconds"));
    else if(type == Tano::Xmltv::Minutes)
        return QString(QObject::tr("minutes"));
    else if(type == Tano::Xmltv::Hours)
        return QString(QObject::tr("hours"));
    else
        return QString();
}

QString Tano::Xmltv::lenghtUnitsShort(const LenghtUnits &type)
{
    if(type == Tano::Xmltv::Seconds)
        return QString(QObject::tr("s"));
    else if(type == Tano::Xmltv::Minutes)
        return QString(QObject::tr("min"));
    else if(type == Tano::Xmltv::Hours)
        return QString(QObject::tr("h"));
    else
        return QString();
}
