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

#include "channels/containers/Channel.h"
#include "channels/helpers/ChannelsOpen.h"
#include "channels/helpers/ChannelsSave.h"

#include "ChannelsModel.h"

ChannelsModel::ChannelsModel(QObject *parent)
    : ListModel(new Channel, parent)
{
    _name = QObject::tr("Channels list");

    _open = new ChannelsOpen();
    _save = new ChannelsSave(this);
}

ChannelsModel::~ChannelsModel()
{
    delete _open;
    delete _save;
}

Channel *ChannelsModel::find(const QString &id) const
{
    return qobject_cast<Channel *>(ListModel::find(id));
}

Channel *ChannelsModel::row(int row)
{
    return qobject_cast<Channel *>(ListModel::row(row));
}

Channel *ChannelsModel::takeRow(int row)
{
    return qobject_cast<Channel *>(ListModel::takeRow(row));
}

void ChannelsModel::clear()
{
    _channelNumbers.clear();
    _categoryList.clear();
    _xmltvIdList.clear();
    _languageList.clear();
    _numbers.clear();

    ListModel::clear();
}

Channel *ChannelsModel::createChannel(const QString &name,
                                      const QString &url)
{
    int tmpNum = 1, previous = 0;
    for (int i = 1; i < 1000; i++) {
        if (!_channelNumbers.contains(i)) {
            tmpNum = i;
            if (i != 1)
                previous = i-1;
            break;
        }
    }

    QString cname;
    if (name.isEmpty())
        cname = QObject::tr("New channel");
    else
        cname = name;

    Channel *channel = new Channel(cname, tmpNum);
    if (!url.isEmpty())
        channel->setUrl(url);

    _numbers.insert(channel->number(), channel);
    _channelNumbers << channel->number();

    if (previous)
        insertRow(indexFromItem(_numbers[previous]).row()+1, channel);
    else
        appendRow(channel);

    return row(indexFromItem(channel).row());
}

Channel *ChannelsModel::deleteChannel(Channel *channel)
{
    _channelNumbers.removeAll(channel->number());

    _numbers.remove(channel->number());

    int r = indexFromItem(channel).row();
    removeRow(r);

    while (r >= rowCount())
        r--;

    if (r < 0)
        return 0;
    else
        return row(r);
}

void ChannelsModel::moveDown(Channel *channel)
{
    if (_channelNumbers.contains(channel->number()+1) && channel->number()+1 < 1000) {
        if (moveRow(indexFromItem(channel).row()+1, indexFromItem(channel).row())) {
            _numbers.remove(channel->number());
            _numbers[channel->number()+1]->setNumber(channel->number());
            _numbers.insert(channel->number(), _numbers[channel->number()+1]);
            _numbers.remove(channel->number()+1);
            channel->setNumber(channel->number()+1);
            _numbers.insert(channel->number(), channel);
        }
    } else if (channel->number()+1 < 1000) {
        _channelNumbers.removeAll(channel->number());
        _numbers.remove(channel->number());
        channel->setNumber(channel->number()+1);
        _numbers.insert(channel->number(), channel);
        _channelNumbers << channel->number();
    }
}

void ChannelsModel::moveUp(Channel *channel)
{
    if (_channelNumbers.contains(channel->number()-1) && channel->number()-1 > 0) {
        if (moveRow(indexFromItem(channel).row(), indexFromItem(channel).row()-1)) {
            _numbers.remove(channel->number());
            _numbers[channel->number()-1]->setNumber(channel->number());
            _numbers.insert(channel->number(), _numbers[channel->number()-1]);
            _numbers.remove(channel->number()-1);
            channel->setNumber(channel->number()-1);
            _numbers.insert(channel->number(), channel);
        }
    } else if (channel->number()-1 > 0) {
        _channelNumbers.removeAll(channel->number());
        _numbers.remove(channel->number());
        channel->setNumber(channel->number()-1);
        _numbers.insert(channel->number(), channel);
        _channelNumbers << channel->number();
    }
}

Channel *ChannelsModel::number(int number)
{
    for (int i = 0; i < rowCount(); i++) {
        if (row(i)->number() == number) {
            return row(i);
        }
    }

    return 0;
}

Channel *ChannelsModel::xmltvId(const QString &xmltvId)
{
    for (int i = 0; i < rowCount(); i++) {
        if (row(i)->xmltvId() == xmltvId) {
            return row(i);
        }
    }

    return 0;
}

void ChannelsModel::open(const QString &file,
                         bool refresh,
                         const File::Type &type,
                         const CSVInfo &info)
{
    if (!refresh)
        clear();

    _fileName = file;

    QList<Channel *> channels;
    switch (type)
    {
    case File::CSV:
        _open->openCSVFile(file, info);
        break;
    case File::M3U:
        _open->openM3UFile(file);
        break;
    default:
        break;
    }

    channels = _open->list();
    _name = _open->name();

    foreach (Channel *channel, channels) {
        processChannel(channel);
    }
}

void ChannelsModel::processChannel(Channel *channel)
{
    Channel *exists = find(channel->id());
    if (!exists) {
        for (int i = 0; i < rowCount(); i++) {
            if (row(i)->name() == channel->name()) {
                row(i)->setUrl(channel->url());
                return;
            }
        }
    } else {
        exists->setCategories(channel->categories());
        exists->setXmltvId(channel->xmltvId());
        exists->setLanguage(channel->language());
        exists->setLogo(channel->logo());
        exists->setName(channel->name());
        exists->setType(channel->type());
        exists->setUrl(channel->url());

        return;
    }

    if (channel->number() == 0) {
        channel->setNumber(1);
    }

    if (_channelNumbers.contains(channel->number())) {
        for (int i = channel->number(); i < 1000; i++) {
            if (!_channelNumbers.contains(i)) {
                channel->setNumber(i);
                break;
            }
        }
    }
    _channelNumbers << channel->number();

    foreach (const QString &category, channel->categories()) {
        if (!_categoryList.contains(category))
            _categoryList << category;
    }
    if (!_languageList.contains(channel->language()))
        _languageList << channel->language();
    if (!_xmltvIdList.contains(channel->xmltvId()) && !channel->xmltvId().isEmpty())
        _xmltvIdList << channel->xmltvId();

    _numbers.insert(channel->number(), channel);
    appendRow(channel);
}

bool ChannelsModel::processNumber(Channel *channel,
                                  int number)
{
    if (_channelNumbers.contains(number))
        return false;

    _channelNumbers.removeAll(channel->number());
    _channelNumbers.append(number);

    _numbers.remove(channel->number());
    channel->setNumber(number);
    _numbers.insert(channel->number(), channel);

    takeRow(indexFromItem(channel).row());
    for (int i = channel->number()-1; i > 0; i--) {
        if (_channelNumbers.contains(i)) {
            insertRow(indexFromItem(_numbers[i]).row()+1, channel);
            break;
        }
    }

    return true;
}

void ChannelsModel::save(const QString &file,
                         const QString &name,
                         const File::Type &type)
{
    _name = name;

    switch (type)
    {
    case File::CSV:
        _save->saveCSVFile(file);
        break;
    case File::M3U:
    case File::M3UClean:
    case File::M3UUdpxy:
        _save->saveM3UFile(file, type);
        break;
    case File::XmltvId:
        _save->saveXmltvId(file);
        break;
    default:
        break;
    }
}

bool ChannelsModel::validate() const
{
    for (int i = 1; i < _channelNumbers.size(); i++) {
        if (_channelNumbers[i-1] == _channelNumbers[i]) {
            return false;
        }
    }

    return true;
}
