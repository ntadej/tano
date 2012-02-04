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

#include "Enums.h"

QString Tano::timerTypeString(const TimerType &type)
{
    switch (type)
    {
    case Tano::Once:
        return QObject::tr("Once");
    case Tano::Daily:
        return QObject::tr("Daily");
    case Tano::Weekly:
        return QObject::tr("Weekly");
    default:
        return QString("error");
    }
}
