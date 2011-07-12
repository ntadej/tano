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

#include "PlaylistSave.h"
#include "playlist/generators/CSVGenerator.h"
#include "playlist/generators/JsGenerator.h"
#include "playlist/generators/M3UGenerator.h"
#include "playlist/generators/TvheadendGenerator.h"
#include "playlist/generators/XmltvIdGenerator.h"

PlaylistSave::PlaylistSave(PlaylistModel *model)
    : _model(model) { }

PlaylistSave::~PlaylistSave() { }

void PlaylistSave::saveCSVFile(const QString &file)
{
    CSVGenerator *generator = new CSVGenerator(file);
    generator->write(_model);
    delete generator;
}

void PlaylistSave::saveJsFile(const QString &file)
{
    JsGenerator *generator = new JsGenerator(file);
    generator->write(_model);
    delete generator;
}

void PlaylistSave::saveM3UFile(const QString &file,
                               const Tano::FileType &type)
{
    M3UGenerator *generator = new M3UGenerator(file, type);
    generator->write(_model);
    delete generator;
}

void PlaylistSave::saveTvheadend(const QString &location,
                                 const QString &interface,
                                 const QString &xmltv)
{
    TvheadendGenerator *generator = new TvheadendGenerator(location, interface, xmltv);
    generator->write(_model);
    delete generator;
}

void PlaylistSave::saveXmltvId(const QString &file)
{
    XmltvIdGenerator *generator = new XmltvIdGenerator(file);
    generator->write(_model);
    delete generator;
}
