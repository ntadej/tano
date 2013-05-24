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

#include "network/NetworkHttpAuth.h"
#include "settings/SettingsPassword.h"

NetworkHttpAuth::NetworkHttpAuth(const QString &password,
                                 QObject *parent)
    : QObject(parent),
      _password(password)
{
    createSettings();
}

NetworkHttpAuth::~NetworkHttpAuth() { }

void NetworkHttpAuth::createSettings()
{
    QScopedPointer<SettingsPassword> settings(new SettingsPassword());
    _username = settings->username();
}

QString NetworkHttpAuth::processUrl(const QString &url) const
{
    QString u = url;
    if (url.contains("http://")) {
       QString newUrl = "http://%1:%2@%3";
       newUrl = newUrl.arg(_username, _password, u.replace("http://", ""));
       return newUrl;
    } else {
        return u;
    }
}
