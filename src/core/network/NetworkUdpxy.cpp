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

#include "network/NetworkUdpxy.h"
#include "settings/Settings.h"

NetworkUdpxy::NetworkUdpxy(const bool &generate, QObject *parent)
    : QObject(parent),
      _generate(generate)
{
    createSettings();
}

NetworkUdpxy::~NetworkUdpxy() { }

void NetworkUdpxy::createSettings()
{
    QScopedPointer<Settings> settings(new Settings());
    _enabled = settings->udpxy();
    _url = settings->udpxyUrl();
    _port = QString::number(settings->udpxyPort());
}

QString NetworkUdpxy::processUrl(const QString &url) const
{
    QString u = url;
    if (_enabled || _generate) {
        if (!_url.isEmpty() && url.contains("udp://@")) {
           QString newUrl = "http://%1:%2/udp/%3";
           newUrl = newUrl.arg(_url, _port, u.replace("udp://@", ""));
           return newUrl;
        } else {
            return u;
        }
    } else {
        return u;
    }
}
