/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This file is also part of the example classes of the Qt Toolkit.
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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "UpdateHandler.h"

UpdateHandler::UpdateHandler()
	: _metTanoTag(false) { }

UpdateHandler::~UpdateHandler() { }

bool UpdateHandler::startElement(const QString & /* namespaceURI */,
								const QString & /* localName */,
								const QString &qName,
								const QXmlAttributes &attributes)
{
	if (!_metTanoTag && qName != "tanoupdates") {
		_errorStr = QObject::tr("The file is not a Tano updates file.");
		return false;
	}

	if(qName == "tanoupdates") {
		_metTanoTag = true;
		_update.clear();
	} else if(qName == "application") {
		_update.append(UpdateInfo());
		_currentId = _update.size()-1;
	}

	_currentText.clear();
	return true;
}

bool UpdateHandler::endElement(const QString & /* namespaceURI */,
							 const QString & /* localName */,
							 const QString &qName)
{
	if (qName == "name") {
		_update[_currentId].setName(_currentText);
	} else if (qName == "version") {
		_update[_currentId].setVersion(_currentText);
	} else if (qName == "date") {
		_update[_currentId].setDate(_currentText);
	} else if (qName == "description") {
		_update[_currentId].setDescription(_currentText);
	} else if (qName == "development") {
		_update[_currentId].setDevelopment(_currentText.toInt());
	}

	return true;
}

bool UpdateHandler::characters(const QString &str)
{
	_currentText += str;
	return true;
}
