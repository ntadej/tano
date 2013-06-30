/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include <QtCore/QFile>
#include <QtCore/QFutureWatcher>
#include <QtCore/QObject>
#include <QtCore/QTimer>

#include "CoreSharedExport.h"

#include "network/NetworkDownload.h"
#include "xmltv/XmltvHandler.h"

extern bool loadXmltvStart(XmltvHandler *handler,
                           QFile *file);

class XmltvProgramme;
class XmltvProgrammeModel;
class XmltvSql;

class TANO_CORE_EXPORT XmltvManager : public QObject
{
Q_OBJECT
public:
    explicit XmltvManager(QObject *parent = 0);
    ~XmltvManager();

    QHash<QString, QString> channels() const;

public slots:
    void request(const QString &id,
                 bool current = false);
    void requestProgramme(const QString &id);
    void requestProgrammeCurrent(const QString &channel);
    void requestProgrammeNext(const QString &id,
                              const QString &channel);
    void requestProgrammePrevious(const QString &id,
                                  const QString &channel);
    void requestProgrammeRecord(const QString &id);
    void stop();

signals:
    void channelsChanged(const QHash<QString, QString> &);
    void current(const QStringList &epg);
    void schedule(const QString &,
                  XmltvProgrammeModel *);
    void programme(XmltvProgramme *);
    void programmeRecord(XmltvProgramme *);

private slots:
    void current();
    void loadXmltv();
    void loadXmltvInit(QFile *file);
    void loadXmltvFinish();

private:
    void loadXmltvWeb(const QString &url);

    QString _currentXmltvId;

    XmltvSql *_db;
    QPointer<NetworkDownload> _downloader;
    QPointer<QFile> _file;
    XmltvHandler *_handler;
    QTimer *_timer;
    QFutureWatcher<bool> *_watcher;
};

#endif // TANO_XMLTVMANAGER_H_
