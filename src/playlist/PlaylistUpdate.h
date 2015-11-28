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

#ifndef TANO_PLAYLISTUPDATE_H_
#define TANO_PLAYLISTUPDATE_H_

#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "common/File.h"

class NetworkDownload;
class PlaylistModel;

/*!
    \class PlaylistUpdate PlaylistUpdate.h core/playlist/PlaylistUpdate.h
    \brief Update playlist from network

    A simple class to download and update playlist from network
*/
class PlaylistUpdate : public QObject
{
Q_OBJECT
public:
    /*!
        \brief PlaylistUpdate constructor
        \param model playlist model to use (PlaylistModel *)
    */
    explicit PlaylistUpdate(PlaylistModel *model);
    ~PlaylistUpdate();

    /*!
        \brief Update the playlist
        \param playlist current playlist location (QString)
    */
    void update(const QString &playlist);

signals:
    /*!
        \brief Loading finished
    */
    void done();

private slots:
    void processPlaylist(QFile *file);

private:
    File::Type _type;
    PlaylistModel *_model;
    QString _playlist;
    bool _save;

    NetworkDownload *_downloader;
};

#endif // TANO_PLAYLISTUPDATE_H_
