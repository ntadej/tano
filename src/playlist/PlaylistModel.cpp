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

#include "container/core/Channel.h"
#include "playlist/PlaylistOpen.h"
#include "playlist/PlaylistSave.h"

#include "PlaylistModel.h"

PlaylistModel::PlaylistModel(QObject *parent)
    : ListModel(new Channel, parent)
{
    _name = QObject::tr("Channel list");

    _open = new PlaylistOpen();
    _save = new PlaylistSave(this);
}

PlaylistModel::~PlaylistModel()
{
    delete _open;
    delete _save;
}

Channel *PlaylistModel::find(const QString &id) const
{
    return qobject_cast<Channel *>(ListModel::find(id));
}

Channel *PlaylistModel::row(const int &row)
{
    return qobject_cast<Channel *>(ListModel::row(row));
}

Channel *PlaylistModel::takeRow(const int &row)
{
    return qobject_cast<Channel *>(ListModel::takeRow(row));
}

Channel *PlaylistModel::createChannel(const QString &name,
                                      const QString &url)
{
    int tmpNum, previous = 1;
    for(int i = 1; i < 1000; i++) {
        if(!_channelNumbers.contains(i)) {
            tmpNum = i;
            if(i != 1)
                previous = i-1;
            break;
        }
    }

    QString cname;
    if(name.isEmpty())
        cname = QObject::tr("New channel");
    else
        cname = name;

    Channel *channel = new Channel(cname, tmpNum);
    if(!url.isEmpty())
        channel->setUrl(url);

    _numbers.insert(channel->number(), channel);
    _channelNumbers << channel->number();

    insertRow(indexFromItem(_numbers[previous]).row()+1, channel);
    return channel;
}

Channel *PlaylistModel::deleteChannel(Channel *channel)
{
    _channelNumbers.removeAll(channel->number());

    _numbers.remove(channel->number());

    int r = indexFromItem(channel).row();
    removeRow(r);

    return row(r);
}

void PlaylistModel::exportTvheadend(const QString &location,
                                    const QString &interface)
{
    _save->saveTvheadend(location, interface);
}

void PlaylistModel::moveDown(Channel *channel)
{
    if(_channelNumbers.contains(channel->number()+1) && channel->number()+1 < 1000) {
        if(moveRow(indexFromItem(channel).row()+1, indexFromItem(channel).row())) {
            _numbers.remove(channel->number());
            _numbers[channel->number()+1]->setNumber(channel->number());
            _numbers.insert(channel->number(), _numbers[channel->number()+1]);
            _numbers.remove(channel->number()+1);
            channel->setNumber(channel->number()+1);
            _numbers.insert(channel->number(), channel);
        }
    } else if(channel->number()+1 < 1000) {
        _channelNumbers.removeAll(channel->number());
        _numbers.remove(channel->number());
        channel->setNumber(channel->number()+1);
        _numbers.insert(channel->number(), channel);
        _channelNumbers << channel->number();
    }
}

void PlaylistModel::moveUp(Channel *channel)
{
    if(_channelNumbers.contains(channel->number()-1) && channel->number()-1 > 0) {
        if(moveRow(indexFromItem(channel).row(), indexFromItem(channel).row()-1)) {
            _numbers.remove(channel->number());
            _numbers[channel->number()-1]->setNumber(channel->number());
            _numbers.insert(channel->number(), _numbers[channel->number()-1]);
            _numbers.remove(channel->number()-1);
            channel->setNumber(channel->number()-1);
            _numbers.insert(channel->number(), channel);
        }
    } else if(channel->number()-1 > 0) {
        _channelNumbers.removeAll(channel->number());
        _numbers.remove(channel->number());
        channel->setNumber(channel->number()-1);
        _numbers.insert(channel->number(), channel);
        _channelNumbers << channel->number();
    }
}

Channel *PlaylistModel::number(const int &number)
{
    for(int i = 0; i < rowCount(); i++) {
        if(row(i)->number() == number) {
            return row(i);
        }
    }

    return new Channel("error");
}

void PlaylistModel::open(const QString &file,
                         const bool &refresh,
                         const Tano::FileType &type,
                         const CSVInfo &info)
{
    if(!refresh)
        clear();

    _fileName = file;

    QList<Channel *> channels;
    switch (type)
    {
    case Tano::CSV:
        _open->openCSVFile(file, info);
        break;
    case Tano::JS:
        _open->openJsFile(file);
        break;
    case Tano::M3U:
        _open->openM3UFile(file);
        break;
    case Tano::TanoOld:
        _open->openTanoOldFile(file);
        break;
    default:
        break;
    }

    channels = _open->list();
    _name = _open->name();

    for(int i = 0; i < channels.size(); i++) {
        processChannel(channels[i]);
    }
}

void PlaylistModel::processChannel(Channel *channel)
{
    bool exists = find(channel->id());
    if(!exists) {
        for(int i = 0; i < rowCount(); i++) {
            if(row(i)->name() == channel->name()) {
                row(i)->setUrl(channel->url());
                return;
            }
        }
    } else {
        return;
    }

    if(channel->number() == 0) {
        channel->setNumber(1);
    }

    if(_channelNumbers.contains(channel->number())) {
        for(int i = channel->number(); i < 1000; i++) {
            if(!_channelNumbers.contains(i)) {
                channel->setNumber(i);
                break;
            }
        }
    }
    _channelNumbers << channel->number();

    for(int i = 0; i < channel->categories().size(); i++) {
        if(!_categoryList.contains(channel->categories()[i]))
            _categoryList << channel->categories()[i];
    }
    if(!_languageList.contains(channel->language()))
        _languageList << channel->language();
    if(!_epgList.contains(channel->epg()) && !channel->epg().isEmpty())
        _epgList << channel->epg();

    _numbers.insert(channel->number(), channel);
    appendRow(channel);
}

bool PlaylistModel::processNumber(Channel *channel,
                                  const int &number)
{
    if(_channelNumbers.contains(number))
        return false;

    _channelNumbers.removeAll(channel->number());
    _channelNumbers.append(number);

    _numbers.remove(channel->number());
    channel->setNumber(number);
    _numbers.insert(channel->number(), channel);

    takeRow(indexFromItem(channel).row());
    for(int i = channel->number()-1; i > 0; i--) {
        if(_channelNumbers.contains(i)) {
            insertRow(indexFromItem(_numbers[i]).row()+1, channel);
            break;
        }
    }

    return true;
}

void PlaylistModel::save(const QString &file,
                         const QString &name,
                         const Tano::FileType &type)
{
    _name = name;

    switch (type)
    {
    case Tano::CSV:
        _save->saveCSVFile(file);
        break;
    case Tano::JS:
        _save->saveJsFile(file);
        break;
    case Tano::M3U:
        _save->saveM3UFile(file);
        break;
    case Tano::XmltvId:
        _save->saveXmltvId(file);
        break;
    default:
        break;
    }
}

bool PlaylistModel::validate() const
{
    for(int i = 1; i < _channelNumbers.size(); i++) {
        if(_channelNumbers[i-1] == _channelNumbers[i]) {
            return false;
        }
    }

    return true;
}
