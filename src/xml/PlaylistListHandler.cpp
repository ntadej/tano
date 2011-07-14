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

#include <QtCore/QLocale>

#include "PlaylistListHandler.h"
#include "container/playlist/PlaylistInfo.h"

PlaylistListHandler::PlaylistListHandler()
	: _metTanoTag(false) { }

PlaylistListHandler::~PlaylistListHandler() { }

bool PlaylistListHandler::startElement(const QString & /* namespaceURI */,
									   const QString & /* localName */,
									   const QString &qName,
									   const QXmlAttributes &attributes)
{
	if (!_metTanoTag && (qName != "playlists" && qName != "channels")) {
		_errorStr = QObject::tr("The file is not a playlist list file.");
		return false;
	}

	if(qName == "playlists" || qName == "channels") {
		_metTanoTag = true;
		_playlistCountries.clear();
		_playlistList.clear();

		if(qName == "channels") {
			QLocale country("sl");
			_playlistCountries << country.countryToString(country.country());
		}
	} else if(qName == "playlist") {
        PlaylistInfo p(_playlistCountries.last());
		_playlistList << p;
	}

	_currentText.clear();
	return true;
}

bool PlaylistListHandler::endElement(const QString & /* namespaceURI */,
									 const QString & /* localName */,
									 const QString &qName)
{
	if (qName == "id") {
		if(_currentText == "int") {
			_playlistCountries << "International";
		} else {
			QLocale country(_currentText);
			_playlistCountries << country.countryToString(country.country());
		}
	} else if (qName == "title") {
		_playlistList.last().setTitle(_currentText);
	} else if (qName == "path") {
		_playlistList.last().setPath(_currentText);
	} else if (qName == "link") {
		_playlistList.last().setPath(_currentText);
	}

	_currentText.clear();
	return true;
}

bool PlaylistListHandler::characters(const QString &str)
{
	_currentText += str;
	return true;
}
