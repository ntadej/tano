/****************************************************************************
* UpdateManager.cpp: Update manager
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

#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#include "core/UpdateInfo.h"
#include "core/UpdateManager.h"
#include "core/Version.h"

UpdateManager::UpdateManager(QObject *parent)
	: QHttp(parent),
	_codec(QTextCodec::codecForName("UTF-8"))
{
	setHost("update.tanoplayer.co.cc");

	_handler = new UpdateHandler();
}

UpdateManager::~UpdateManager()
{
	delete _handler;
}

void UpdateManager::getUpdates()
{
	get("/update.xml");
	connect(this, SIGNAL(done(bool)), this, SLOT(readUpdates()));
}

void UpdateManager::readUpdates()
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(readUpdates()));

	QByteArray httpResponse = readAll();
	QString string = _codec->toUnicode(httpResponse);

	QXmlSimpleReader reader;
	reader.setContentHandler(_handler);
	reader.setErrorHandler(_handler);

	QXmlInputSource xmlInputSource;
	xmlInputSource.setData(string);
	if (!reader.parse(xmlInputSource))
		return;

	QStringList updatesList;
	QList<UpdateInfo> list = _handler->updateInfo();
	UpdateInfo info;

	for(int i=0; i < list.size(); i++) {
		if(Version::version() == list[i].version() && list[i].development()) {
			updatesList << "development" << list[i].version();
			continue;
		} else if(Version::version() == list[i].version() && !list[i].development()) {
			updatesList << "latest";
			break;
		} else if(Version::version() != list[i].version() && list[i].development()) {
			continue;
		} else if(Version::version() != list[i].version() && !list[i].development()) {
			if(!updatesList.isEmpty()) {
				updatesList << list[i].version();
			} else {
				updatesList << "update" << list[i].version();
				info = list[i];
			}
		}
	}

	emit updateInfo(updatesList, info);
}
