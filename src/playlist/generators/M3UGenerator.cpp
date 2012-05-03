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

#include "container/core/Channel.h"
#include "core/Settings.h"
#include "core/Udpxy.h"
#include "playlist/PlaylistModel.h"
#include "playlist/generators/M3UGenerator.h"

M3UGenerator::M3UGenerator(const QString &file,
                           const Tano::FileType &type)
    : _type(type)
{
    _file = new QFile(file);
    _udpxy = new Udpxy();

    Settings *settings = new Settings();
    _radio = settings->radioCategory();
    delete settings;
}

M3UGenerator::~M3UGenerator()
{
    delete _file;
    delete _udpxy;
}

bool M3UGenerator::write(PlaylistModel *model)
{
    if (!_file->open(QFile::WriteOnly | QFile::Text))
        return false;

    _out.setDevice(_file);
    _out.setCodec("UTF-8");
    _out << "#EXTM3U\n";
    if(_type != Tano::M3UClean) {
        _out << "#EXTNAME:"
             << model->name()
             << "\n\n";
    }
    for (int i = 0; i < model->rowCount(); i++) {
        switch (_type)
        {
        case Tano::M3U:
            generateItemNormal(model->row(i));
            break;
        case Tano::M3UClean:
            generateItemClean(model->row(i));
            break;
        case Tano::M3UUdpxy:
            generateItemUdpxy(model->row(i));
            break;
        default:
            break;
        }
    }
    return true;
}

void M3UGenerator::generateItemNormal(Channel *channel)
{
    _out << "#EXTINF:"
         << channel->numberString() << ","
         << channel->name() << "\n";

    _out << "#EXTTV:";
    if (channel->radio()) {
        _out << _radio;

        if (!channel->categories().isEmpty())
            _out << ",";
    }
    _out << channel->categories().join(",") << ";";
    _out << channel->language() << ";"
         << channel->xmltvId();
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

    _out << "#EXTTV:";
    if (channel->radio()) {
        _out << _radio;

        if (channel->categories().isEmpty())
            _out << ",";
    }
    _out << channel->categories().join(",") << ";";
    _out << channel->language() << ";"
         << channel->xmltvId();
    _out << "\n";


    if(!channel->logo().isEmpty()) {
        _out << "#EXTLOGO:"
             << channel->logo();
        _out << "\n";
    }

    _out << _udpxy->processUrl(channel->url());
    _out << "\n\n";
}
