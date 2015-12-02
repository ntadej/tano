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

#include <QtCore/QStandardPaths>

#include "Config.h"
#include "common/NetworkManager.h"

NetworkManager::NetworkManager(QObject *parent)
    : QNetworkAccessManager(parent)
{    
    _cache = new QNetworkDiskCache(this);
    _cache->setCacheDirectory(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
    _cache->setMaximumCacheSize(1073741824); // 1 GB
    setCache(_cache);

    connect(this, &QNetworkAccessManager::sslErrors, this, &NetworkManager::sslError);
}

NetworkManager::~NetworkManager() { }

QNetworkReply *NetworkManager::request(const QNetworkRequest &request,
                                       const QNetworkAccessManager::Operation &operation,
                                       const QByteArray &data)
{
    QNetworkReply *reply;
    switch (operation) {
    case QNetworkAccessManager::PostOperation:
        reply = post(request, data);
        break;
    case QNetworkAccessManager::GetOperation:
    default:
        reply = get(request);
        break;
    }

    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
    connect(reply, &QNetworkReply::finished, this, &NetworkManager::httpRequestFinished);

    return reply;
}

void NetworkManager::httpError(QNetworkReply::NetworkError err)
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());

    qDebug() << "NetworkRequest Error" << err;

    emit error(reply, err);
}

void NetworkManager::httpRequestFinished()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    if (reply->error()) {
        return;
    } else {
        emit result(reply);
    }

    disconnect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
    disconnect(reply, &QNetworkReply::finished, this, &NetworkManager::httpRequestFinished);
}

void NetworkManager::sslError(QNetworkReply *reply,
                              QList<QSslError> error)
{
#if !RELEASE
    reply->ignoreSslErrors(error);
#endif
}
