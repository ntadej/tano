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

#include "channels/generators/CSVGenerator.h"
#include "channels/generators/M3UGenerator.h"
#include "channels/generators/XmltvIdGenerator.h"
#include "channels/helpers/ChannelsSave.h"

ChannelsSave::ChannelsSave(ChannelsModel *model)
    : _model(model) { }

ChannelsSave::~ChannelsSave() { }

void ChannelsSave::saveCSVFile(const QString &file)
{
    QScopedPointer<CSVGenerator> generator(new CSVGenerator(file));
    generator->write(_model);
}

void ChannelsSave::saveM3UFile(const QString &file,
                               const File::Type &type)
{
    QScopedPointer<M3UGenerator> generator(new M3UGenerator(file, type));
    generator->write(_model);
}

void ChannelsSave::saveXmltvId(const QString &file)
{
    QScopedPointer<XmltvIdGenerator> generator(new XmltvIdGenerator(file));
    generator->write(_model);
}
