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

#ifndef TANO_MOBILEPLAYLISTHANDLER_H_
#define TANO_MOBILEPLAYLISTHANDLER_H_

#include <QtCore/QObject>
#include <QtCore/QVariantList>

class Channel;
class PlaylistFilterModel;
class PlaylistModel;

class MobilePlaylistHandler : public QObject
{
Q_OBJECT
public:
    MobilePlaylistHandler(QObject *parent = 0);
    ~MobilePlaylistHandler();

    PlaylistFilterModel *model() { return _filterModel; }
    Q_INVOKABLE QVariantList categories();
    Q_INVOKABLE QVariantList languages();

    Q_INVOKABLE void processGroups(const QString &category,
                                   const QString &language);
    Q_INVOKABLE void processSearch(const QString &name);

private:
    void openPlaylist();

    PlaylistModel *_model;
    PlaylistFilterModel *_filterModel;
};

#endif // TANO_MOBILEPLAYLISTHANDLER_H_
