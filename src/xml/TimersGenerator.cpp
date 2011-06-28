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

#include <QtCore/QDate>
#include <QtCore/QTime>

#include "TimersGenerator.h"
#include "container/Timer.h"

TimersGenerator::TimersGenerator(QTreeWidget *treeWidget,
								 QMap<QTreeWidgetItem *, Timer *> map)
	: _treeWidget(treeWidget),
	_map(map) { }

TimersGenerator::~TimersGenerator() { }

bool TimersGenerator::write(QIODevice *device)
{
	_out.setDevice(device);
	_out.setCodec("UTF-8");
	_out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		 << "<!DOCTYPE tano>\n"
		 << "<tano version=\"0.8\">\n";
	for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i)
		generateItem(_treeWidget->topLevelItem(i));
	_out << "</tano>\n";
	return true;
}

QString TimersGenerator::indent(const int &indentLevel)
{
	const int IndentSize = 4;
	return QString(IndentSize * indentLevel, ' ');
}

QString TimersGenerator::escapedText(const QString &str)
{
	QString result = str;
	result.replace("&", "&amp;");
	result.replace("<", "&lt;");
	result.replace(">", "&gt;");
	return result;
}

QString TimersGenerator::escapedAttribute(const QString &str)
{
	QString result = escapedText(str);
	result.replace("\"", "&quot;");
	result.prepend("\"");
	result.append("\"");
	return result;
}

QString TimersGenerator::boolToString(const bool &b)
{
	if(b) {
		return QString("true");
	} else {
		return QString("false");
	}
}

void TimersGenerator::generateItem(QTreeWidgetItem *item)
{
	Timer *currentTimer = _map[item];
	int depth = 1;

	_out << indent(depth) << "<timer>\n"
		 << indent(depth + 1) << "<name>" << escapedText(currentTimer->name()) << "</name>\n"
		 << indent(depth + 1) << "<playlist>" << escapedText(currentTimer->playlist()) << "</playlist>\n"
		 << indent(depth + 1) << "<channelid>" << escapedText(QString().number(currentTimer->num())) << "</channelid>\n"
		 << indent(depth + 1) << "<channel>" << escapedText(currentTimer->channel()) << "</channel>\n"
		 << indent(depth + 1) << "<url>" << escapedText(currentTimer->url()) << "</url>\n"
         << indent(depth + 1) << "<type>" << escapedText(QString().number(currentTimer->type())) << "</type>\n"
		 << indent(depth + 1) << "<starttime>" << escapedText(currentTimer->startTime().toString(Qt::ISODate)) << "</starttime>\n"
		 << indent(depth + 1) << "<endtime>" << escapedText(currentTimer->endTime().toString(Qt::ISODate)) << "</endtime>\n"
		 << indent(depth + 1) << "<disabled>" << boolToString(currentTimer->isDisabled()) << "</disabled>\n"
		 << indent(depth) << "</timer>\n";
}
