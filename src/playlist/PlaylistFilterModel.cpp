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

#include "container/core/Channel.h"

#include "PlaylistFilterModel.h"

PlaylistFilterModel::PlaylistFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent) { }

PlaylistFilterModel::~PlaylistFilterModel() { }

void PlaylistFilterModel::setCategory(const QString &category)
{
    if(category != tr("All categories"))
        _category = category;
    else
        _category = "";
    invalidateFilter();
}

void PlaylistFilterModel::setLanguage(const QString &language)
{
    if(language != tr("All languages"))
        _language = language;
    else
        _language = "";
    invalidateFilter();
}

bool PlaylistFilterModel::filterAcceptsRow(int sourceRow,
                                           const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    bool name = sourceModel()->data(index, Channel::NameRole).toString().contains(filterRegExp());
    bool category = sourceModel()->data(index, Channel::CategoriesRole).toStringList().join("|").contains(_category, Qt::CaseInsensitive);
    bool language = sourceModel()->data(index, Channel::LanguageRole).toString().contains(_language, Qt::CaseInsensitive);

    return (name && category && language);
}
