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

#ifndef TANO_PLAYLISTOPEN_H_
#define TANO_PLAYLISTOPEN_H_

#include <QtCore/QList>
#include <QtCore/QString>

class Channel;
struct CSVInfo;

class PlaylistOpen
{
public:
    PlaylistOpen();
    ~PlaylistOpen();

    inline QString name() const { return _name; }
    inline QList<Channel *> list() const { return _list; }
    void openCSVFile(const QString &file,
                     const CSVInfo &info);
    void openM3UFile(const QString &file);

private:
    QList<Channel *> _list;
    QString _name;
};

#endif // PLAYLISTOPEN_H
