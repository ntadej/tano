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

#include <QtCore/QDebug>
#include <QtCore/QtConcurrentRun>

#include "container/xmltv/XmltvChannel.h"
#include "container/xmltv/XmltvList.h"
#include "container/xmltv/XmltvProgramme.h"
#include "core/Common.h"
#include "epg/XmltvCommon.h"
#include "epg/XmltvChannelsModel.h"
#include "epg/XmltvHandler.h"
#include "epg/XmltvManager.h"
#include "epg/XmltvProgrammeModel.h"

XmltvManager::XmltvManager(QObject *parent)
    : QObject(parent),
      _type(Tano::EpgXmltv),
      _currentXmltvId("")
{
    _handler = new XmltvHandler();

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(current()));

    _watcher = new QFutureWatcher<bool>(this);
    connect(_watcher, SIGNAL(finished()), this, SLOT(loadXmltvFinish()));
}

XmltvManager::~XmltvManager()
{
    delete _handler;
    delete _timer;
    delete _watcher;
}

void XmltvManager::current()
{
    if(_xmltv->channels()->find(_currentXmltvId)->programme()->rowCount() < 2)
        return;

    for(int i = 1; i < _xmltv->channels()->find(_currentXmltvId)->programme()->rowCount(); i++) {
        if(QDateTime::currentDateTime() < _xmltv->channels()->find(_currentXmltvId)->programme()->row(i)->start()) {
            emit epgCurrent(processCurrentString(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i-1)),
                            processCurrentString(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i)));
            break;
        }
    }

    _timer->start(60000);
}

void XmltvManager::loadXmltv()
{
    if(_location.isEmpty())
        return;

    QFuture<bool> future = QtConcurrent::run(loadXmltvStart, _handler, _location);
    _watcher->setFuture(future);
}

bool loadXmltvStart(XmltvHandler *handler,
                    const QString &location)
{
    QXmlSimpleReader reader;
    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);

    QFile file(Tano::locateResource(location));
    if(!file.exists())
        return false;
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;

    QXmlInputSource xmlInputSource(&file);

    return reader.parse(xmlInputSource);
}

void XmltvManager::loadXmltvFinish()
{
    if(!_watcher->result())
        return;

    _xmltv = _handler->list();

    // Debug
    qDebug() << _handler->list()->sourceInfoName() << _handler->list()->sourceInfoUrl();
    for(int i = 0; i < 2; i++) {
        qDebug() << _handler->list()->channels()->row(i)->id()
                 << _handler->list()->channels()->row(i)->displayName()
                 << _handler->list()->channels()->row(i)->icon()
                 << _handler->list()->channels()->row(i)->url();
        for(int k = 0; k < 5; k++) {
            qDebug() << _handler->list()->channels()->row(i)->programme()->row(k)->channel()
                     << _handler->list()->channels()->row(i)->programme()->row(k)->start()
                     << _handler->list()->channels()->row(i)->programme()->row(k)->stop();
        }
    }
}

QString XmltvManager::processCurrentString(XmltvProgramme *programme) const
{
    QString output = "<a href=\"%1\">%2 - %3</a>";
    output = output.arg(programme->start().toString(Tano::Xmltv::dateFormat()), programme->start().toString("HH:mm"), programme->title());
    return output;
}

void XmltvManager::request(const QString &id,
                           const Tano::Id &identifier)
{
    if(id.isEmpty())
        return;

    if(!_xmltv)
        return;

    if(!_xmltv->channels()->find(id)->programme()->rowCount())
        return;

    emit epgSchedule(_xmltv->channels()->find(id)->programme(), identifier);

    _currentIdentifier = identifier;
    if(_currentIdentifier == Tano::Main) {
        _currentXmltvId = id;
        current();
    }
}

void XmltvManager::requestProgramme(const QString &programme)
{
    for(int i = 1; i < _xmltv->channels()->find(_currentXmltvId)->programme()->rowCount(); i++) {
        if(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i)->start() == QDateTime::fromString(programme, Tano::Xmltv::dateFormat())) {
            emit epgProgramme(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i));
            break;
        }
    }
}

void XmltvManager::requestProgrammeNext(XmltvProgramme *programme)
{
    if(_xmltv->channels()->find(programme->channel())->programme()->indexFromItem(programme).row() != _xmltv->channels()->find(programme->channel())->programme()->rowCount()-1) {
        emit epgProgramme(_xmltv->channels()->find(programme->channel())->programme()->row(_xmltv->channels()->find(programme->channel())->programme()->indexFromItem(programme).row()+1));
    }
}

void XmltvManager::requestProgrammePrevious(XmltvProgramme *programme)
{
    if(_xmltv->channels()->find(programme->channel())->programme()->indexFromItem(programme).row() != 0) {
        emit epgProgramme(_xmltv->channels()->find(programme->channel())->programme()->row(_xmltv->channels()->find(programme->channel())->programme()->indexFromItem(programme).row()-1));
    }
}

void XmltvManager::setSource(const Tano::EpgType &type,
                             const QString &location)
{
    _type = type;
    _location = location;

    loadXmltv();
}

void XmltvManager::stop()
{
    _timer->stop();
}
