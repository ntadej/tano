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

#include <QtCore/QObject>

#include "XmltvEnums.h"

Tano::Xmltv::CrewMemberType Tano::Xmltv::crewMemberType(const QString &type)
{
    if (type == QObject::tr("Director"))
        return Tano::Xmltv::Director;
    else if (type == QObject::tr("Actor"))
        return Tano::Xmltv::Actor;
    else if (type == QObject::tr("Writer"))
        return Tano::Xmltv::Writer;
    else if (type == QObject::tr("Adapter"))
        return Tano::Xmltv::Adapter;
    else if (type == QObject::tr("Producer"))
        return Tano::Xmltv::Producer;
    else if (type == QObject::tr("Composer"))
        return Tano::Xmltv::Composer;
    else if (type == QObject::tr("Editor"))
        return Tano::Xmltv::Editor;
    else if (type == QObject::tr("Presenter"))
        return Tano::Xmltv::Presenter;
    else if (type == QObject::tr("Commentator"))
        return Tano::Xmltv::Commentator;
    else if (type == QObject::tr("Guest"))
        return Tano::Xmltv::Guest;
    else
        return Tano::Xmltv::All;
}

QString Tano::Xmltv::crewMemberType(const CrewMemberType &type)
{
    switch (type)
    {
    case Tano::Xmltv::Director:
        return QObject::tr("Director");
    case Tano::Xmltv::Actor:
        return QObject::tr("Actor");
    case Tano::Xmltv::Writer:
        return QObject::tr("Writer");
    case Tano::Xmltv::Adapter:
        return QObject::tr("Adapter");
    case Tano::Xmltv::Producer:
        return QObject::tr("Producer");
    case Tano::Xmltv::Composer:
        return QObject::tr("Composer");
    case Tano::Xmltv::Editor:
        return QObject::tr("Editor");
    case Tano::Xmltv::Presenter:
        return QObject::tr("Presenter");
    case Tano::Xmltv::Commentator:
        return QObject::tr("Commentator");
    case Tano::Xmltv::Guest:
        return QObject::tr("Guest");
    default:
        return QString();
    }
}

Tano::Xmltv::LenghtUnits Tano::Xmltv::lenghtUnits(const QString &type)
{
    if (type == "seconds")
        return Tano::Xmltv::Seconds;
    else if (type == "minutes")
        return Tano::Xmltv::Minutes;
    else if (type == "hours")
        return Tano::Xmltv::Hours;
    else
        return Tano::Xmltv::Seconds;
}

QString Tano::Xmltv::lenghtUnits(const LenghtUnits &type)
{
    switch (type)
    {
    case Tano::Xmltv::Seconds:
        return QObject::tr("seconds");
    case Tano::Xmltv::Minutes:
        return QObject::tr("minutes");
    case Tano::Xmltv::Hours:
        return QObject::tr("hours");
    default:
        return QString();
    }
}

QString Tano::Xmltv::lenghtUnitsShort(const LenghtUnits &type)
{
    if (type == Tano::Xmltv::Seconds)
        return QObject::tr("s");
    else if (type == Tano::Xmltv::Minutes)
        return QObject::tr("min");
    else if (type == Tano::Xmltv::Hours)
        return QObject::tr("h");
    else
        return QString();
}
