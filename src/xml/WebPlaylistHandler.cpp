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

#include "WebPlaylistHandler.h"

WebPlaylistHandler::WebPlaylistHandler()
	: _metTanoTag(false) { }

WebPlaylistHandler::~WebPlaylistHandler() { }

bool WebPlaylistHandler::startElement(const QString & /* namespaceURI */,
									  const QString & /* localName */,
									  const QString &qName,
									  const QXmlAttributes &attributes)
{
	if (!_metTanoTag && qName != "channels") {
		_errorStr = QObject::tr("The file is not a Web playlist import file.");
		return false;
	}

	if(qName == "channels") {
		_metTanoTag = true;
		_playlistName.clear();
		_playlistUrl.clear();
	}

	_currentText.clear();
	return true;
}

bool WebPlaylistHandler::endElement(const QString & /* namespaceURI */,
									const QString & /* localName */,
									const QString &qName)
{
	if (qName == "title") {
		_playlistName << _currentText;
	} else if (qName == "link") {
		_playlistUrl << _currentText;
	}

	return true;
}

bool WebPlaylistHandler::characters(const QString &str)
{
	_currentText += str;
	return true;
}
