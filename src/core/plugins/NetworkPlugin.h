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

#ifndef TANO_NETWORKPLUGIN_H_
#define TANO_NETWORKPLUGIN_H_

#include <QtCore/QObject>
#include <QtCore/QString>

class NetworkPlugin : public QObject
{
Q_OBJECT
public:
    NetworkPlugin(QObject *parent = 0) : QObject(parent) { }
    virtual ~NetworkPlugin() { }

    virtual void authentication(const QString &username,
                                const QString &password) = 0;

    virtual void statusLogin() = 0;
    virtual void statusLogout() = 0;
    virtual void statusPlaylist() = 0;
    virtual void statusPlay(int id) = 0;
    virtual void statusStop() = 0;

signals:
    void passwordOk(QString);
    void passwordError(int);
};

#endif // TANO_NETWORKPLUGIN_H_
