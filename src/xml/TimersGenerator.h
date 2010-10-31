/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This file is also part of the example classes of the Qt Toolkit.
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
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_TIMERSGENERATOR_H_
#define TANO_TIMERSGENERATOR_H_

#include <QtCore/QMap>
#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "container/Timer.h"

class TimersGenerator
{
public:
	TimersGenerator(QTreeWidget *treeWidget,
					QMap<QTreeWidgetItem*,Timer*> map);
	~TimersGenerator();

	bool write(QIODevice *device);

private:
	static QString indent(const int &indentLevel);
	QString boolToString(const bool &b);
	static QString escapedText(const QString &str);
	static QString escapedAttribute(const QString &str);
	void generateItem(QTreeWidgetItem *item, const int &depth);

	QTreeWidget *_treeWidget;
	QTextStream _out;

	QMap<QTreeWidgetItem*,Timer*> _map;
};

#endif // TANO_TIMERSGENERATOR_H_
