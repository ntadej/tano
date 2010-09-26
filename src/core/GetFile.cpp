/****************************************************************************
* GetFile.cpp: File downloader class
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QUrl>

#include "core/GetFile.h"

GetFile::GetFile(QObject *parent)
	: QHttp(parent),
	_file(0),
	_httpGetId(0)
{
	connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

GetFile::~GetFile() { }

void GetFile::getFile(const QString &fileUrl, const QString &location)
{
	if(fileUrl.isEmpty())
		return;

	QUrl url(fileUrl);
	QFileInfo fileInfo(url.path());

	QString destination;
	if(location.isNull()) {
		QDir dir(QDir::tempPath());
		dir.mkdir("tano");

		destination = QDir::tempPath() + "/tano/" + fileInfo.fileName();
	} else {
		destination = location;
	}

	_file = new QFile(destination);
	if (!_file->open(QIODevice::WriteOnly)) {
		delete _file;
		return;
	}

	QHttp::ConnectionMode mode = url.scheme().toLower() == "https" ? QHttp::ConnectionModeHttps : QHttp::ConnectionModeHttp;
	setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

	if (!url.userName().isEmpty())
		setUser(url.userName(), url.password());

	QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
	if (path.isEmpty())
		path = "/";

	_httpGetId = get(path, _file);
}

void GetFile::httpRequestFinished(const int &requestId, const bool &error)
{
	if (requestId != _httpGetId)
		return;

	_file->close();

	if (!error) {
		emit image(_file->fileName());
	}

	delete _file;
}
