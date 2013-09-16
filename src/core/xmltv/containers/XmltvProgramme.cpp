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

#include "xmltv/XmltvCommon.h"
#include "xmltv/containers/XmltvCrewMember.h"
#include "xmltv/models/XmltvCrewModel.h"

#include "XmltvProgramme.h"

XmltvProgramme::XmltvProgramme(QObject *parent)
    : ListItem(parent),
      _crew(0) { }

XmltvProgramme::XmltvProgramme(const QString &channel,
                               QObject *parent)
    : ListItem(parent),
      _channel(channel),
      _crew(0) { }

XmltvProgramme::~XmltvProgramme()
{
    if (_crew)
        delete _crew;
}

QHash<int, QByteArray> XmltvProgramme::roleNames() const
{
    QHash<int, QByteArray> names;
    names[DisplayRole] = "display";
    names[DecorationRole] = "decoration";
    names[DisplayFontRole] = "displayFont";
    names[TitleRole] = "title";
    names[ChannelRole] = "channel";
    names[StartRole] = "start";
    names[StopRole] = "stop";
    names[SubTitleRole] = "subtitle";
    names[DescRole] = "desc";
    names[DateRole] = "date";
    names[CategoriesRole] = "categories";
    names[LanguageRole] = "language";
    names[OriginalLanguageRole] = "originallanguage";
    names[LengthRole] = "length";
    names[LengthUnitsRole] = "lengthunits";
    names[IconRole] = "icon";
    names[IconSizeRole] = "iconsize";
    return names;
}

QString XmltvProgramme::id() const
{
    return channel() + "_" + QString::number(start());
}

QVariant XmltvProgramme::data(int role) const
{
    switch (role)
    {
    case DisplayRole:
        return display();
    case DecorationRole:
        return decoration();
    case DisplayFontRole:
        return displayFont();
    case TitleRole:
        return title();
    case ChannelRole:
        return channelDisplayName();
    case StartRole:
        return start();
    case StopRole:
        return stop();
    case SubTitleRole:
        return subTitle();
    case DescRole:
        return desc();
    case DateRole:
        return date();
    case CategoriesRole:
        return categories();
    case LanguageRole:
        return language();
    case OriginalLanguageRole:
        return originalLanguage();
    case LengthRole:
        return length();
    case LengthUnitsRole:
        return lengthUnits();
    case IconRole:
        return icon();
    case IconSizeRole:
        return iconSize();
    default:
        return QVariant();
    }
}

QString XmltvProgramme::display() const
{
    return QString("%1 - %2").arg(QDateTime::fromTime_t(start()).toString(Tano::Xmltv::timeFormatDisplay()), title());
}

QPixmap XmltvProgramme::decoration() const
{
    return QIcon::fromTheme("x-office-calendar").pixmap(16);
}

QFont XmltvProgramme::displayFont() const
{
    QFont f;
    if (QDateTime::currentDateTime() > QDateTime::fromTime_t(start()) && QDateTime::currentDateTime() < QDateTime::fromTime_t(stop()))
        f.setBold(true);

    return f;
}

void XmltvProgramme::setChannel(const QString &s)
{
    if (_channel != s) {
        _channel = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setChannelDisplayName(const QString &s)
{
    if (_channelDisplayName != s) {
        _channelDisplayName = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setTitle(const QString &s)
{
    if (_title != s) {
        _title = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setStart(int i)
{
    if (_start != i) {
        _start = i;
        emit dataChanged();
    }
}

void XmltvProgramme::setStop(int i)
{
    if (_stop != i) {
        _stop = i;
        emit dataChanged();
    }
}

void XmltvProgramme::setSubTitle(const QString &s)
{
    if (_subTitle != s) {
        _subTitle = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setDesc(const QString &s)
{
    if (_desc != s) {
        _desc = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setDate(const QDateTime &d)
{
    if (_date != d) {
        _date = d;
        emit dataChanged();
    }
}

void XmltvProgramme::addCategory(const QString &s)
{
    QString cat;
    if (!Tano::Xmltv::mapCategory(s).isEmpty())
        cat = Tano::Xmltv::mapCategory(s);
    else
        cat = s;

    if (!_categories.contains(cat)) {
        _categories << cat;
        emit dataChanged();
    }
}

void XmltvProgramme::setCategories(const QStringList &s)
{
    if (_categories != s) {
        _categories = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setLanguage(const QString &s)
{
    if (_language != s) {
        _language = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setOriginalLanguage(const QString &s)
{
    if (_originalLanguage != s) {
        _originalLanguage = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setLength(const QString &s)
{
    if (_length != s) {
        _length = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setLengthUnits(const XmltvProgramme::LengthUnits &e)
{
    if (_lengthUnits != e) {
        _lengthUnits = e;
        emit dataChanged();
    }
}

void XmltvProgramme::setIcon(const QString &s)
{
    if (_icon != s) {
        _icon = s;
        emit dataChanged();
    }
}

void XmltvProgramme::setIconSize(const QSize &s)
{
    if (_iconSize != s) {
        _iconSize = s;
        emit dataChanged();
    }
}

XmltvProgramme::LengthUnits XmltvProgramme::lengthUnits(const QString &type)
{
    if (type == "seconds")
        return XmltvProgramme::Seconds;
    else if (type == "minutes")
        return XmltvProgramme::Minutes;
    else if (type == "hours")
        return XmltvProgramme::Hours;
    else
        return XmltvProgramme::Seconds;
}

QString XmltvProgramme::lengthUnits(const LengthUnits &type)
{
    switch (type)
    {
    case XmltvProgramme::Seconds:
        return QObject::tr("seconds");
    case XmltvProgramme::Minutes:
        return QObject::tr("minutes");
    case XmltvProgramme::Hours:
        return QObject::tr("hours");
    default:
        return QString();
    }
}

QString XmltvProgramme::lengthUnitsShort(const LengthUnits &type)
{
    if (type == XmltvProgramme::Seconds)
        return QObject::tr("s");
    else if (type == XmltvProgramme::Minutes)
        return QObject::tr("min");
    else if (type == XmltvProgramme::Hours)
        return QObject::tr("h");
    else
        return QString();
}
