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

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>
#include <QtNetwork/QNetworkReply>

#if defined(Qt5)
    #include <QtWidgets/QMessageBox>
#elif defined(Qt4)
    #include <QtGui/QMessageBox>
#endif

#include "NetworkDownload.h"
#include "core/NetworkRequest.h"

NetworkDownload::NetworkDownload(QObject *parent)
    : QObject(parent),
      _file(0),
      _request(new NetworkRequest(this))
{
    connect(_request, SIGNAL(result(QByteArray)), this, SLOT(write(QByteArray)));
}

NetworkDownload::~NetworkDownload() { }

void NetworkDownload::getFile(const QString &fileUrl,
                      const QString &location)
{
    if(fileUrl.isEmpty())
        return;

    QUrl url = fileUrl;

    QFileInfo fileInfo(url.path());
    QString fileName;
    QString path;
    if(!location.isNull()) {
        fileName = location;
    } else {
        QDir dir(QDir::tempPath());
        dir.mkdir("tano");
        path = QDir::tempPath() + "/tano/";
        fileName = path + fileInfo.fileName();
    }

    _file = new QFile(fileName);
    if (!_file->open(QIODevice::WriteOnly)) {
        QMessageBox::information(0, tr("Tano"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName, _file->errorString()));
        delete _file;
        return;
    }

    _request->getRequest(url);
}

void NetworkDownload::write(const QByteArray &data)
{
    if (!_file)
        return;

    _file->write(data);
    _file->flush();
    _file->close();

    emit file(_file->fileName());
    delete _file;
}
