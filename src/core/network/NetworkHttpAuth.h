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

#ifndef TANO_NETWORKHTTPAUTH_H_
#define TANO_NETWORKHTTPAUTH_H_

#include <QtCore/QObject>

#include "CoreSharedExport.h"

class TANO_CORE_EXPORT NetworkHttpAuth : public QObject
{
Q_OBJECT
public:
    NetworkHttpAuth(const QString &password,
                    QObject *parent = 0);
    ~NetworkHttpAuth();

    void createSettings();
    void logout();
    QString processUrl(const QString &url) const;

private:
    QString _username;
    QString _password;
};

#endif // TANO_NETWORKHTTPAUTH_H_
