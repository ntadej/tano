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

#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include "channels/containers/Channel.h"
#include "settings/Settings.h"

#include "M3UHandler.h"

M3UHandler::M3UHandler()
{
    _name = QObject::tr("Channels list");

    QScopedPointer<Settings> settings(new Settings());
    _hd = settings->hdCategory();
    _radio = settings->radioCategory();
}

M3UHandler::~M3UHandler() { }

void M3UHandler::processFile(const QString &m3uFile)
{
    QFile file(m3uFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        _m3uLineList << in.readLine();
    }

    processList();
}

void M3UHandler::processList()
{
    QString tmp;
    QStringList tmpList;
    QStringList tmpCList;

    if (!_m3uLineList[0].contains("#EXTM3U"))
        return;

    for (int i = 1; i < _m3uLineList.size(); i++) {
        if (_m3uLineList[i] == "")
            continue;

        if (_m3uLineList[i].contains("#EXTNAME")) {
            _name = _m3uLineList[i];
            _name.replace(QString("#EXTNAME:"), QString(""));
        } else if (_m3uLineList[i].contains("#EXTINF")) {
            tmp = _m3uLineList[i];
            tmp.replace(QString("#EXTINF:"), QString(""));
            tmpList = tmp.split(",");

            _channel = new Channel(tmpList[1], tmpList[0].toInt());
            _channels << _channel;
        } else if (_m3uLineList[i].contains("#EXTTV")) {
            tmp = _m3uLineList[i];
            tmp.replace(QString("#EXTTV:"), QString(""));
            tmpList = tmp.split(";");

            if (!tmpList.isEmpty()) {
                tmpCList = tmpList[0].split(",");
                if (tmpCList.contains(_radio)) {
                    tmpCList.removeAll(_radio);
                    _channel->setType(Channel::Radio);
                } else if (tmpCList.contains(_hd)) {
                    tmpCList.removeAll(_hd);
                    _channel->setType(Channel::HD);
                } else {
                    _channel->setType(Channel::SD);
                }
                _channel->setCategories(tmpCList);
                _channel->setLanguage(tmpList[1]);
                _channel->setXmltvId(tmpList[2]);
            }
        } else if (_m3uLineList[i].contains("#EXTLOGO")) {
            tmp = _m3uLineList[i];
            tmp.replace(QString("#EXTLOGO:"), QString(""));
            _channel->setLogo(tmp);
        } else {
            tmp = _m3uLineList[i];
            _channel->setUrl(tmp);
        }
    }
}
