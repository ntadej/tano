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

#include <QtCore/QDebug>

#include "common/Resources.h"
#include "network/NetworkDownload.h"
#include "channels/models/ChannelsModel.h"

#include "ChannelsUpdate.h"

ChannelsUpdate::ChannelsUpdate(ChannelsModel *model)
    : QObject(model),
      _type(File::M3U),
      _model(model),
      _save(true),
      _downloader(0) { }

ChannelsUpdate::~ChannelsUpdate()
{
    if (_downloader)
        delete _downloader;
}

void ChannelsUpdate::processChannels(QFile *file)
{
    disconnect(_downloader, SIGNAL(file(QFile *)), this, SLOT(processChannels(QFile *)));

    _model->open(file->fileName(), true);

    if (_save)
        _model->save(_channelsList, _model->name());

    emit done();

    delete file;
}

void ChannelsUpdate::update(const QString &channelsList)
{
    if (channelsList.isEmpty()) {
        _save = false;
    } else {
        _channelsList = Tano::Resources::resource(channelsList);
        QFile f(_channelsList);
        if (!f.open(QFile::ReadOnly | QFile::Text)) {
            qWarning() << tr("Cannot read file %1:\n%2.")
                          .arg(_channelsList)
                          .arg(f.errorString());
            _save = false;
        }
        f.close();
    }

    if (_save)
        _model->open(_channelsList);

// Channels downloading feature is disabled for now
/*    if (settings->channelsListUpdate()) {
        _downloader = new NetworkDownload(this);
        connect(_downloader, SIGNAL(file(QFile *)), this, SLOT(processChannels(QFile *)));
        _downloader->getFile(settings->channelsListUpdateUrl());
    } else {*/

    emit done();
}
