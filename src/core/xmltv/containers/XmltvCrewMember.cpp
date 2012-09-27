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

#include "XmltvCrewMember.h"

XmltvCrewMember::XmltvCrewMember(QObject *parent)
    : ListItem(parent) { }

XmltvCrewMember::XmltvCrewMember(const QString &name,
                                 const Type &type,
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
    return QString("%1 (%2)").arg(name(), stringFromType(type()));
}

QIcon XmltvCrewMember::displayIcon() const
{
    return QIcon::fromTheme("config-users");
}

XmltvCrewMember::Type XmltvCrewMember::typeFromString(const QString &type)
{
    if (type == tr("Director"))
        return XmltvCrewMember::Director;
    else if (type == tr("Actor"))
        return XmltvCrewMember::Actor;
    else if (type == tr("Writer"))
        return XmltvCrewMember::Writer;
    else if (type == tr("Adapter"))
        return XmltvCrewMember::Adapter;
    else if (type == tr("Producer"))
        return XmltvCrewMember::Producer;
    else if (type == tr("Composer"))
        return XmltvCrewMember::Composer;
    else if (type == tr("Editor"))
        return XmltvCrewMember::Editor;
    else if (type == tr("Presenter"))
        return XmltvCrewMember::Presenter;
    else if (type == tr("Commentator"))
        return XmltvCrewMember::Commentator;
    else if (type == tr("Guest"))
        return XmltvCrewMember::Guest;
    else
        return XmltvCrewMember::All;
}

QString XmltvCrewMember::stringFromType(const Type &type)
{
    switch (type)
    {
    case XmltvCrewMember::Director:
        return tr("Director");
    case XmltvCrewMember::Actor:
        return tr("Actor");
    case XmltvCrewMember::Writer:
        return tr("Writer");
    case XmltvCrewMember::Adapter:
        return tr("Adapter");
    case XmltvCrewMember::Producer:
        return tr("Producer");
    case XmltvCrewMember::Composer:
        return tr("Composer");
    case XmltvCrewMember::Editor:
        return tr("Editor");
    case XmltvCrewMember::Presenter:
        return tr("Presenter");
    case XmltvCrewMember::Commentator:
        return tr("Commentator");
    case XmltvCrewMember::Guest:
        return tr("Guest");
    default:
        return QString();
    }
}

QString XmltvCrewMember::stringFromTypeOutput(const Type &type)
{
    switch (type)
    {
    case XmltvCrewMember::Director:
        return QString("director");
    case XmltvCrewMember::Actor:
        return QString("actor");
    case XmltvCrewMember::Writer:
        return QString("writer");
    case XmltvCrewMember::Adapter:
        return QString("adapter");
    case XmltvCrewMember::Producer:
        return QString("producer");
    case XmltvCrewMember::Composer:
        return QString("composer");
    case XmltvCrewMember::Editor:
        return QString("editor");
    case XmltvCrewMember::Presenter:
        return QString("presenter");
    case XmltvCrewMember::Commentator:
        return QString("commentator");
    case XmltvCrewMember::Guest:
        return QString("guest");
    default:
        return QString();
    }
}
