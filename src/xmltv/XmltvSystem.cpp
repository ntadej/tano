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

#include <QtCore/QProcess>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>

#include "xmltv/XmltvSystem.h"

XmltvSystem::XmltvSystem(QObject *parent)
    : QObject(parent)
{
    _watcher = new QFutureWatcher< QList<XmltvGrabber> >(this);
    connect(_watcher, SIGNAL(finished()), this, SLOT(processGrabbers()));
}

XmltvSystem::~XmltvSystem()
{
    delete _watcher;
}

extern QList<XmltvGrabber> loadGrabbers()
{
    QProcess p;
    p.start("tv_find_grabbers");
    p.waitForFinished(-1);

    QList<XmltvGrabber> list;
    QTextStream in(&p);
    in.setCodec(QTextCodec::codecForName("UTF-8"));
    while (!in.atEnd()) {
        XmltvGrabber grabber;
        QStringList tmp = in.readLine().split("|");
        grabber.name = tmp[1];
        grabber.path = tmp[0];
        list << grabber;
    }

    return list;
}

void XmltvSystem::processGrabbers()
{
    QList<XmltvGrabber> result = _watcher->result();
    if(result.isEmpty())
        return;

    emit grabbers(result);
}

void XmltvSystem::requestGrabbers()
{
    QFuture< QList<XmltvGrabber> > future = QtConcurrent::run(loadGrabbers);
    _watcher->setFuture(future);
}
