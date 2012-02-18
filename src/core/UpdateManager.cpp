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

#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#include "core/Common.h"
#include "core/UpdateManager.h"

UpdateManager::UpdateManager(QObject *parent)
    : QHttp(parent),
     _codec(QTextCodec::codecForName("UTF-8"))
{
    setHost("update.tano.si");

    _handler = new UpdateHandler();
}

UpdateManager::~UpdateManager()
{
    delete _handler;
}

void UpdateManager::getUpdates()
{
    get("/player/update.xml");

    connect(this, SIGNAL(done(bool)), this, SLOT(readUpdates()));
}

void UpdateManager::readUpdates()
{
    disconnect(this, SIGNAL(done(bool)), this, SLOT(readUpdates()));

    QByteArray httpResponse = readAll();
    QString string = _codec->toUnicode(httpResponse);

    QXmlSimpleReader reader;
    reader.setContentHandler(_handler);
    reader.setErrorHandler(_handler);

    QXmlInputSource xmlInputSource;
    xmlInputSource.setData(string);
    if (!reader.parse(xmlInputSource))
        return;

    QStringList updatesList;
    QList<UpdateInfo> list = _handler->updateInfo();
    UpdateInfo info;

    foreach (UpdateInfo item, list) {
        if(Tano::version() == item.version && item.development) {
            updatesList << "development" << item.version;
            continue;
        } else if(Tano::version() == item.version && !item.development) {
            updatesList << "latest";
            break;
        } else if(Tano::version() != item.version && item.development) {
            continue;
        } else if(Tano::version() != item.version && !item.development) {
            if(!updatesList.isEmpty()) {
                updatesList << item.version;
            } else {
                updatesList << "update" << item.version;
                info = item;
            }
        }
    }

    emit updateInfo(updatesList, info);
}
