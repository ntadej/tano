/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** Copyright (C) 2008-2009 Tadej Novak
**
** This file is part of the example classes of the Qt Toolkit.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file.  Please review the following information
** to ensure GNU General Public Licensing requirements will be met:
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.  In addition, as a special
** exception, Nokia gives you certain additional rights. These rights
** are described in the Nokia Qt GPL Exception version 1.3, included in
** the file GPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include <QDate>
#include <QTime>

#include "TimersGenerator.h"

TanoGenerator::TanoGenerator(QTreeWidget *treeWidget, QMap<QTreeWidgetItem*,Timer*> map)
	: treeWidget(treeWidget), _map(map)
{

}

bool TanoGenerator::write(QIODevice *device)
{
	out.setDevice(device);
	out.setCodec("UTF-8");
	out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		<< "<!DOCTYPE tano>\n"
		<< "<tano version=\"0.1\">\n";
	for (int i = 0; i < treeWidget->topLevelItemCount(); ++i)
		generateItem(treeWidget->topLevelItem(i), 1);
	out << "</tano>\n";
	return true;
}

QString TanoGenerator::indent(int depth)
{
	const int IndentSize = 4;
	return QString(IndentSize * depth, ' ');
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

void TanoGenerator::generateItem(QTreeWidgetItem *item, int depth)
{
	Timer *currentTimer = _map[item];

	out << indent(depth) << "<timer>\n"
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
