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

#ifndef TANO_PLAYLISTUPDATE_H_
#define TANO_PLAYLISTUPDATE_H_

#include <QtCore/QObject>
#include <QtCore/QString>

class GetFile;
class PlaylistModel;

class PlaylistUpdate : public QObject
{
Q_OBJECT
public:
    explicit PlaylistUpdate(PlaylistModel *model);
    ~PlaylistUpdate();

    void update(const QString &playlist);

private slots:
    void processPlaylist(const QString &playlist);

private:
    PlaylistModel *_model;
    QString _playlist;

    GetFile *_downloader;
};

#endif // TANO_PLAYLISTUPDATE_H_
