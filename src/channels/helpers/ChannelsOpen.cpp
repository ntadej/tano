/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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
#include <QtCore/QScopedPointer>

#include "channels/handlers/CSVHandler.h"
#include "channels/handlers/M3UHandler.h"

#include "ChannelsOpen.h"

ChannelsOpen::ChannelsOpen() { }

ChannelsOpen::~ChannelsOpen() { }

void ChannelsOpen::openCSVFile(const QString &file,
                               const CSVInfo &info)
{
    QScopedPointer<CSVHandler> open(new CSVHandler());
    open->setParameters(info.separator, info.header, info.columns);
    open->processFile(file);

    _list = open->channelList();
    _name = QObject::tr("CSV channels list");
}

void ChannelsOpen::openM3UFile(const QString &file)
{
    QScopedPointer<M3UHandler> open(new M3UHandler());
    open->processFile(file);

    _list = open->channelList();
    _name = open->name();
}
