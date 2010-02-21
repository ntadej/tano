/****************************************************************************
* TimersGenerator.cpp: Generator and writer of tano timers format
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

#include <QDate>
#include <QTime>

#include "TimersGenerator.h"

TanoGenerator::TanoGenerator(QTreeWidget *treeWidget, QMap<QTreeWidgetItem*,Timer*> map)
	: _treeWidget(treeWidget), _map(map)
{

}

bool TanoGenerator::write(QIODevice *device)
{
	_out.setDevice(device);
	_out.setCodec("UTF-8");
	_out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<!DOCTYPE tano>\n"
		<< "<tano version=\"0.1\">\n";
	for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i)
		generateItem(_treeWidget->topLevelItem(i), 1);
	_out << "</tano>\n";
	return true;
}

QString TanoGenerator::indent(const int &indentLevel)
{
	const int IndentSize = 4;
	return QString(IndentSize * indentLevel, ' ');
}

QString TanoGenerator::escapedText(const QString &str)
{
	QString result = str;
	result.replace("&", "&amp;");
	result.replace("<", "&lt;");
	result.replace(">", "&gt;");
	return result;
}

QString TanoGenerator::escapedAttribute(const QString &str)
{
	QString result = escapedText(str);
	result.replace("\"", "&quot;");
	result.prepend("\"");
	result.append("\"");
	return result;
}

QString TanoGenerator::boolToString(const bool &b)
{
	if(b) {
		return QString("true");
	} else {
		return QString("false");
	}
}

void TanoGenerator::generateItem(QTreeWidgetItem *item, const int &depth)
{
	Timer *currentTimer = _map[item];

	_out << indent(depth) << "<timer>\n"
		<< indent(depth + 1) << "<name disabled=\"" << boolToString(currentTimer->isDisabled())
							 << "\">" << escapedText(currentTimer->name())
							 << "</name>\n"
		<< indent(depth + 1) << "<channel>" << escapedText(currentTimer->channel())
							 << "</channel>\n"
		<< indent(depth + 1) << "<playlist channelId=\"" << escapedText(QString().number(currentTimer->num()))
							 << "\">" << escapedText(currentTimer->playlist())
							 << "</playlist>\n"
		<< indent(depth + 1) << "<url>" << escapedText(currentTimer->url())
							 << "</url>\n"
		<< indent(depth + 1) << "<date>" << escapedText(currentTimer->date().toString(Qt::ISODate))
							 << "</date>\n"
		<< indent(depth + 1) << "<start>" << escapedText(currentTimer->startTime().toString(Qt::ISODate))
							 << "</start>\n"
		<< indent(depth + 1) << "<end>" << escapedText(currentTimer->endTime().toString(Qt::ISODate))
							 << "</end>\n"
		<< indent(depth) << "</timer>\n";
}
