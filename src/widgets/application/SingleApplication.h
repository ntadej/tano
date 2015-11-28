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

#ifndef TANO_SINGLEAPPLICATION_H_
#define TANO_SINGLEAPPLICATION_H_

#include <QApplication>

class LocalServer;

class SingleApplication : public QApplication
{
Q_OBJECT
public:
    explicit SingleApplication(int argc,
                               char *argv[]);
    ~SingleApplication();

    bool shouldContinue();

signals:
    void activate();
  
private:
    LocalServer *_server;
    bool _shouldContinue;
};

#endif // TANO_SINGLEAPPLICATION_H_
