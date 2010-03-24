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

EpgShow::EpgShow(QWidget *parent)
	: QStackedWidget(parent), _file(0), _httpGetId(0), _epgNext(""), _epgPrevious("")
{
	ui.setupUi(this);

	_loader = new EpgLoader();
	_http = new QHttp(this);

	connect(_loader, SIGNAL(show(QStringList)), this, SLOT(display(QStringList)));
	connect(_http, SIGNAL(requestFinished(int, bool)), this, SLOT(httpRequestFinished(int, bool)));

	connect(ui.buttonPrevious, SIGNAL(clicked()), this, SLOT(previous()));
	connect(ui.buttonNext, SIGNAL(clicked()), this, SLOT(next()));
}

EpgShow::~EpgShow()
{
	delete _loader;
	delete _http;
}

void EpgShow::open(const QString &url)
{
	setCurrentIndex(0);

	setWindowTitle(tr("Show info"));
	ui.labelTitle->setText("");
	ui.labelTime->setText("");
	ui.labelInfo->setText("");
	ui.labelDescription->setText("");
	ui.labelStarring->setText("");
	ui.labelPhoto->setPixmap(QPixmap(":/icons/images/image.png"));

	show();
	_loader->getShow(url);
}

void EpgShow::loadPlugin(const QString &plugin)
{
	_loader->loadPlugin(plugin);
}

void EpgShow::display(const QStringList &list)
{
	setWindowTitle(list[0]);
	ui.labelTitle->setText("<h1>" + list[0] + "</h1>");
	ui.labelTime->setText("<h2>" + list[3] + " (" + list[1] + list[2] + ")</h2>");
	ui.labelInfo->setText("<h3>" + list[4] + "</h3>");
	ui.labelDescription->setText(list[5]);
	if(!list[6].isEmpty())
		ui.labelStarring->setText("<b>" + tr("Starring:") + "</b> " + list[6]);

	downloadFile(list[7]);

	_epgPrevious = list[8];
	_epgNext =list[9];

	setCurrentIndex(1);
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

void EpgShow::next()
{
	if(_epgNext.isEmpty())
		return;

	open(_epgNext);
}

void EpgShow::previous()
{
	if(_epgPrevious.isEmpty())
		return;

	open(_epgPrevious);
}
