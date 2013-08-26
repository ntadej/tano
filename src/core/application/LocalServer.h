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

#ifndef TANO_LOCALSERVER_H_
#define TANO_LOCALSERVER_H_

#include <QtNetwork/QLocalServer>

#include "CoreSharedExport.h"

class TANO_CORE_EXPORT LocalServer : public QObject
{
Q_OBJECT
public:
    explicit LocalServer(QObject *parent = 0);
    ~LocalServer();

signals:
    void connected();

private slots:
    void newConnection();

private:
    QLocalServer *_server;
};

#endif // TANO_LOCALSERVER_H_
