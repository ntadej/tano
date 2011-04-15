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

#ifndef TANO_WEBPLAYLISTHANDLER_H_
#define TANO_WEBPLAYLISTHANDLER_H_

#include <QtXml/QXmlDefaultHandler>

class PlaylistFile;

class PlaylistListHandler : public QXmlDefaultHandler
{
public:
	PlaylistListHandler();
	~PlaylistListHandler();

	bool startElement(const QString &namespaceURI,
					  const QString &localName,
					  const QString &qName,
					  const QXmlAttributes &attributes);
	bool endElement(const QString &namespaceURI,
					const QString &localName,
					const QString &qName);
	bool characters(const QString &str);
	QString errorString() const { return _errorStr; }

	QStringList playlistCountries() const { return _playlistCountries; }
	QList<PlaylistFile> playlistList() const { return _playlistList; }

private:
	QString _currentText;
	QString _errorStr;
	bool _metTanoTag;

	QStringList _playlistCountries;
	QList<PlaylistFile> _playlistList;
};

#endif // TANO_WEBPLAYLISTHANDLER_H_
