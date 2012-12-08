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

#ifndef TANO_XMLTVSYSTEM_H_
#define TANO_XMLTVSYSTEM_H_

#include <QtCore/QFutureWatcher>
#include <QtCore/QString>

#include "CoreSharedExport.h"

struct XmltvGrabber
{
    QString name;
    QString path;
};

extern QList<XmltvGrabber> loadGrabbers();

class TANO_CORE_EXPORT XmltvSystem : public QObject
{
Q_OBJECT
public:
    XmltvSystem(QObject *parent = 0);
    ~XmltvSystem();

    void requestGrabbers();

signals:
    void grabbers(const QList<XmltvGrabber> &);

private slots:
    void processGrabbers();

private:
    QFutureWatcher< QList<XmltvGrabber> > *_watcher;
};

#endif // TANO_XMLTVSYSTEM_H_
