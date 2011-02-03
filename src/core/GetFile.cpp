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

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QVariant>
#include <QtGui/QMessageBox>

#include "GetFile.h"

GetFile::GetFile(QObject *parent)
	: QObject(parent),
	_file(0) { }

GetFile::~GetFile() { }

void GetFile::getFile(const QString &fileUrl,
					  const QString &location)
{
	_url = fileUrl;

	QFileInfo fileInfo(_url.path());
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
		_file = 0;
		return;
	}

	startRequest(_url);
}

void GetFile::httpReadyRead()
{
	if (_file)
		_file->write(_nreply->readAll());
}

void GetFile::httpRequestFinished()
{
	_file->flush();
	_file->close();

	QVariant redirectionTarget = _nreply->attribute(QNetworkRequest::RedirectionTargetAttribute);
	if (_nreply->error()) {
		_file->remove();
		QMessageBox::information(0, tr("Tano"),
								 tr("Download failed: %1.")
								 .arg(_nreply->errorString()));
	} else if (!redirectionTarget.isNull()) {
		_url = _url.resolved(redirectionTarget.toUrl());
		_nreply->deleteLater();
		_file->open(QIODevice::WriteOnly);
		_file->resize(0);
		startRequest(_url);
		return;
	} else {
		emit file(_file->fileName());
	}

	disconnect(_nreply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
	disconnect(_nreply, SIGNAL(finished()), this, SLOT(httpRequestFinished()));
	_nreply->deleteLater();
	_nreply = 0;
	delete _file;
	_file = 0;
}

void GetFile::startRequest(const QUrl &url)
{
	_nreply = _nam.get(QNetworkRequest(url));
	connect(_nreply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
	connect(_nreply, SIGNAL(finished()), this, SLOT(httpRequestFinished()));
}
