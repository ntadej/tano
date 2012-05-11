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

#include "PlaylistOpen.h"
#include "playlist/handlers/CSVHandler.h"
#include "playlist/handlers/JsHandler.h"
#include "playlist/handlers/M3UHandler.h"

PlaylistOpen::PlaylistOpen() { }

PlaylistOpen::~PlaylistOpen() { }

void PlaylistOpen::openCSVFile(const QString &file,
                               const CSVInfo &info)
{
    QScopedPointer<CSVHandler> open(new CSVHandler());
    open->setParameters(info.separator, info.header, info.columns);
    open->processFile(file);

    _list = open->channelList();
    _name = QObject::tr("CSV channel list");
}

void PlaylistOpen::openJsFile(const QString &file)
{
    QScopedPointer<JsHandler> open(new JsHandler());
    open->processFile(file);

    _list = open->channelList();
    _name = QObject::tr("Sagem STB channel list");
}

void PlaylistOpen::openM3UFile(const QString &file)
{
    QScopedPointer<M3UHandler> open(new M3UHandler());
    open->processFile(file);

    _list = open->channelList();
    _name = open->name();
}
