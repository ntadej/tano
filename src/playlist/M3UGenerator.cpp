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

#include "container/Channel.h"
#include "core/Udpxy.h"
#include "playlist/M3UGenerator.h"

M3UGenerator::M3UGenerator(QTreeWidget *treeWidget,
                           const QString &name,
                           QMap<QTreeWidgetItem *, Channel *> map,
                           const Tano::M3UType &type)
    : _treeWidget(treeWidget),
      _name(name),
      _map(map),
      _type(type)
{
    _udpxy = new Udpxy();
}

M3UGenerator::~M3UGenerator() { }

bool M3UGenerator::write(QIODevice *device)
{
    _out.setDevice(device);
    _out.setCodec("UTF-8");
    _out << "#EXTM3U\n";
    if(_type != Tano::M3UClean) {
        _out << "#EXTNAME:"
             << _name
             << "\n\n";
    }
    for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i) {
        if(_type == Tano::M3UClean) {
            generateItemClean(_map[_treeWidget->topLevelItem(i)]);
        } else if(_type == Tano::M3UUdpxy) {
            generateItemUdpxy(_map[_treeWidget->topLevelItem(i)]);
        } else {
            generateItemNormal(_map[_treeWidget->topLevelItem(i)]);
        }
    }
    return true;
}

void M3UGenerator::generateItemNormal(Channel *channel)
{
    _out << "#EXTINF:"
         << channel->numberString() << ","
         << channel->name() << "\n";

    _out << "#EXTTV:"
         << channel->categories().join(",") << ";"
         << channel->language() << ";"
         << channel->epg();
    _out << "\n";

    if(!channel->logo().isEmpty()) {
        _out << "#EXTLOGO:"
             << channel->logo();
        _out << "\n";
    }

    _out << channel->url();
    _out << "\n\n";
}

void M3UGenerator::generateItemClean(Channel *channel)
{
    _out << "#EXTINF:"
         << channel->numberString() << ","
         << channel->name() << "\n";

    _out << channel->url();
    _out << "\n\n";
}

void M3UGenerator::generateItemUdpxy(Channel *channel)
{
    _out << "#EXTINF:"
         << channel->numberString() << ","
         << channel->name() << "\n";

    _out << "#EXTTV:"
         << channel->categories().join(",") << ";"
         << channel->language() << ";"
         << channel->epg();
    _out << "\n";


    if(!channel->logo().isEmpty()) {
        _out << "#EXTLOGO:"
             << channel->logo();
        _out << "\n";
    }

    _out << _udpxy->processUrl(channel->url());
    _out << "\n\n";
}
