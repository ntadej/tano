/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#include <QDebug>
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

void GetFile::getFile(const QString &fileUrl,
					  const QString &location)
{
	if(fileUrl.isEmpty())
		return;

	QUrl url(fileUrl);
	QFileInfo fileInfo(url.path());

	qDebug() << fileUrl;

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

void GetFile::httpRequestFinished(const int &requestId,
								  const bool &error)
{
	if (requestId != _httpGetId)
		return;

	_file->close();

	QString tmp;
	if(lastResponse().statusCode() < 400 && lastResponse().statusCode() > 200) {
		if (_file->open(QIODevice::ReadOnly | QIODevice::Text)) {
			tmp = QString::fromUtf8(_file->readAll());
			_file->close();
			delete _file;
			QRegExp newUrl("HREF=\"([^\"]*)");
			newUrl.indexIn(tmp);
			//qDebug() << newUrl.cap(1) << tmp;
			getFile(newUrl.cap(1));
		} else {
			delete _file;
		}
		return;
	}

	if (!error) {
		emit file(_file->fileName());
	}

	delete _file;
}
