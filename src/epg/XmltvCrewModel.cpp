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

#include "container/xmltv/XmltvCrewMember.h"
#include "epg/XmltvCrewModel.h"

XmltvCrewModel::XmltvCrewModel(QObject *parent)
    : ListModel(new XmltvCrewMember, parent)
{

}

XmltvCrewModel::~XmltvCrewModel()
{

}

XmltvCrewMember *XmltvCrewModel::find(const QString &id) const
{
    return qobject_cast<XmltvCrewMember *>(ListModel::find(id));
}

XmltvCrewMember *XmltvCrewModel::row(const int &row)
{
    return qobject_cast<XmltvCrewMember *>(ListModel::row(row));
}

XmltvCrewMember *XmltvCrewModel::takeRow(const int &row)
{
    return qobject_cast<XmltvCrewMember *>(ListModel::takeRow(row));
}
