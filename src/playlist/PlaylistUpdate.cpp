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

#include "core/GetFile.h"
#include "core/Settings.h"
#include "playlist/PlaylistModel.h"
#include "playlist/PlaylistUpdate.h"

PlaylistUpdate::PlaylistUpdate(PlaylistModel *model)
    : QObject(model),
      _model(model) { }

PlaylistUpdate::~PlaylistUpdate()
{
    if (_downloader)
        delete _downloader;
}

void PlaylistUpdate::processPlaylist(const QString &playlist)
{
    disconnect(_downloader, SIGNAL(file(QString)), this, SLOT(processPlaylist(QString)));

    _model->open(playlist, true);
    _model->save(_playlist, _model->name());
}

void PlaylistUpdate::update(const QString &playlist)
{
    _playlist = playlist;
    _model->open(_playlist);

    QScopedPointer<Settings> settings(new Settings(this));
    if (settings->playlistUpdate()) {
        _downloader = new GetFile(this);
        connect(_downloader, SIGNAL(file(QString)), this, SLOT(processPlaylist(QString)));
        _downloader->getFile(settings->playlistUpdateUrl());
    }
}
