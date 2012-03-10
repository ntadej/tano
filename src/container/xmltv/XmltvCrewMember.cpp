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

#include "container/xmltv/XmltvCrewMember.h"

XmltvCrewMember::XmltvCrewMember(QObject *parent)
    : ListItem(parent) { }

XmltvCrewMember::XmltvCrewMember(const QString &name,
                                 const Tano::Xmltv::CrewMemberType &type,
                                 QObject *parent)
    : ListItem(parent),
      _name(name),
      _type(type) { }

XmltvCrewMember::~XmltvCrewMember() { }

QHash<int, QByteArray> XmltvCrewMember::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DisplayIconRole] = "displayIcon";
    names[NameRole] = "name";
    names[TypeRole] = "type";
    return names;
}

QVariant XmltvCrewMember::data(int role) const
{
    switch (role)
    {
    case DisplayRole:
        return display();
    case DisplayIconRole:
        return displayIcon();
    case NameRole:
        return name();
    case TypeRole:
        return type();
    default:
        return QVariant();
    }
}

QString XmltvCrewMember::display() const
{
    return QString("%1 (%2)").arg(name(), Tano::Xmltv::crewMemberType(type()));
}

QIcon XmltvCrewMember::displayIcon() const
{
    return QIcon(":/icons/16x16/person.png");
}
