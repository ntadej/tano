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

#include "TimersHandler.h"
#include "container/Timer.h"

TimersHandler::TimersHandler(QTreeWidget *treeWidget)
	: _treeWidget(treeWidget),
	_item(0),
	_timer(0),
	_metTanoTag(false),
	_timerIcon(QIcon(":/icons/16x16/timer.png")) { }

TimersHandler::~TimersHandler()
{
	clear();
}

void TimersHandler::clear()
{
	if(_treeWidget) {
		for(int i = 0; i < _treeWidget->topLevelItemCount(); i++)
			delete _map.value(_treeWidget->topLevelItem(i));

		_treeWidget->clear();
	}
	_map.clear();
}

bool TimersHandler::startElement(const QString & /* namespaceURI */,
								 const QString & /* localName */,
								 const QString &qName,
								 const QXmlAttributes &attributes)
{
	if (!_metTanoTag && qName != "tano") {
		_errorStr = QObject::tr("The file is not a Tano Timers list.");
		return false;
	}

	if(qName == "tano") {
		_metTanoTag = true;
	} else if(qName == "timer") {
		_item = new QTreeWidgetItem(_treeWidget);
		_item->setIcon(0, _timerIcon);
		_item->setText(0, QObject::tr("Unknown title"));
		_timer = new Timer(QObject::tr("Unknown title"));
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
		if (_item && _timer) {
			_item->setText(0, _currentText);
			_item->setText(2, QObject::tr("Active"));
			_timer->setName(_currentText);
			_map.insert(_item, _timer);
		}
	} else if (qName == "playlist") {
		if (_item && _timer) {
			_timer->setPlaylist(_currentText);
		}
	} else if (qName == "channelid") {
		if (_item && _timer) {
			_timer->setNum(_currentText.toInt());
		}
	} else if (qName == "channel") {
		if (_item && _timer) {
			_timer->setChannel(_currentText);
		}
	} else if (qName == "url") {
		if (_item && _timer) {
			_timer->setUrl(_currentText);
		}
	} else if (qName == "type") {
		if (_item && _timer) {
            _timer->setType(Tano::TimerType(_currentText.toInt()));
			_item->setText(1, Tano::timerTypeString(_timer->type()));
		}
	} else if (qName == "starttime") {
		if (_item && _timer) {
			_timer->setStartTime(QDateTime::fromString(_currentText, Qt::ISODate));
		}
	} else if (qName == "endtime") {
		if (_item && _timer) {
			_timer->setEndTime(QDateTime::fromString(_currentText, Qt::ISODate));
			/*if(_timer->endTime() < QDateTime::currentDateTime()) {
				_item->setText(2, QObject::tr("Disabled or expired"));
				//_timer->setDisabled(true);
			}*/
		}
	} else if (qName == "disabled") {
		if (_item && _timer) {
			if(_currentText == "true") {
				_timer->setDisabled(true);
			} else {
				_timer->setDisabled(false);
			}
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
	QMessageBox::information(_treeWidget->window(), QObject::tr("Tano"),
							 QObject::tr("Parse error at line %1, column %2:\n"
										 "%3")
							 .arg(exception.lineNumber())
							 .arg(exception.columnNumber())
							 .arg(exception.message()));
	return false;
}


QTreeWidgetItem *TimersHandler::itemRead(Timer *item)
{
	if(_treeWidget) {
		for(int i = 0; i < _treeWidget->topLevelItemCount(); i++)
			if(_treeWidget->topLevelItem(i)->text(0) == item->name())
				return _treeWidget->topLevelItem(i);
	} else {
		return new QTreeWidgetItem();
	}
}

QTreeWidgetItem *TimersHandler::newTimer(const QString &name,
										 const QString &channel,
										 const QString &playlist,
										 const int &num,
										 const QString &url)
{
	_item = new QTreeWidgetItem(_treeWidget);
	_item->setIcon(0, _timerIcon);
	_item->setText(0, name);
	_item->setText(1, QObject::tr("Active"));

	_timer = new Timer(name, channel, playlist, num, url);
	_map.insert(_item, _timer);

	return _item;
}

void TimersHandler::deleteItem(Timer *item)
{
	QTreeWidgetItem *tmp = itemRead(item);
	_list.removeAll(item);
	_map.remove(tmp);
	delete tmp;
	delete item;
}

void TimersHandler::deleteItem(QTreeWidgetItem *item)
{
	Timer *tmp = _map[item];
	_list.removeAll(tmp);
	_map.remove(item);
	delete tmp;
	delete item;
}
