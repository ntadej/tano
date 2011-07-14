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

#include "container/core/Channel.h"
#include "playlist/PlaylistModel.h"
#include "playlist/generators/XmltvIdGenerator.h"

XmltvIdGenerator::XmltvIdGenerator(const QString &file)
{
    _file = new QFile(file);
}

XmltvIdGenerator::~XmltvIdGenerator()
{
    delete _file;
}

bool XmltvIdGenerator::write(PlaylistModel *model)
{
    if (!_file->open(QFile::WriteOnly | QFile::Text))
        return false;

    _out.setDevice(_file);
    _out.setCodec("UTF-8");

    for (int i = 0; i < model->rowCount(); i++) {
        generateItem(model->row(i));
    }
    return true;
}

void XmltvIdGenerator::generateItem(Channel *channel)
{
    if(!channel->epg().isEmpty())
        _out << channel->name() << "=" << channel->epg() << "\n";
}
