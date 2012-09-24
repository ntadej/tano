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

#ifndef TANO_PLAYLISTSAVE_H_
#define TANO_PLAYLISTSAVE_H_

#include <QtCore/QString>

#include "File.h"

class PlaylistModel;

class PlaylistSave
{
public:
    PlaylistSave(PlaylistModel *model);
    ~PlaylistSave();

    void saveCSVFile(const QString &file);
    void saveJsFile(const QString &file);
    void saveM3UFile(const QString &file,
                     const File::Type &type = File::M3U);
    void saveTvheadend(const QString &location,
                       const QString &interface);
    void saveXmltvId(const QString &file);

private:
    PlaylistModel *_model;
};

#endif // TANO_PLAYLISTSAVE_H_
