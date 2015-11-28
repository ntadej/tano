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

#include "PlaylistModel.h"
#include "PlaylistFilterModel.h"

PlaylistFilterModel::PlaylistFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    _types << Channel::SD
           << Channel::HD
           << Channel::Radio;
}

PlaylistFilterModel::~PlaylistFilterModel() { }

QString PlaylistFilterModel::name() const
{
    PlaylistModel *model = qobject_cast<PlaylistModel *>(sourceModel());
    if (model)
        return model->name();
    else
        return tr("Playlist");
}

void PlaylistFilterModel::setCategory(const QString &category)
{
    if(!category.isEmpty() || category != tr("All categories"))
        _category = category;
    else
        _category = "";
    invalidateFilter();
}

void PlaylistFilterModel::setLanguage(const QString &language)
{
    if(!language.isEmpty() || language != tr("All languages"))
        _language = language;
    else
        _language = "";
    invalidateFilter();
}

void PlaylistFilterModel::setTypes(const QList<Channel::Type> &types)
{
    _types = types;
    invalidateFilter();
}

bool PlaylistFilterModel::filterAcceptsRow(int sourceRow,
                                           const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    bool name = sourceModel()->data(index, Channel::NameRole).toString().contains(filterRegExp());
    bool category = sourceModel()->data(index, Channel::CategoriesRole).toStringList().join("|").contains(_category, Qt::CaseInsensitive);
    bool language = sourceModel()->data(index, Channel::LanguageRole).toString().contains(_language, Qt::CaseInsensitive);
    bool type = _types.contains(Channel::Type(sourceModel()->data(index, Channel::TypeRole).toInt()));

    return name && category && language && type;
}
