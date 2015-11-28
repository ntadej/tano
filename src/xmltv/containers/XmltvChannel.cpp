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

#include <QtGui/QIcon>

#include "xmltv/containers/XmltvChannel.h"

XmltvChannel::XmltvChannel(QObject *parent)
    : ListItem(parent) { }

XmltvChannel::XmltvChannel(const QString &id,
                           QObject *parent)
    : ListItem(parent),
      _id(id) { }

XmltvChannel::~XmltvChannel() { }

QHash<int, QByteArray> XmltvChannel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DecorationRole] = "decoration";
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
    case DecorationRole:
        return decoration();
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

QPixmap XmltvChannel::decoration() const
{
    return QIcon::fromTheme("video-x-generic").pixmap(16);
}

void XmltvChannel::setDisplayName(const QString &s)
{
    if (_displayName != s) {
        _displayName = s;
        emit dataChanged();
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
