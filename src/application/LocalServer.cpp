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

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QStringList>

#include "common/Common.h"
#include "application/LocalServer.h"

LocalServer::LocalServer(QObject *parent)
    : QObject(parent)
{
    _server = new QLocalServer(this);
    _server->removeServer(Tano::localServer());
    if (!_server->listen(Tano::localServer())) {
        qCritical() << "Local server could not be started." << _server->errorString();
        return;
    }

    connect(_server, SIGNAL(newConnection()), this, SLOT(newConnection()));
}

LocalServer::~LocalServer() { }

void LocalServer::newConnection()
{
    emit connected();
}
