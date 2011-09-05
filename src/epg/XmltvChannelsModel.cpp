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

#include "container/xmltv/XmltvChannel.h"
#include "epg/XmltvChannelsModel.h"

XmltvChannelsModel::XmltvChannelsModel(QObject *parent)
    : ListModel(new XmltvChannel, parent) { }

XmltvChannelsModel::~XmltvChannelsModel() { }

XmltvChannel *XmltvChannelsModel::find(const QString &id) const
{
    return qobject_cast<XmltvChannel *>(ListModel::find(id));
}

XmltvChannel *XmltvChannelsModel::row(const int &row)
{
    return qobject_cast<XmltvChannel *>(ListModel::row(row));
}

XmltvChannel *XmltvChannelsModel::takeRow(const int &row)
{
    return qobject_cast<XmltvChannel *>(ListModel::takeRow(row));
}
