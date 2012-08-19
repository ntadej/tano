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
#include "core/Settings.h"
#include "playlist/handlers/JsHandler.h"

JsHandler::JsHandler()
{
    QScopedPointer<Settings> settings(new Settings());
    _hd = settings->hdCategory();
    _radio = settings->radioCategory();
}

JsHandler::~JsHandler() { }

void JsHandler::processFile(const QString &jsFile)
{
    QFile file(jsFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        _lineList << in.readLine();
    }

    processList();
}

void JsHandler::processList()
{
    QString line;
    foreach (const QString &l, _lineList) {
        if (l.contains("la=")) {
            line = l;
            break;
        }
    }

    line = line.replace("la=[[", "");
    line = line.replace("]];", "");
    line = line.replace("\"", "");

    QStringList channels = line.split("],[");
    QStringList currentChannel;
    foreach (const QString &c, channels) {
        currentChannel = c.split(",");

        Channel *channel = new Channel(currentChannel[1], currentChannel[2].toInt());
        channel->setUrl("udp://@" + currentChannel[3] + ":" + currentChannel[4]);
        if (currentChannel[5] == _radio) {
            channel->setType(Tano::Radio);
        } else if (currentChannel[5] == _hd) {
            channel->setType(Tano::HD);
        } else {
            channel->setType(Tano::SD);
            channel->setCategories(QStringList() << currentChannel[5]);
        }
        channel->setLanguage(currentChannel[6]);
        channel->setXmltvId(currentChannel[7]);

        _channelList << channel;
    }
}
