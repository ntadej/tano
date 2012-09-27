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

#ifndef TANO_NETWORKREQUEST_H_
#define TANO_NETWORKREQUEST_H_

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include "CoreSharedExport.h"

class TANO_CORE_EXPORT NetworkRequest : public QObject
{
Q_OBJECT
public:
	NetworkRequest(QObject *parent = 0);
	~NetworkRequest();

    QNetworkReply *getRequest(const QNetworkRequest &request);
    QNetworkReply *postRequest(const QNetworkRequest &request,
                               const QByteArray &data);

signals:
	void error(const int &);
    void result(const QByteArray &,
                QNetworkReply *);

private slots:
    void requestFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *_nam;

    QByteArray _currentData;
	QNetworkRequest _currentRequest;
	QByteArray _currentResult;

	QUrl _url;
};

#endif // MEEPLUS_NETWORKREQUEST_H_
