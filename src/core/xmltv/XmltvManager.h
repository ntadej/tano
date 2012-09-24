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

#ifndef TANO_XMLTVMANAGER_H_
#define TANO_XMLTVMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QTimer>

#if defined(Qt5)
    #include <QtConcurrent/QFutureWatcher>
#elif defined(Qt4)
    #include <QtCore/QFutureWatcher>
#endif

#include "Common.h"
#include "xmltv/XmltvHandler.h"

extern bool loadXmltvStart(XmltvHandler *handler,
                           const QString &location);

class NetworkDownload;

class XmltvChannel;
class XmltvChannelsModel;
class XmltvList;
class XmltvProgramme;
class XmltvProgrammeModel;

class XmltvManager : public QObject
{
Q_OBJECT
public:
    explicit XmltvManager(QObject *parent = 0);
    ~XmltvManager();

    void loadXmltv();
    void loadXmltv(const QString &file);

public slots:
    void request(const QString &id,
                 const Tano::Id &identifier);
    void requestProgramme(const QString &id);
    void requestProgrammeNext(XmltvProgramme *current);
    void requestProgrammePrevious(XmltvProgramme *current);
    void stop();

signals:
    void current(const QString,
                 const QString);
    void schedule(XmltvProgrammeModel *,
                  const Tano::Id);
    void programme(XmltvProgramme *);

private slots:
    void current();
    void loadXmltvInit();
    void loadXmltvFinish();

private:
    QString processCurrentString(XmltvProgramme *programme) const;
    void updateWeb(const QString &location,
                   const QString &url);

    QString _location;

    bool _loading;

    Tano::Id _currentIdentifier;
    QString _currentXmltvId;

    NetworkDownload *_downloader;
    XmltvHandler *_handler;
    QTimer *_timer;
    QFutureWatcher<bool> *_watcher;
    XmltvList *_xmltv;
};

#endif // TANO_XMLTVMANAGER_H_
