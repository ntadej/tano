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

#include <QtGui>

#include "tanogenerator.h"

TanoGenerator::TanoGenerator(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
}

bool TanoGenerator::write(QIODevice *device)
{
    out.setDevice(device);
    out.setCodec("UTF-8");
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
        << "<!DOCTYPE tano>\n"
        << "<tano version=\"1.2\">\n";
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

void TanoGenerator::generateItem(QTreeWidgetItem *item, int depth)
{
    QString tagName = item->data(0, Qt::UserRole).toString();
    if (tagName == "category") {
        bool expanded = !treeWidget->isItemExpanded(item);
        bool type;
        if(treeWidget->indexOfTopLevelItem(item) == -1)
        	type = false;
        else
        	type = true;

        out << indent(depth) << "<category "
							 << "expanded=\"" << (expanded ? "false" : "true") << "\""
							 << "type=\"" << (type ? "main" : "sub")
                             << "\">\n"
            << indent(depth + 1) << "<title>" << escapedText(item->text(0))
                                 << "</title>\n";
        for (int i = 0; i < item->childCount(); ++i)
            generateItem(item->child(i), depth + 1);
        out << indent(depth) << "</category>\n";
    } else if (tagName == "channel") {
        out << indent(depth) << "<channel>\n"
            << indent(depth + 1) << "<title>" << escapedText(item->text(0))
                                 << "</title>\n"
            << indent(depth + 1) << "<epg>" << escapedText(item->text(2))
                                 << "</epg>\n"
            << indent(depth + 1) << "<language>" << escapedText(item->text(1))
                                 << "</language>\n"
            << indent(depth + 1) << "<url>" << escapedText(item->text(3))
                                 << "</url>\n"
            << indent(depth) << "</channel>\n";
    }
}
