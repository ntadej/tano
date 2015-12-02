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

#ifndef TANO_NETWORKMANAGER_H_
#define TANO_NETWORKMANAGER_H_

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkReply>

class NetworkManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkManager(QObject *parent = 0);
    ~NetworkManager();

    QNetworkReply *request(const QNetworkRequest &request,
                           const Operation &operation,
                           const QByteArray &data = QByteArray());

signals:
    void error(QNetworkReply *,
               QNetworkReply::NetworkError);
    void result(QNetworkReply *);

private slots:
    void httpError(QNetworkReply::NetworkError err);
    void httpRequestFinished();
    void sslError(QNetworkReply *reply,
                  QList<QSslError> error);

private:
    QNetworkDiskCache *_cache;
};

#endif // TANO_NETWORKMANAGER_H_
