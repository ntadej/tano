/****************************************************************************
**
** Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** Copyright (C) 2008-2010 Tadej Novak
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

#ifndef TANO_TIMERSGENERATOR_H_
#define TANO_TIMERSGENERATOR_H_

#include <QtCore/QMap>
#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "../recorder/Timer.h"

class TanoGenerator
{
public:
	TanoGenerator(QTreeWidget *treeWidget, QMap<QTreeWidgetItem*,Timer*> map);

	bool write(QIODevice *device);

private:
	static QString indent(int indentLevel);
	QString boolToString(const bool &b);
	static QString escapedText(const QString &str);
	static QString escapedAttribute(const QString &str);
	void generateItem(QTreeWidgetItem *item, int depth);

	QTreeWidget *treeWidget;
	QTextStream out;

	QMap<QTreeWidgetItem*,Timer*> _map;
};

#endif // TANO_TIMERSGENERATOR_H_
