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

#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include "container/core/Channel.h"
#include "core/settings/Settings.h"
#include "playlist/handlers/CSVHandler.h"

CSVHandler::CSVHandler()
{
    QScopedPointer<Settings> settings(new Settings());
    _hd = settings->hdCategory();
    _radio = settings->radioCategory();
}

CSVHandler::~CSVHandler() { }

void CSVHandler::processFile(const QString &csvFile)
{
    QFile file(csvFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        _csvLineList << in.readLine();
    }

    processList();
}

void CSVHandler::processChannel(const QStringList &list)
{
    _channel = new Channel(list[_name], list[_number].toInt());
    _channel->setUrl(list[_url]);
    _channel->setLanguage(list[_language]);
    _channel->setXmltvId(list[_xmltvId]);

    QStringList categories = list[_categories].split(",");
    if (categories.contains(_radio)) {
        categories.removeAll(_radio);
        _channel->setType(Channel::Radio);
    } else if (categories.contains(_hd)) {
        categories.removeAll(_hd);
        _channel->setType(Channel::HD);
    } else {
        _channel->setType(Channel::SD);
    }
    _channel->setCategories(categories);

    _channels << _channel;
}

void CSVHandler::processList()
{
    foreach (const QString &line, _csvLineList) {
        if (!(_header && (line == _csvLineList.first())))
            processChannel(line.split(_separator));
    }
}

void CSVHandler::setParameters(const QString &separator,
                               const bool &header,
                               const QList<int> &columns)
{
    _separator = separator;
    _header = header;

    _number = columns[0];
    _name = columns[1];
    _url = columns[2];
    _categories = columns[3];
    _language = columns[4];
    _xmltvId = columns[5];
}
