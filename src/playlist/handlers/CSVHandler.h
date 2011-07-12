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

#ifndef TANO_CSVHANDLER_H_
#define TANO_CSVHANDLER_H_

#include <QtCore/QString>
#include <QtCore/QStringList>

class Channel;

class CSVHandler
{
public:
    CSVHandler();
    ~CSVHandler();

    void processFile(const QString &csvFile);
    void setParameters(const QString &separator,
                       const bool &header,
                       const QList<int> &columns);

    QList<Channel *> channelList() const { return _channels; }

private:
    void processChannel(const QStringList &list);
    void processList();

    Channel *_channel;
    QList<Channel *> _channels;
    QStringList _csvLineList;

    // Parameters
    QString _separator;
    bool _header;

    int _number;
    int _name;
    int _url;
    int _categories;
    int _language;
    int _epg;
};

#endif // TANO_CSVHANDLER_H_
