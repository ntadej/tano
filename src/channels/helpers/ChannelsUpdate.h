/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_CHANNELSUPDATE_H_
#define TANO_CHANNELSUPDATE_H_

#include <QtCore/QFile>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "common/File.h"

class NetworkDownload;
class ChannelsModel;

class ChannelsUpdate : public QObject
{
    Q_OBJECT
public:
    explicit ChannelsUpdate(ChannelsModel *model);
    ~ChannelsUpdate();

    void update(const QString &channelsList);

signals:
    void done();

private slots:
    void processChannels(QFile *file);

private:
    File::Type _type;
    ChannelsModel *_model;
    QString _channelsList;
    bool _save;

    NetworkDownload *_downloader;
};

#endif // TANO_CHANNELSUPDATE_H_
