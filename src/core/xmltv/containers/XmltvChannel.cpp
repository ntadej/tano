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

#include "xmltv/containers/XmltvChannel.h"
#include "xmltv/containers/XmltvProgramme.h"
#include "xmltv/models/XmltvProgrammeModel.h"

XmltvChannel::XmltvChannel(QObject *parent)
    : ListItem(parent),
      _programme(0) { }

XmltvChannel::XmltvChannel(const QString &id,
                           QObject *parent)
    : ListItem(parent),
      _id(id)
{
    _programme = new XmltvProgrammeModel(this);
}

XmltvChannel::~XmltvChannel()
{
    if (_programme)
        delete _programme;
}

QHash<int, QByteArray> XmltvChannel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DisplayIconRole] = "displayIcon";
    names[IdRole] = "id";
    names[DisplayNameRole] = "displayName";
    names[IconRole] = "icon";
    names[UrlRole] = "url";
    return names;
}

QVariant XmltvChannel::data(int role) const
{
    switch (role)
    {
    case DisplayRole:
        return display();
    case DisplayIconRole:
        return displayIcon();
    case IdRole:
        return id();
    case DisplayNameRole:
        return displayName();
    case IconRole:
        return icon();
    case UrlRole:
        return url();
    default:
        return QVariant();
    }
}

QString XmltvChannel::display() const
{
    return displayName();
}

QIcon XmltvChannel::displayIcon() const
{
    return QIcon::fromTheme("video-x-generic");
}

void XmltvChannel::addProgramme(XmltvProgramme *p)
{
    _programme->appendRow(p);
}

void XmltvChannel::setDisplayName(const QString &s)
{
    if (_displayName != s) {
        _displayName = s;
        emit dataChanged();

        for(int i = 0; i < _programme->rowCount(); i++) {
            _programme->row(i)->setChannelDisplayName(s);
        }
    }
}

void XmltvChannel::setIcon(const QString &s)
{
    if (_icon != s) {
        _icon = s;
        emit dataChanged();
    }
}

void XmltvChannel::setUrl(const QString &s)
{
    if (_url != s) {
        _url = s;
        emit dataChanged();
    }
}
