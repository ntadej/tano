/****************************************************************************
* TimersHandler.cpp: Reader and handler of tano timers format
* This file is also part of the example classes of the Qt Toolkit.
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
* Contact: Qt Software Information (qt-info@nokia.com)
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtGui/QMessageBox>

#include "TimersHandler.h"

TimersHandler::TimersHandler(QTreeWidget *treeWidget)
	: _treeWidget(treeWidget),
	_item(0),
	_timer(0),
	_metTanoTag(false),
	_timerIcon(QIcon(":/icons/images/clock.png")) { }

TimersHandler::~TimersHandler()
{
	for(int i=0; i < _treeWidget->topLevelItemCount(); i++)
		delete _map.value(_treeWidget->topLevelItem(i));
}

void TimersHandler::clear()
{
	for(int i=0; i < _treeWidget->topLevelItemCount(); i++)
		delete _map.value(_treeWidget->topLevelItem(i));

	_treeWidget->clear();
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
	} else if(qName == "playlist") {
		if(_item && _timer)
			_timer->setNum(attributes.value("channelId").toInt());
	} else if(qName == "name") {
		if(_item && _timer) {
			if(attributes.value("disabled") == "true")
				_timer->setDisabled(true);
			else
				_timer->setDisabled(false);
		}
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
			_item->setText(1, QObject::tr("Active"));
			_timer->setName(_currentText);
			_map.insert(_item, _timer);
		}
	} else if (qName == "channel") {
		if (_item && _timer)
			_timer->setChannel(_currentText);
	} else if (qName == "playlist") {
		if (_item && _timer)
			_timer->setPlaylist(_currentText);
	} else if (qName == "url") {
		if (_item && _timer)
			_timer->setUrl(_currentText);
	} else if (qName == "date") {
		if (_item && _timer) {
			_timer->setDate(QDate::fromString(_currentText,Qt::ISODate));
			if(QDate::fromString(_currentText,Qt::ISODate) < QDate::currentDate()) {
				_item->setText(1, QObject::tr("Disabled or expired"));
				_timer->setDisabled(true);
			}
		}
	} else if (qName == "start") {
		if (_item && _timer)
			_timer->setStartTime(QTime::fromString(_currentText,Qt::ISODate));
	} else if (qName == "end") {
		if (_item && _timer) {
			_timer->setEndTime(QTime::fromString(_currentText,Qt::ISODate));
			if(QTime::fromString(_currentText,Qt::ISODate) < QTime::currentTime()) {
				_item->setText(1, QObject::tr("Disabled or expired"));
				_timer->setDisabled(true);
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
	for(int i=0; i<_treeWidget->topLevelItemCount(); i++)
		if(_treeWidget->topLevelItem(i)->text(0) == item->name())
			return _treeWidget->topLevelItem(i);
}

QTreeWidgetItem *TimersHandler::newTimer(const QString &name, const QString &channel,
										 const QString &playlist, const int &num,
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

void TimersHandler::deleteItem(QTreeWidgetItem *item)
{
	Timer *tmp = _map[item];
	_map.remove(_item);
	delete tmp;
	delete _item;
}
