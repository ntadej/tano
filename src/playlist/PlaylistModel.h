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

#ifndef TANO_PLAYLISTMODEL_H_
#define TANO_PLAYLISTMODEL_H_

#include <QtCore/QStringList>

#include "core/ListModel.h"

class Channel;

class PlaylistModel : public ListModel
{
Q_OBJECT
public:
    PlaylistModel(QObject *parent = 0);
    ~PlaylistModel();

    Channel *find(const QString &id) const;
    Channel *row(const int &row);
    Channel *takeRow(const int &row);

    QStringList categories() const { return _categoryList; }
    QStringList epg() const { return _epgList; }
    QStringList languages() const { return _languageList; }
    QString name() const { return _name; }
    QList<int> numbers() const { return _channelNumbers; }

    void openM3UFile(const QString &file);

    bool validate() const;

private:
    void processChannel(Channel *channel);

    QList<int> _channelNumbers;
    QStringList _categoryList;
    QStringList _epgList;
    QStringList _languageList;
    QMap<int, Channel *> _numbers;

    QString _name;
};

#endif // TANO_PLAYLISTMODEL_H_
