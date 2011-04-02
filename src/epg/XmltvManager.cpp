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

#include <QtCore/QDebug>

#include "container/xmltv/XmltvChannel.h"
#include "container/xmltv/XmltvList.h"
#include "core/Common.h"
#include "epg/XmltvManager.h"
#include "xml/XmltvHandler.h"

XmltvManager::XmltvManager(QObject *parent)
	: QObject(parent)
{
	_handler = new XmltvHandler();
	loadXmltv();
}

XmltvManager::~XmltvManager()
{
	delete _handler;
}

void XmltvManager::loadXmltv()
{
	QXmlSimpleReader reader;
	reader.setContentHandler(_handler);
	reader.setErrorHandler(_handler);

	QFile file(Tano::locateResource("xmltv.xml")); // Test file
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;

	QXmlInputSource xmlInputSource(&file);
	if (!reader.parse(xmlInputSource))
		return;

	// Debug
	qDebug() << _handler->list()->sourceInfoName() << _handler->list()->sourceInfoUrl();
	for(int i = 0; i < 5; i++) {
		qDebug() << _handler->list()->channels()[i]->id()
				 << _handler->list()->channels()[i]->displayName()
				 << _handler->list()->channels()[i]->icon()
				 << _handler->list()->channels()[i]->url();
	}
}
