/****************************************************************************
* tanohandler.cpp: Old reader and handler of tano playlist format
* This file is also part of the example classes of the Qt Toolkit.
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
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

#include <QtCore/QDebug>

#include "tanohandler.h"

TanoHandler::TanoHandler()
{
	num = 0;
	cat = 0;
	metTanoTag = false;
	playlistName = QObject::tr("Channel list");
	category = "";
}

bool TanoHandler::startElement(const QString & /* namespaceURI */,
							   const QString & /* localName */,
							   const QString &qName,
							   const QXmlAttributes &attributes)
{
	if (!metTanoTag && qName != "tano") {
		errorStr = QObject::tr("The file is not a Tano TV channel list file.");
		return false;
	}

	if (qName == "tano") {
		QString version = attributes.value("version");
		if (!version.isEmpty() && !(version == "1.1" || version == "1.2" || version == "1.3")) {
			errorStr = QObject::tr("The file is not a Tano TV channel list 1.1 or later.");
			return false;
		}
		metTanoTag = true;
	} else if (qName == "category") {
		if (attributes.value("type") == "main") {
			cat++;
			num = channelNumSync(cat);
		}
		channel = createChannel(QObject::tr("Unknown title"), cat, true);
	} else if (qName == "channel") {
		num++;
		channel = createChannel(QObject::tr("Unknown title"), num, false);
		_channelList << channel;
	}

	currentText.clear();
	return true;
}

bool TanoHandler::endElement(const QString & /* namespaceURI */,
							 const QString & /* localName */,
							 const QString &qName)
{
	if (qName == "title") {
		if (channel) {
			if(channel->isCategory())
				category = currentText;
			channel->setName(currentText);
		}
	} else if (qName == "epg") {
		if (channel) {
			channel->setEpg(currentText);
		}
	} else if (qName == "language") {
		if (channel) {
			channel->setLanguage(currentText);
		}
	} else if (qName == "url") {
		if (channel) {
			channel->setUrl(currentText);
		}
	} else if (qName == "category" || qName == "channel") {
	   // item = item->parent();
	} else if (qName == "playlist") {
		playlistName = currentText;
	}
	return true;
}

bool TanoHandler::characters(const QString &str)
{
	currentText += str;
	return true;
}

bool TanoHandler::fatalError(const QXmlParseException &exception)
{
	qDebug() << QString("Parse error at line %1, column %2: %3")
							 .arg(exception.lineNumber())
							 .arg(exception.columnNumber())
							 .arg(exception.message());
	return false;
}

QString TanoHandler::errorString() const
{
	return errorStr;
}

Channel *TanoHandler::createChannel(const QString &name, const int &num, const bool &cat)
{
	Channel *childChannel = new Channel(name,num, cat);
	return childChannel;
}

int TanoHandler::channelNumSync(const int &c)
{
	if(c>1) {
		int i=c*100;
		while(i<num)
			i+=100;
		return i-1;
	} else {
		return 0;
	}
}
