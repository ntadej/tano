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

#include <QtCore/QDebug>
#include <QtGui/QStyledItemDelegate>

#include "MobilePlaylistHandler.h"

#include "core/Common.h"
#include "playlist/PlaylistFilterModel.h"
#include "playlist/PlaylistModel.h"

MobilePlaylistHandler::MobilePlaylistHandler(QObject *parent)
    : QObject(parent)
{
    _model = new PlaylistModel(this);
    _filterModel = new PlaylistFilterModel(this);
    _filterModel->setDynamicSortFilter(true);
    _filterModel->setSourceModel(_model);
    openPlaylist();
}

MobilePlaylistHandler::~MobilePlaylistHandler()
{
    delete _model;
    delete _filterModel;
}

QVariantList MobilePlaylistHandler::categories()
{
    QVariantList list;
    for(int i = 0; i < _model->categories().size(); i++) {
        if(!_model->categories()[i].isEmpty())
            list << _model->categories()[i];
    }
    return list;
}

QVariantList MobilePlaylistHandler::languages()
{
    QVariantList list;
    for(int i = 0; i < _model->languages().size(); i++) {
        if(!_model->languages()[i].isEmpty())
            list << _model->languages()[i];
    }
    return list;
}

void MobilePlaylistHandler::openPlaylist()
{
    _model->openM3UFile(Tano::locateResource("playlists/sl/siol-mpeg4.m3u"));
}

void MobilePlaylistHandler::processGroups(const QString &category,
                                          const QString &language)
{
    _filterModel->setCategory(category);
    _filterModel->setLanguage(language);

    qDebug() << category << language;
}

void MobilePlaylistHandler::processSearch(const QString &name)
{
    QRegExp regExp(name, Qt::CaseInsensitive);
    _filterModel->setFilterRegExp(regExp);
}
