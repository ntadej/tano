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

#ifndef TANO_NETWORKDOWNLOAD_H_
#define TANO_NETWORKDOWNLOAD_H_

#include <QtCore/QFile>
#include <QtCore/QPointer>

#include "CoreSharedExport.h"

class QNetworkReply;

class NetworkRequest;

/*!
    \class NetworkDownload NetworkDownload.h core/network/NetworkDownload.h
    \brief Download files from network

    A convenience class to download files from network
*/
class TANO_CORE_EXPORT NetworkDownload : public QObject
{
Q_OBJECT
public:
	NetworkDownload(QObject *parent = 0);
	~NetworkDownload();

    /*!
        \brief Request a file download
        \param fileUrl url of the file to be downloaded (QString)
        \param location location of the downloaded file - tmp, if not defined (QString)
    */
	void getFile(const QString &fileUrl,
				 const QString &location = 0);

signals:
    /*!
        \brief Signal emited when file succesfully downloaded
        \param file downloaded file object (QFile *)
    */
    void file(QFile *);

private slots:
    void write(const QByteArray &data,
               QNetworkReply *reply);

private:
    QPointer<QFile> _file;

    NetworkRequest *_request;
    QNetworkReply *_currentReply;
};

#endif // TANO_NETWORKDOWNLOAD_H_
