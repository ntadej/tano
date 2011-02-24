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

#include "Enums.h"

Tano::EpgType Tano::epgType(const QString &type)
{
	if(type == "slovenia")
		return Tano::Slovenia;
	else if(type == "xmltv")
		return Tano::XMLTV;
	else
		return Tano::Slovenia;
}

QString Tano::epgType(const EpgType &type)
{
	if(type == Tano::Slovenia)
		return QString("slovenia");
	else if(type == Tano::XMLTV)
		return QString("xmltv");
	else
		return QString("error");
}

Tano::TimerType Tano::timerType(const int &type)
{
	if(type == 0)
		return Tano::Once;
	else if(type == 1)
		return Tano::Daily;
	else if(type == 2)
		return Tano::Weekly;
	else
		return Tano::Once;
}

int Tano::timerType(const TimerType &type)
{
	if(type == Tano::Once)
		return 0;
	else if(type == Tano::Daily)
		return 1;
	else if(type == Tano::Weekly)
		return 2;
	else
		return -1;
}

QString Tano::timerTypeString(const TimerType &type)
{
	if(type == Tano::Once)
		return QString(QObject::tr("Once"));
	else if(type == Tano::Daily)
		return QString(QObject::tr("Daily"));
	else if(type == Tano::Weekly)
		return QString(QObject::tr("Weekly"));
	else
		return QString("error");
}
