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

#include "network/NetworkUdpxy.h"
#include "playlist/PlaylistModel.h"
#include "playlist/containers/Channel.h"
#include "playlist/generators/M3UGenerator.h"
#include "settings/Settings.h"

M3UGenerator::M3UGenerator(const QString &file,
                           const File::Type &type)
    : _type(type)
{
    _file = new QFile(file);
    _udpxy = new NetworkUdpxy();

    QScopedPointer<Settings> settings(new Settings());
    _hd = settings->hdCategory();
    _radio = settings->radioCategory();
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
    if(_type != File::M3UClean) {
        _out << "#EXTNAME:"
             << model->name()
             << "\n\n";
    }
    for (int i = 0; i < model->rowCount(); i++) {
        switch (_type)
        {
        case File::M3U:
            generateItem(model->row(i));
            break;
        case File::M3UClean:
            generateItem(model->row(i), true, false);
            break;
        case File::M3UUdpxy:
            generateItem(model->row(i), false, true);
            break;
        default:
            break;
        }
    }
    return true;
}

void M3UGenerator::generateItem(Channel *channel, const bool &clean, const bool &udpxy)
{
    _out << "#EXTINF:"
         << QString::number(channel->number()) << ","
         << channel->name() << "\n";

    if (!clean) {
        _out << "#EXTTV:";
        switch (channel->type())
        {
        case Channel::Radio:
            _out << _radio;
            if (!channel->categories().isEmpty())
                _out << ",";
            break;
        case Channel::HD:
            _out << _hd;
            if (!channel->categories().isEmpty())
                _out << ",";
            break;
        case Channel::SD:
        default:
            break;
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
    }

    if (udpxy) {
        _out << _udpxy->processUrl(channel->url());
    } else {
        _out << channel->url();
    }
    _out << "\n\n";
}
