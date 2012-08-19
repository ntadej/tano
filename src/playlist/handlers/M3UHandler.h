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

#ifndef TANO_M3UHANDLER_H_
#define TANO_M3UHANDLER_H_

#include <QtCore/QString>
#include <QtCore/QStringList>

class Channel;

class M3UHandler
{
public:
    M3UHandler();
    ~M3UHandler();

    void processFile(const QString &m3uFile);

    QString name() const { return _name; }
    QList<Channel *> channelList() const { return _channels; }

private:
    void processList();

    Channel *_channel;

    QString _name;
    QString _hd;
    QString _radio;
    QStringList _m3uLineList;

    QList<Channel *> _channels;
};

#endif // TANO_M3UHANDLER_H_
