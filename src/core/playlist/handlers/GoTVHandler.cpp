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

#include <QtCore/QDebug>

#include "playlist/containers/Channel.h"
#include "GoTVHandler.h"

GoTVHandler::GoTVHandler()
    : _metTanoTag(false) { }

GoTVHandler::~GoTVHandler() { }

bool GoTVHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    Q_UNUSED(attributes)

    if(qName == "channels")
        _metTanoTag = true;

    if (!_metTanoTag && qName != "channels") {
        _errorStr = QObject::tr("The file is not a GoTV channel list file.");
        return false;
    }

    if (qName == "channel") {
        _channel = new Channel(QObject::tr("Unknown title"), 0);
        _channelList << _channel;
    }

    _currentText.clear();
    return true;
}

bool GoTVHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    if (qName == "name") {
        if (_channel) {
            _channel->setName(_currentText.replace(QRegExp("^[^\\.]*\\.\\s"), ""));
        }
    } else if (qName == "epg") {
        if (_channel) {
            _channel->setXmltvId(_currentText);
        }
    } else if (qName == "chicon") {
        if (_channel) {
            _channel->setLogo(_currentText);
        }
    } else if (qName == "url") {
        if (_channel) {
            _channel->setUrl(_currentText);
        }
    } else if (qName == "passwordprotected") {
        if (_channel) {
            //_channel->setPasswordProtected((_currentText == "yes") ? true : false);
        }
    }
    return true;
}

bool GoTVHandler::characters(const QString &str)
{
    _currentText += str;
    return true;
}

bool GoTVHandler::fatalError(const QXmlParseException &exception)
{
    qCritical() << QObject::tr("Error:") << QObject::tr("Parse error at line %1, column %2:\n%3")
                                                       .arg(exception.lineNumber())
                                                       .arg(exception.columnNumber())
                                                       .arg(exception.message());
    return false;
}
