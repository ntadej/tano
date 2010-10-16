/****************************************************************************
* TimersGenerator.h: Generator and writer of tano timers format
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
