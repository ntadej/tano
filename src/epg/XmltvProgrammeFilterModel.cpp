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

#include "container/xmltv/XmltvProgramme.h"
#include "epg/XmltvProgrammeFilterModel.h"

XmltvProgrammeFilterModel::XmltvProgrammeFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    _date = QDate::currentDate();
}

XmltvProgrammeFilterModel::~XmltvProgrammeFilterModel() { }

void XmltvProgrammeFilterModel::setDate(const QDate &date)
{
    _date = date;
    invalidateFilter();
}

bool XmltvProgrammeFilterModel::filterAcceptsRow(int sourceRow,
                                           const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    bool language = (sourceModel()->data(index, XmltvProgramme::StartRole).toDateTime().date() == _date);

    return language;
}
