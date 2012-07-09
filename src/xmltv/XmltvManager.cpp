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

#if defined(Qt5)
    #include <QtConcurrent/QtConcurrentRun>
#elif defined(Qt4)
    #include <QtCore/QtConcurrentRun>
#endif

#include "container/xmltv/XmltvChannel.h"
#include "container/xmltv/XmltvList.h"
#include "container/xmltv/XmltvProgramme.h"
#include "core/NetworkDownload.h"
#include "core/Resources.h"
#include "core/Settings.h"
#include "xmltv/XmltvCommon.h"
#include "xmltv/XmltvChannelsModel.h"
#include "xmltv/XmltvHandler.h"
#include "xmltv/XmltvManager.h"
#include "xmltv/XmltvProgrammeModel.h"

XmltvManager::XmltvManager(QObject *parent)
    : QObject(parent),
      _loading(true),
      _currentXmltvId(""),
      _downloader(0),
      _xmltv(0)
{
    _handler = new XmltvHandler();

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(current()));

    _watcher = new QFutureWatcher<bool>(this);
    connect(_watcher, SIGNAL(finished()), this, SLOT(loadXmltvFinish()));
}

XmltvManager::~XmltvManager()
{
    if (_downloader)
        delete _downloader;
    delete _handler;
    delete _timer;
    delete _watcher;
}

void XmltvManager::current()
{
    if (_xmltv->channels()->find(_currentXmltvId)->programme()->rowCount() < 2)
        return;

    for (int i = 1; i < _xmltv->channels()->find(_currentXmltvId)->programme()->rowCount(); i++) {
        if (QDateTime::currentDateTime() < _xmltv->channels()->find(_currentXmltvId)->programme()->row(i)->start()) {
            emit current(processCurrentString(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i-1)),
                         processCurrentString(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i)));
            break;
        }
    }

    _timer->start(60000);
}

void XmltvManager::loadXmltv()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _location = settings->xmltvLocation();

    if (settings->xmltvUpdate()) {
        updateWeb(settings->xmltvLocation(), settings->xmltvUpdateUrl());
    } else {
        loadXmltvInit();
    }
}

void XmltvManager::loadXmltvInit()
{
    if (_downloader)
        disconnect(_downloader, SIGNAL(file(QString)), this, SLOT(loadXmltvInit()));

    QFuture<bool> future = QtConcurrent::run(loadXmltvStart, _handler, _location);
    _watcher->setFuture(future);
}

bool loadXmltvStart(XmltvHandler *handler,
                    const QString &location)
{
    QXmlSimpleReader reader;
    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);

    QFile file(Tano::Resources::resource(location));
    if (!file.exists())
        return false;
    if (!file.open(QFile::ReadOnly | QFile::Text))
        return false;

    QXmlInputSource xmlInputSource(&file);

    return reader.parse(xmlInputSource);
}

void XmltvManager::loadXmltvFinish()
{
    if (!_watcher->result())
        return;

    _xmltv = _handler->list();
    _loading = false;

    request(_currentXmltvId, _currentIdentifier);
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
    if (id.isEmpty())
        return;

    if (!_xmltv) {
        _currentIdentifier = identifier;
        _currentXmltvId = id;
        return;
    }

    XmltvProgrammeModel *currentProgramme;
    if (_xmltv->channels()->find(id))
        currentProgramme = _xmltv->channels()->find(id)->programme();
    else
        return;

    if (!currentProgramme->rowCount())
        return;

    emit schedule(currentProgramme, identifier);

    _currentIdentifier = identifier;
    if (_currentIdentifier == Tano::Main) {
        _currentXmltvId = id;
        current();
    }
}

void XmltvManager::requestProgramme(const QString &id)
{
    for (int i = 1; i < _xmltv->channels()->find(_currentXmltvId)->programme()->rowCount(); i++) {
        if (_xmltv->channels()->find(_currentXmltvId)->programme()->row(i)->start() == QDateTime::fromString(id, Tano::Xmltv::dateFormat())) {
            emit programme(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i));
            break;
        }
    }
}

void XmltvManager::requestProgrammeNext(XmltvProgramme *current)
{
    if (_xmltv->channels()->find(current->channel())->programme()->indexFromItem(current).row() != _xmltv->channels()->find(current->channel())->programme()->rowCount()-1) {
        emit programme(_xmltv->channels()->find(current->channel())->programme()->row(_xmltv->channels()->find(current->channel())->programme()->indexFromItem(current).row()+1));
    }
}

void XmltvManager::requestProgrammePrevious(XmltvProgramme *current)
{
    if (_xmltv->channels()->find(current->channel())->programme()->indexFromItem(current).row() != 0) {
        emit programme(_xmltv->channels()->find(current->channel())->programme()->row(_xmltv->channels()->find(current->channel())->programme()->indexFromItem(current).row()-1));
    }
}

void XmltvManager::stop()
{
    _timer->stop();
}

void XmltvManager::updateWeb(const QString &location,
                             const QString &url)
{
    QFile file(location);
    if (file.exists())
        file.remove();

    _downloader = new NetworkDownload(this);
    connect(_downloader, SIGNAL(file(QString)), this, SLOT(loadXmltvInit()));
    _downloader->getFile(url, location);
}
