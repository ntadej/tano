/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include "xmltv/XmltvCommon.h"
#include "xmltv/XmltvSql.h"
#include "xmltv/containers/XmltvChannel.h"
#include "xmltv/containers/XmltvCrewMember.h"
#include "xmltv/containers/XmltvProgramme.h"

#include "XmltvHandler.h"

XmltvHandler::XmltvHandler(XmltvSql *db)
    : _metTag(false),
      _db(db) { }

XmltvHandler::~XmltvHandler() { }

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
        _metTag = true;
    } else if (qName == "channel") { // Channel
        _currentChannel = new XmltvChannel(attributes.value("id"));
    } else if (qName == "icon") {
        if(_currentChannel) {
            _currentChannel->setIcon(attributes.value("src"));
        } else if(_currentProgramme) {
            _currentProgramme->setIcon(attributes.value("src"));
            _currentProgramme->setIconSize(QSize(attributes.value("width").toInt(), attributes.value("height").toInt()));
        }
    } else if (qName == "programme") { // Programme
        QString start = attributes.value("start").replace(Tano::Xmltv::dateRegExp(), "");
        QString stop = attributes.value("stop").replace(Tano::Xmltv::dateRegExp(), "");
        _currentProgramme = new XmltvProgramme(attributes.value("channel"));
        _currentProgramme->setStart(QDateTime::fromString(start, Tano::Xmltv::dateFormat()));
        _currentProgramme->setStop(QDateTime::fromString(stop, Tano::Xmltv::dateFormat()));
        if (_previousProgramme && !_previousProgramme->stop().isValid()) {
            _db->updateProgramme(_previousProgramme->id(), "stop", _currentProgramme->start().toString(Tano::Xmltv::dateFormat()));
            delete _previousProgramme;
        }
    } else if (qName == "lenght") {
        if(_currentProgramme) {
            _currentProgramme->setLengthUnits(XmltvProgramme::lengthUnits(attributes.value("units")));
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
    if(qName == "channel") {
        if(_currentChannel) {
            _db->addChannel(_currentChannel);

            delete _currentChannel;
        }
    } else if(qName == "display-name") {
        if(_currentChannel) {
            _currentChannel->setDisplayName(_currentText);
        }
    } else if(qName == "url") {
        if(_currentChannel) {
            _currentChannel->setUrl(_currentText);
        }
    } else if(qName == "programme") { // Programme
        if(_currentProgramme) {
            _db->addProgramme(_currentProgramme);

            _previousProgramme = _currentProgramme;
            _currentProgramme = 0;
        }
    } else if(qName == "title") {
        if(_currentProgramme) {
            _currentProgramme->setTitle(_currentText);
        }
    } else if(qName == "sub-title") {
        if(_currentProgramme) {
            _currentProgramme->setSubTitle(_currentText);
        }
    } else if(qName == "desc") {
        if(_currentProgramme) {
            _currentProgramme->setDesc(_currentText);
        }
    } else if(qName == "director") { // Programme credits
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Director, _currentProgramme));
        }
    } else if(qName == "actor") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Actor, _currentProgramme));
        }
    } else if(qName == "writer") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Writer, _currentProgramme));
        }
    } else if(qName == "adapter") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Adapter, _currentProgramme));
        }
    } else if(qName == "producer") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Producer, _currentProgramme));
        }
    } else if(qName == "composer") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Composer, _currentProgramme));
        }
    } else if(qName == "editor") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Editor, _currentProgramme));
        }
    } else if(qName == "presenter") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Presenter, _currentProgramme));
        }
    } else if(qName == "commentator") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Commentator, _currentProgramme));
        }
    } else if(qName == "guest") {
        if(_currentProgramme) {
            _db->addCrewMember(new XmltvCrewMember(_currentText, XmltvCrewMember::Guest, _currentProgramme));
        }
    } else if(qName == "date") { // Programme - continuation
        if(_currentProgramme) {
            QString date = _currentText.replace(Tano::Xmltv::dateRegExp(), "");
            _currentProgramme->setDate(QDateTime::fromString(date, Tano::Xmltv::dateFormat()));
        }
    } else if(qName == "category") {
        if(_currentProgramme) {
            _currentProgramme->addCategory(_currentText);
        }
    } else if(qName == "language") {
        if(_currentProgramme) {
            _currentProgramme->setLanguage(_currentText);
        }
    } else if(qName == "orig-language") {
        if(_currentProgramme) {
            _currentProgramme->setOriginalLanguage(_currentText);
        }
    } else if(qName == "lenght") {
        if(_currentProgramme) {
            _currentProgramme->setLength(_currentText);
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
    qCritical() << QObject::tr("Error:") << QObject::tr("Parse error at line %1, column %2: %3")
                                                       .arg(exception.lineNumber())
                                                       .arg(exception.columnNumber())
                                                       .arg(exception.message());
    return false;
}
