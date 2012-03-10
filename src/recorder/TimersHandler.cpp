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

#include <QtGui/QMessageBox>

#include "container/core/Timer.h"
#include "TimersHandler.h"

TimersHandler::TimersHandler()
    : _timer(0),
      _metTanoTag(false) { }

TimersHandler::~TimersHandler() { }

bool TimersHandler::startElement(const QString & /* namespaceURI */,
                                 const QString & /* localName */,
                                 const QString &qName,
                                 const QXmlAttributes &attributes)
{
    Q_UNUSED(attributes)

    if (!_metTanoTag && qName != "tano") {
        _errorStr = QObject::tr("The file is not a Tano Timers list.");
        return false;
    }

    if(qName == "tano") {
        _metTanoTag = true;
    } else if(qName == "timer") {
        _timer = new Timer();
        _list << _timer;
    }

    _currentText.clear();
    return true;
}

bool TimersHandler::endElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName)
{
    if (qName == "name") {
        if (_timer) {
            _timer->setName(_currentText);
        }
    } else if (qName == "playlist") {
        if (_timer) {
            _timer->setPlaylist(_currentText);
        }
    } else if (qName == "channelid") {
        if (_timer) {
            _timer->setNum(_currentText.toInt());
        }
    } else if (qName == "channel") {
        if (_timer) {
            _timer->setChannel(_currentText);
        }
    } else if (qName == "url") {
        if (_timer) {
            _timer->setUrl(_currentText);
        }
    } else if (qName == "starttime") {
        if (_timer) {
            _timer->setStartTime(QDateTime::fromString(_currentText, Qt::ISODate));
        }
    } else if (qName == "endtime") {
        if (_timer) {
            _timer->setEndTime(QDateTime::fromString(_currentText, Qt::ISODate));
        }
    } else if (qName == "type") {
        if (_timer) {
            _timer->setType(Tano::TimerType(_currentText.toInt()));
        }
    } else if (qName == "state") {
        if (_timer) {
            _timer->setState(Tano::TimerState(_currentText.toInt()));
        }
    }

    return true;
}

bool TimersHandler::characters(const QString &str)
{
    _currentText += str;
    return true;
}

bool TimersHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(0, QObject::tr("Tano"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}
