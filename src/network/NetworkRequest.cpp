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

#include "NetworkRequest.h"

NetworkRequest::NetworkRequest(QObject *parent)
    : QObject(parent),
      _nam(new QNetworkAccessManager(this)),
      _currentData(""),
      _currentRequest(QNetworkRequest()),
      _currentResult("")
{
    connect(_nam, SIGNAL(finished(QNetworkReply *)), this, SLOT(requestFinished(QNetworkReply *)));
    connect(_nam, SIGNAL(authenticationRequired(QNetworkReply *, QAuthenticator *)), this, SIGNAL(auth(QNetworkReply *, QAuthenticator *)));
}

NetworkRequest::~NetworkRequest()
{
    delete _nam;
}

QNetworkReply *NetworkRequest::getRequest(const QNetworkRequest &request)
{
    _currentRequest = request;

    return _nam->get(request);
}

void NetworkRequest::requestFinished(QNetworkReply *reply)
{
    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        emit error(reply->error(), reply);
        return;
    } else if (!redirectionTarget.isNull()) {
        _url = _url.resolved(redirectionTarget.toUrl());
        reply->deleteLater();

        _currentRequest.setUrl(_url);
        if(_currentData == "") {
            postRequest(_currentRequest, _currentData);
        } else {
            getRequest(_currentRequest);
        }

        return;
    } else {
        _currentResult = reply->readAll();
        emit result(_currentResult, reply);
    }

    reply->deleteLater();

    _currentData = "";
    _currentRequest = QNetworkRequest();
    _currentResult = "";
}

QNetworkReply *NetworkRequest::postRequest(const QNetworkRequest &request,
                                           const QByteArray &data)
{
    _currentData = data;
    _currentRequest = request;

    return _nam->post(request, data);
}
