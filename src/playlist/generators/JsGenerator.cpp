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
#include "core/settings/Settings.h"
#include "playlist/PlaylistModel.h"
#include "playlist/generators/JsGenerator.h"

JsGenerator::JsGenerator(const QString &file)
{
    _file = new QFile(file);

    QScopedPointer<Settings> settings(new Settings());
    _hd = settings->hdCategory();
    _radio = settings->radioCategory();
}

JsGenerator::~JsGenerator()
{
    delete _file;
}

bool JsGenerator::write(PlaylistModel *model)
{
    if (!_file->open(QFile::WriteOnly | QFile::Text))
        return false;

    _out.setDevice(_file);
    _out.setCodec("UTF-8");
    _out << "la=[";
    for (int i = 0; i < model->rowCount(); i++) {
        generateItem(i+1, model->row(i));
        if(i != model->rowCount()-1) {
            _out << ",";
        }
    }
    _out << "];";
    return true;
}

void JsGenerator::generateItem(const int &id,
                               Channel *channel)
{
    _out << "["
         << id << ","
         << "\"" << channel->name() << "\"" << ","
         << "\"" << QString::number(channel->number()) << "\"" << ","
         << "\"" << channel->url().replace(QRegExp("udp://@"), "").replace(QRegExp(":.*"), "") << "\"" << ","
         << "\"" << channel->url().replace(QRegExp("udp://@.*:"), "") << "\"" << ",";
    switch (channel->type())
    {
    case Channel::Radio:
        _out << "\"" << _radio << "\"" << ",";
        break;
    case Channel::HD:
        _out << "\"" << _hd << "\"" << ",";
        break;
    case Channel::SD:
    default:
        _out << "\"" << channel->categories()[0] << "\"" << ",";
        break;
    }
    _out << "\"" << channel->language() << "\"" << ","
         << "\"" << channel->xmltvId() << "\"" << ","
         << "\"" << "false" << "\""
         << "]";
}
