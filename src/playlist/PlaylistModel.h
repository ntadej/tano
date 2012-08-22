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

#ifndef TANO_PLAYLISTMODEL_H_
#define TANO_PLAYLISTMODEL_H_

#include <QtCore/QStringList>

#include "container/ListModel.h"
#include "core/File.h"
#include "playlist/handlers/CSVHandler.h"

class Channel;
class PlaylistOpen;
class PlaylistSave;

class PlaylistModel : public ListModel
{
Q_OBJECT
public:
    PlaylistModel(QObject *parent = 0);
    ~PlaylistModel();

    Channel *find(const QString &id) const;
    Channel *row(const int &row);
    Channel *takeRow(const int &row);

    Channel *createChannel(const QString &name = 0,
                           const QString &url = 0);
    Channel *deleteChannel(Channel *channel);

    Channel *number(const int &number);
    Channel *xmltvId(const QString &xmltvId);

    void moveDown(Channel *channel);
    void moveUp(Channel *channel);
    bool processNumber(Channel *channel,
                       const int &number);

    QStringList categories() const { return _categoryList; }
    QStringList xmltvId() const { return _xmltvIdList; }
    QString fileName() const { return _fileName; }
    QStringList languages() const { return _languageList; }
    QString name() const { return _name; }
    QList<int> numbers() const { return _channelNumbers; }

    void clear();
    void open(const QString &file,
              const bool &refresh = false,
              const File::Type &type = File::M3U,
              const CSVInfo &info = CSVInfo());
    void save(const QString &file,
              const QString &name,
              const File::Type &type = File::M3U);
    void exportTvheadend(const QString &location,
                         const QString &interface);

    bool validate() const;

private:
    void processChannel(Channel *channel);

    PlaylistOpen *_open;
    PlaylistSave *_save;

    QList<int> _channelNumbers;
    QStringList _categoryList;
    QStringList _xmltvIdList;
    QStringList _languageList;
    QMap<int, Channel *> _numbers;

    QString _fileName;
    QString _name;
};

#endif // TANO_PLAYLISTMODEL_H_
