/****************************************************************************
* EpgShow.cpp: EPG show information
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

#include "EpgShow.h"
#include "../Common.h"

EpgShow::EpgShow(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	_file = 0;
	_loader = new EpgLoader();
	_http = new QHttp(this);

	connect(_loader, SIGNAL(epgDone(QStringList, int)), this, SLOT(display(QStringList)));
	connect(_http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));
}

EpgShow::~EpgShow()
{
	delete _loader;
	delete _http;
}

void EpgShow::open(const QString &url)
{
	ui.mainWidget->setCurrentIndex(0);

	setWindowTitle(tr("Show info"));
	ui.labelName->setText("");
	ui.labelTime->setText("");
	ui.labelInfo->setText("");
	ui.labelDescription->setText("");
	ui.labelActors->setText("");
	ui.labelPhoto->setPixmap(QPixmap(":/icons/images/image.png"));
	show();
	_loader->getEpg(url, true);
}

void EpgShow::loadPlugin(const QString &plugin)
{
	_loader->loadPlugin(plugin);
}

void EpgShow::display(const QStringList &list)
{
	setWindowTitle(list[0]);
	ui.labelName->setText("<h1>" + list[0] + "</h1>");
	ui.labelTime->setText("<h2>" + list[1] + "</h2>");
	ui.labelInfo->setText("<h3>" + list[2] + "</h3>");
	ui.labelDescription->setText("\n" + list[3]);
	ui.labelActors->setText("\n" + list[4]);

	downloadFile(list[5]);

	ui.mainWidget->setCurrentIndex(1);
}

void EpgShow::downloadFile(const QString &u)
{
	if(u == "")
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
	_http->setHost(url.host(), mode, url.port() == -1 ? 0 : url.port());

	if (!url.userName().isEmpty())
		_http->setUser(url.userName(), url.password());

	QByteArray path = QUrl::toPercentEncoding(url.path(), "!$&'()*+,;=:@/");
	if (path.isEmpty())
		path = "/";

	_httpGetId = _http->get(path, _file);
}

void EpgShow::httpRequestFinished(const int &requestId, const bool &error)
{
	if (requestId != _httpGetId)
		return;

	_file->close();

	if (!error) {
		ui.labelPhoto->setPixmap(QPixmap(_file->fileName()));
	}

	delete _file;
}
