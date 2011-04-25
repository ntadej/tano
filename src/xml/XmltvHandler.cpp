/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This file was based on the example classes of the Qt Toolkit.
* Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
* Contact: Qt Software Information (qt-info@nokia.com)
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
#include "container/xmltv/XmltvProgramme.h"
#include "core/ConsoleOutput.h"
#include "epg/XmltvCommon.h"
#include "XmltvHandler.h"

XmltvHandler::XmltvHandler()
	: _metTag(false) { }

XmltvHandler::~XmltvHandler()
{
	delete _list;
}

bool XmltvHandler::startElement(const QString & /* namespaceURI */,
								const QString & /* localName */,
								const QString &qName,
								const QXmlAttributes &attributes)
{
	if (!_metTag && qName != "tv") {
		_errorStr = QObject::tr("The file is not a valid XMLTV file.");
		return false;
	}

	if (qName == "tv") { // Main
		_list = new XmltvList(QDate::fromString(attributes.value("date"), Tano::Xmltv::dateFormat()));
		_list->setSourceInfoUrl(attributes.value("source-info-url"));
		_list->setSourceInfoName(attributes.value("source-info-name"));
		_list->setSourceDataUrl(attributes.value("source-data-url"));
		_list->setGeneratorInfoName(attributes.value("source-info-url"));
		_list->setGeneratorInfoUrl(attributes.value("source-info-url"));
		_metTag = true;
	} else if (qName == "channel") { // Channel
		if(_list) {
			_currentChannel = new XmltvChannel(attributes.value("id"));
			_list->addChannel(_currentChannel);
		}
	} else if (qName == "icon") {
		if(_list && _currentChannel) {
			_currentChannel->setIcon(attributes.value("src"));
		}
	} else if (qName == "programme") { // Programme
		if(_list) {
			QString start = attributes.value("start").replace(Tano::Xmltv::dateRegExp(), "");
			QString stop = attributes.value("stop").replace(Tano::Xmltv::dateRegExp(), "");
			_currentProgramme = new XmltvProgramme(attributes.value("channel"));
			_currentProgramme->setStart(QDateTime::fromString(start, Tano::Xmltv::dateFormat()));
			_currentProgramme->setStop(QDateTime::fromString(stop, Tano::Xmltv::dateFormat()));
			_list->channel(attributes.value("channel"))->addProgramme(_currentProgramme);
		}
	}

	_currentText.clear();
	return true;
}

bool XmltvHandler::endElement(const QString & /* namespaceURI */,
							  const QString & /* localName */,
							  const QString &qName)
{
	// Channel
	if(qName == "display-name") {
		if(_list && _currentChannel) {
			_currentChannel->setDisplayName(_currentText);
		}
	} else if(qName == "url") {
		if(_list && _currentChannel) {
			_currentChannel->setUrl(_currentText);
		}
	} else if(qName == "title") { // Programme
		if(_list && _currentProgramme) {
			_currentProgramme->setTitle(_currentText);
		}
	}

	_currentText.clear();
	return true;
}

bool XmltvHandler::characters(const QString &str)
{
	_currentText += str;
	return true;
}

bool XmltvHandler::fatalError(const QXmlParseException &exception)
{
	if(ConsoleOutput::debug())
		qDebug() << QString("Parse error at line %1, column %2: %3")
							.arg(exception.lineNumber())
							.arg(exception.columnNumber())
							.arg(exception.message());
	return false;
}
