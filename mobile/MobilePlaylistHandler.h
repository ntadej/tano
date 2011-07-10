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
class ListItem;
class ListModel;
class PlaylistWidget;

class MobilePlaylistHandler : public QObject
{
Q_OBJECT
public:
    MobilePlaylistHandler(QObject *parent = 0);
    ~MobilePlaylistHandler();

    ListModel *model() { return _model; }
    QVariantList categories();
    QVariantList languages();

private:
    void openPlaylist();
    void processPlaylist();

    QList<Channel *> _channels;
    QList<ListItem *> _items;

    ListModel *_model;
    PlaylistWidget *_playlist;
};

#endif // TANO_MOBILEPLAYLISTHANDLER_H_
