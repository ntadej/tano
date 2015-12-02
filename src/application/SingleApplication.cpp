/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2015 Tadej Novak <tadej@tano.si>
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

#include <QtNetwork/QLocalSocket>

#include "application/Common.h"
#include "application/LocalServer.h"
#include "application/SingleApplication.h"

SingleApplication::SingleApplication(int &argc,
                                     char **argv)
    : QApplication(argc, argv)
{
    _shouldContinue = false; // By default this is not the main process

    QScopedPointer<QLocalSocket> socket(new QLocalSocket(this));;

    // Attempt to connect to the LocalServer
    socket->connectToServer(Tano::localServer());
    if (socket->waitForConnected(100)) {
        socket->close();
    } else {
        // The attempt was insuccessful, so we continue the program
        _shouldContinue = true;
        _server = new LocalServer(this);
        connect(_server, SIGNAL(connected()), this, SIGNAL(activate()));
    }
}

SingleApplication::~SingleApplication() { }

bool SingleApplication::shouldContinue()
{
    return _shouldContinue;
}
