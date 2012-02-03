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
#include "container/playlist/CSVInfo.h"
#include "playlist/handlers/CSVHandler.h"
#include "playlist/handlers/JsHandler.h"
#include "playlist/handlers/M3UHandler.h"
#include "playlist/handlers/TanoHandlerOld.h"

PlaylistOpen::PlaylistOpen() { }

PlaylistOpen::~PlaylistOpen() { }

void PlaylistOpen::openCSVFile(const QString &file,
                               const CSVInfo &info)
{
    CSVHandler *open = new CSVHandler();
    open->setParameters(info.separator(), info.header(), info.columns());
    open->processFile(file);

    _list = open->channelList();
    _name = QObject::tr("CSV channel list");

    delete open;
}

void PlaylistOpen::openJsFile(const QString &file)
{
    JsHandler *open = new JsHandler();
    open->processFile(file);

    _list = open->channelList();
    _name = QObject::tr("Sagem STB channel list");

    delete open;
}

void PlaylistOpen::openM3UFile(const QString &file)
{
    M3UHandler *open = new M3UHandler();
    open->processFile(file);

    _list = open->channelList();
    _name = open->name();

    delete open;
}

void PlaylistOpen::openTanoOldFile(const QString &file)
{
    TanoHandlerOld *open = new TanoHandlerOld();

    QXmlSimpleReader reader;
    reader.setContentHandler(open);
    reader.setErrorHandler(open);

    QFile f(file);
    if (!f.open(QFile::ReadOnly | QFile::Text))
        return;

    QXmlInputSource xmlInputSource(&f);
    if (!reader.parse(xmlInputSource))
        return;

    _list = open->channelList();
    _name = open->name();

    delete open;
}
