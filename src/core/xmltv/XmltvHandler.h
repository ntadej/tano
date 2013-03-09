/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_XMLTVHANDLER_H_
#define TANO_XMLTVHANDLER_H_

#include <QtXml/QXmlDefaultHandler>

class XmltvChannel;
class XmltvProgramme;
class XmltvSql;

/*!
    \class XmltvHandler XmltvHandler.h core/xmltv/XmltvHandler.h
    \brief XMLTV parser

    A basic parser that parses XMLTV files and stores data in a database
*/
class XmltvHandler : public QXmlDefaultHandler
{
public:
    XmltvHandler(XmltvSql *db);
    ~XmltvHandler();

    bool startElement(const QString &namespaceURI,
                      const QString &localName,
                      const QString &qName,
                      const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI,
                    const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);

    inline QString errorString() const { return _errorStr; }

private:
    QString _currentText;
    QString _errorStr;
    bool _metTag;

    XmltvSql *_db;

    XmltvChannel *_currentChannel;
    XmltvProgramme *_currentProgramme;
};

#endif // TANO_XMLTVHANDLER_H_
