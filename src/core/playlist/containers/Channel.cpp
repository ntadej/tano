/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include "Channel.h"

Channel::Channel(QObject *parent)
    : ListItem(parent) { }

Channel::Channel(const QString &name,
                 const int &number,
                 QObject *parent)
    : ListItem(parent),
      _name(name),
      _number(number)
{
    _type = SD;
    _language = "";
    _url = "";
    _xmltvId = "";
    _categories = QStringList();
    _logo = "";
}

Channel::~Channel() { }

QHash<int, QByteArray> Channel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DecorationRole] = "decoration";
    names[NameRole] = "name";
    names[TypeRole] = "type";
    names[NumberRole] = "number";
    names[LanguageRole] = "language";
    names[UrlRole] = "url";
    names[XmltvIdRole] = "xmltvid";
    names[CategoriesRole] = "categories";
    names[LogoRole] = "logo";
    return names;
}

QVariant Channel::data(const int &role) const
{
    switch (role)
    {
    case DisplayRole:
        return display();
    case DecorationRole:
        return decoration();
    case NameRole:
        return name();
    case NumberRole:
        return number();
    case TypeRole:
        return type();
    case LanguageRole:
        return language();
    case UrlRole:
        return url();
    case XmltvIdRole:
        return xmltvId();
    case CategoriesRole:
        return categories();
    case LogoRole:
        return logo();
    default:
        return QVariant();
    }
}

QString Channel::display() const
{
    return QString("%1. %2").arg(QString::number(number()), name());
}

QIcon Channel::decoration() const
{
    if (type() == Radio)
        return QIcon::fromTheme("audio-x-generic");
    else if (type() == HD)
        return QIcon::fromTheme("video-x-generic");
    else // type() == SD
        return QIcon::fromTheme("video-x-generic");
}

void Channel::setNumber(const int &number)
{
    if (_number != number) {
        _number = number;
        emit dataChanged();
    }
}

void Channel::setType(const Type &type)
{
    if (_type != type) {
        _type = type;
        emit dataChanged();
    }
}

void Channel::setName(const QString &name)
{
    if (_name != name) {
        _name = name;
        emit dataChanged();
    }
}

void Channel::setLanguage(const QString &language)
{
    if (_language != language) {
        _language = language;
        emit dataChanged();
    }
}

void Channel::setUrl(const QString &url)
{
    if (_url != url) {
        _url = url;
        emit dataChanged();
    }
}

void Channel::setXmltvId(const QString &xmltvId)
{
    if (_xmltvId != xmltvId) {
        _xmltvId = xmltvId;
        emit dataChanged();
    }
}

void Channel::setCategories(const QStringList &categories)
{
    if (_categories != categories) {
        _categories = categories;
        emit dataChanged();
    }
}

void Channel::setLogo(const QString &logo)
{
    if (_logo != logo) {
        _logo = logo;
        emit dataChanged();
    }
}

QStringList Channel::types()
{
    QStringList list;
    list << tr("SD")
         << tr("HD")
         << tr("Radio");

    return list;
}
