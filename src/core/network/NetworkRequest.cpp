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

#include <QtCore/QDebug>

#include "network/NetworkRequest.h"

NetworkRequest::NetworkRequest(QObject *parent)
    : QObject(parent),
      _currentData(""),
      _currentRequest(QNetworkRequest()),
      _currentResult("") { }

NetworkRequest::~NetworkRequest() { }

void NetworkRequest::getRequest(const QNetworkRequest &request)
{
    _currentRequest = request;

    _nreply = _nam.get(request);
    connect(_nreply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
    connect(_nreply, SIGNAL(finished()), this, SLOT(httpRequestFinished()));
}

void NetworkRequest::getRequest(const QUrl &url)
{
    getRequest(QNetworkRequest(url));
}

void NetworkRequest::httpError(const QNetworkReply::NetworkError &err)
{
    qDebug() << "NetworkRequest Error" << err;

    emit error(err);
}

void NetworkRequest::httpRequestFinished()
{
    QVariant redirectionTarget = _nreply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (_nreply->error()) {
        return;
    } else if (!redirectionTarget.isNull()) {
        _url = _url.resolved(redirectionTarget.toUrl());
        _nreply->deleteLater();

        _currentRequest.setUrl(_url);
        if(_currentData == "") {
            postRequest(_currentRequest, _currentData);
        } else {
            getRequest(_currentRequest);
        }

        return;
    } else {
        _currentResult = _nreply->readAll();
        emit result(_currentResult);
    }

    disconnect(_nreply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
    disconnect(_nreply, SIGNAL(finished()), this, SLOT(httpRequestFinished()));
    _nreply->deleteLater();

    _currentData = "";
    _currentRequest = QNetworkRequest();
    _currentResult = "";
}

void NetworkRequest::postRequest(const QNetworkRequest &request,
                                   const QByteArray &data)
{
    _currentData = data;
    _currentRequest = request;

    _nreply = _nam.post(request, data);
    connect(_nreply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(httpError(QNetworkReply::NetworkError)));
    connect(_nreply, SIGNAL(finished()), this, SLOT(httpRequestFinished()));
}
