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

#ifndef TANOHANDLER_H
#define TANOHANDLER_H

#include <QIcon>
#include <QMap>
#include <QXmlDefaultHandler>
#include "../channels/Channel.h"

QT_BEGIN_NAMESPACE
class QTreeWidget;
class QTreeWidgetItem;
QT_END_NAMESPACE

class TanoHandler : public QXmlDefaultHandler
{
public:
    TanoHandler(QTreeWidget *treeWidget, bool editable = false);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;

    Channel *channelRead(QTreeWidgetItem *clickedItem);
    Channel *channelReadNum(int clickedItem);

    QList<int> limit();
    void clear();

    QString getName();

private:
	int num;
	int cat;

	bool edit;

    QTreeWidgetItem *createChildItem(const QString &tagName);
    Channel *createChannel(QString name, int num, bool cat);
    void treeStyle();
    int channelNumSync(int c);

    QTreeWidget *treeWidget;
    QTreeWidgetItem *item;
    Channel *channel;
    QString currentText;
    QString errorStr;
    bool metTanoTag;

    QMap<QTreeWidgetItem*, Channel*> map;
    QMap<int, Channel*> nmap;
    QList<int> limits;

    QIcon categoryIcon;
    QIcon channelIcon;

    QString name;
    QString category;
};

#endif
