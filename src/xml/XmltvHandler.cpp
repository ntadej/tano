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

#include "container/xmltv/XmltvList.h"
#include "core/ConsoleOutput.h"
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

	if (qName == "tv") {
		_list = new XmltvList(QDate::fromString(attributes.value("date"), "YYYYMMDDhhmmss"));
		_list->setSourceInfoUrl(attributes.value("source-info-url"));
		_list->setSourceInfoName(attributes.value("source-info-name"));
		_list->setSourceDataUrl(attributes.value("source-data-url"));
		_list->setGeneratorInfoName(attributes.value("source-info-url"));
		_list->setGeneratorInfoUrl(attributes.value("source-info-url"));
		_metTag = true;
	} else if (qName == "channel") {
		// Channel
	} else if (qName == "programme") {
		// Programme
	}

	_currentText.clear();
	return true;
}

bool XmltvHandler::endElement(const QString & /* namespaceURI */,
								const QString & /* localName */,
								const QString &qName)
{
	//if (qName == "title") {
		//_currentText;
	//}
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
