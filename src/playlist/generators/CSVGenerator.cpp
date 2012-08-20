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
#include "playlist/PlaylistModel.h"
#include "CSVGenerator.h"

CSVGenerator::CSVGenerator(const QString &file)
{
    _file = new QFile(file);

    QScopedPointer<Settings> settings(new Settings());
    _hd = settings->hdCategory();
    _radio = settings->radioCategory();
}

CSVGenerator::~CSVGenerator()
{
    delete _file;
}

bool CSVGenerator::write(PlaylistModel *model)
{
    if (!_file->open(QFile::WriteOnly | QFile::Text))
        return false;

    _out.setDevice(_file);
    _out.setCodec("UTF-8");
    _out << QObject::tr("Number") << ";"
         << QObject::tr("Channel") << ";"
         << QObject::tr("URL") << ";"
         << QObject::tr("Categories") << ";"
         << QObject::tr("Language") << ";"
         << QObject::tr("XMLTV ID") << "\n";

    for (int i = 0; i < model->rowCount(); i++) {
        generateItem(model->row(i));
    }
    return true;
}

void CSVGenerator::generateItem(Channel *channel)
{
    _out << QString::number(channel->number()) << ";"
         << channel->name() << ";"
         << channel->url() << ";";
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
    _out << channel->categories().join(",") << ";"
         << channel->language() << ";"
         << channel->xmltvId();
    _out << "\n";
}
