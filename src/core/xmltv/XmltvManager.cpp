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
      _loading(true),
      _currentXmltvId(""),
      _downloader(0)
{
    _db = new XmltvSql();
    _db->open();
    _handler = new XmltvHandler(_db);

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

QHash<QString, QString> XmltvManager::channels() const
{
    // TODO: Channel maping
    //if (_xmltv)
    //    return _xmltv->channels()->map();
    //else
        return QHash<QString, QString>();
}

void XmltvManager::current()
{
    // TODO: Current
    /*if (_xmltv->channels()->find(_currentXmltvId)->programme()->rowCount() < 2)
        return;

    for (int i = 1; i < _xmltv->channels()->find(_currentXmltvId)->programme()->rowCount(); i++) {
        if (QDateTime::currentDateTime() < _xmltv->channels()->find(_currentXmltvId)->programme()->row(i)->start()) {
            emit current(processCurrentString(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i-1)),
                         processCurrentString(_xmltv->channels()->find(_currentXmltvId)->programme()->row(i)));
            break;
        }
    }

    _timer->start(60000);*/
}

void XmltvManager::loadXmltv()
{
    QScopedPointer<Settings> settings(new Settings(this));
    _location = settings->xmltvUpdateLocation();

    if (!settings->xmltvUpdate())
        return;

    if (settings->xmltvUpdateRemote()) {
        // TODO: Remote update
        //updateWeb(settings->xmltvLocation(), settings->xmltvUpdateUrl());
    } else {
        loadXmltvInit();
    }
}

void XmltvManager::loadXmltv(const QString &file)
{
    _location = file;

    loadXmltvInit();
}

void XmltvManager::loadXmltvInit()
{
    if (_downloader)
        disconnect(_downloader, SIGNAL(file(QString)), this, SLOT(loadXmltvInit()));

    //QFuture<bool> future = QtConcurrent::run(loadXmltvStart, _handler, _location);
   // _watcher->setFuture(future);
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

    qDebug() << "XMLTV loaded!";

    return;

    _loading = false;

    emit channelsChanged(channels());

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

void XmltvManager::requestProgrammeNext(const QString &id)
{
    // TODO: Request next programme
}

void XmltvManager::requestProgrammePrevious(const QString &id)
{
    // TODO: Request previous programme
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

void XmltvManager::requestQml(const QString &id)
{
    request(id, Tano::Schedule);
}

void XmltvManager::requestQmlCurrent(const QString &id)
{
    request(id, Tano::Main);
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
