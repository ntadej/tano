/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <info@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtXml/QXmlInputSource>
#include <QtXml/QXmlSimpleReader>

#include "container/UpdateInfo.h"
#include "core/UpdateManager.h"
#include "core/Version.h"

UpdateManager::UpdateManager(QObject *parent)
	: QHttp(parent),
	_codec(QTextCodec::codecForName("UTF-8"))
{
	setHost("update.tano.si");

	_handler = new UpdateHandler();
}

UpdateManager::~UpdateManager()
{
	delete _handler;
}

void UpdateManager::getUpdates()
{
	get("/player/update.xml");

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
