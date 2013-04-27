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

#include <QtCore/QDebug>

#if QT_VERSION >= 0x050000
    #include <QtConcurrent/QtConcurrentRun>
#else
    #include <QtCore/QtConcurrentRun>
#endif

#include "Resources.h"
#include "network/NetworkDownload.h"
#include "settings/Settings.h"
#include "xmltv/XmltvCommon.h"
#include "xmltv/XmltvManager.h"
#include "xmltv/XmltvSql.h"
#include "xmltv/containers/XmltvChannel.h"
#include "xmltv/containers/XmltvProgramme.h"
#include "xmltv/models/XmltvProgrammeModel.h"

XmltvManager::XmltvManager(QObject *parent)
    : QObject(parent),
      _currentXmltvId("")
{
    _db = new XmltvSql();
    _db->open();
    _db->clean();
    _handler = new XmltvHandler(_db);

    _timer = new QTimer(this);
    connect(_timer, SIGNAL(timeout()), this, SLOT(current()));

    _watcher = new QFutureWatcher<bool>(this);
    connect(_watcher, SIGNAL(finished()), this, SLOT(loadXmltvFinish()));

    loadXmltv();
}

XmltvManager::~XmltvManager()
{
    if (_downloader)
        delete _downloader;
    delete _handler;
    delete _timer;
    delete _watcher;
}

QHash<QString, QString> XmltvManager::channels() const
{
    return _db->channels();
}

void XmltvManager::current()
{
    emit current(_db->programmeCurrent(_currentXmltvId));

    _timer->start(60000);
}

void XmltvManager::loadXmltv()
{
    QScopedPointer<Settings> settings(new Settings(this));
    if (!settings->xmltvUpdate())
        return;

    if (settings->xmltvUpdateRemote()) {
        loadXmltvWeb(settings->xmltvUpdateLocation());
    } else {
        loadXmltvInit(new QFile(Tano::Resources::resource(settings->xmltvUpdateLocation())));
    }
}

void XmltvManager::loadXmltvInit(QFile *file)
{
    if (_downloader)
        disconnect(_downloader, SIGNAL(file(QFile *)), this, SLOT(loadXmltvInit(QFile *)));

    _file = file;
    _db->startTransaction();

    QFuture<bool> future = QtConcurrent::run(loadXmltvStart, _handler, _file);
    _watcher->setFuture(future);
}

bool loadXmltvStart(XmltvHandler *handler,
                    QFile *file)
{
    QXmlSimpleReader reader;
    reader.setContentHandler(handler);
    reader.setErrorHandler(handler);

    if (!file->exists())
        return false;
    if (!file->open(QFile::ReadOnly | QFile::Text))
        return false;

    QXmlInputSource xmlInputSource(file);

    return reader.parse(xmlInputSource);
}

void XmltvManager::loadXmltvFinish()
{
    delete _file;
    _db->endTransaction();

    if (!_watcher->result())
        return;

    qDebug() << "XMLTV loaded!";

    emit channelsChanged(channels());

    request(_currentXmltvId, _currentIdentifier);
}

void XmltvManager::loadXmltvWeb(const QString &url)
{
    if (!_downloader)
        _downloader = new NetworkDownload(this);
    connect(_downloader, SIGNAL(file(QFile *)), this, SLOT(loadXmltvInit(QFile *)));
    _downloader->getFile(url);
}

void XmltvManager::request(const QString &id,
                           const Tano::Id &identifier)
{
    if (id.isEmpty())
        return;

    XmltvProgrammeModel *currentProgramme = new XmltvProgrammeModel(id, _db);

    if (!currentProgramme->rowCount()) {
        delete currentProgramme;
        return;
    }

    emit schedule(currentProgramme, identifier);

    _currentIdentifier = identifier;
    if (_currentIdentifier == Tano::Main) {
        _currentXmltvId = id;
        current();
    }
}

void XmltvManager::requestProgramme(const QString &id)
{
    if (id.isEmpty())
        return;

    XmltvProgramme *p = _db->programme(id);
    if (!p->id().isEmpty())
        emit programme(p);
    else
        delete p;
}

void XmltvManager::requestProgrammeNext(const QString &id,
                                        const QString &channel)
{
    if (id.isEmpty() || channel.isEmpty())
        return;

    XmltvProgramme *p = _db->programmeNext(id, channel);
    if (!p->id().isEmpty())
        emit programme(p);
    else
        delete p;
}

void XmltvManager::requestProgrammePrevious(const QString &id,
                                            const QString &channel)
{
    if (id.isEmpty() || channel.isEmpty())
        return;

    XmltvProgramme *p = _db->programmePrevious(id, channel);
    if (!p->id().isEmpty())
        emit programme(p);
    else
        delete p;
}

void XmltvManager::requestProgrammeRecord(const QString &id)
{
    if (id.isEmpty())
        return;

    XmltvProgramme *p = _db->programme(id);
    if (!p->id().isEmpty())
        emit programmeRecord(p);
    else
        delete p;
}

void XmltvManager::stop()
{
    _timer->stop();
}
