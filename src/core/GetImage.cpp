/****************************************************************************
* GetImage.cpp: Image downloader class
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

#include "GetImage.h"

GetImage::GetImage(QObject *parent) :
	QHttp(parent),
	_file(0),
	_httpGetId(0)
{
	connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

GetImage::~GetImage()
{

}

void GetImage::getImage(const QString &u)
{
	if(u.isEmpty())
		return;

	QUrl url(u);
	QFileInfo fileInfo(url.path());

	if (QFile::exists(fileInfo.fileName())) {
		QFile::remove(fileInfo.fileName());
	}

	_file = new QFile(QDir::tempPath() + "/" + fileInfo.fileName());
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

void GetImage::httpRequestFinished(const int &requestId, const bool &error)
{
	if (requestId != _httpGetId)
		return;

	_file->close();

	if (!error) {
		emit image(_file->fileName());
	}

	delete _file;
}
