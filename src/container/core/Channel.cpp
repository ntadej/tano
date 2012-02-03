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

Channel::Channel(const QString &name,
                 const int &number,
                 QObject *parent)
    : ListItem(parent),
      _name(name),
      _number(number)
{
    _radio = false;
    _language = "";
    _url = "";
    _epg = "";
    _categories = QStringList();
    _logo = "";
}

Channel::~Channel() { }

QHash<int, QByteArray> Channel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DisplayIconRole] = "displayIcon";
    names[NameRole] = "name";
    names[RadioRole] = "radio";
    names[NumberRole] = "number";
    names[LanguageRole] = "language";
    names[UrlRole] = "url";
    names[EpgRole] = "epg";
    names[CategoriesRole] = "categories";
    names[LogoRole] = "logo";
    return names;
}

QVariant Channel::data(int role) const
{
    switch (role)
    {
    case DisplayRole:
        return display();
    case DisplayIconRole:
        return displayIcon();
    case NameRole:
        return name();
    case NumberRole:
        return number();
    case RadioRole:
        return radio();
    case LanguageRole:
        return language();
    case UrlRole:
        return url();
    case EpgRole:
        return epg();
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
    return numberString() + ". " + name();
}

QIcon Channel::displayIcon() const
{
    if (radio())
        return QIcon(":/icons/16x16/audio.png");
    else
        return QIcon(":/icons/16x16/video.png");
}

void Channel::setNumber(const int &number)
{
    if (_number != number) {
        _number = number;
        emit dataChanged();
    }
}

void Channel::setRadio(const bool &radio)
{
    if (_radio != radio) {
        _radio = radio;
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

void Channel::setEpg(const QString &epg)
{
    if (_epg != epg) {
        _epg = epg;
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
