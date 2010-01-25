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

#ifndef TANO_TIMERSHANDLER_H_
#define TANO_TIMERSHANDLER_H_

#include <QIcon>
#include <QMap>
#include <QXmlDefaultHandler>
#include <QTreeWidget>
#include <QTreeWidgetItem>

#include "../recorder/Timer.h"

class TimersHandler : public QXmlDefaultHandler
{
public:
    TimersHandler(QTreeWidget *treeWidget);

    bool startElement(const QString &namespaceURI, const QString &localName,
                      const QString &qName, const QXmlAttributes &attributes);
    bool endElement(const QString &namespaceURI, const QString &localName,
                    const QString &qName);
    bool characters(const QString &str);
    bool fatalError(const QXmlParseException &exception);
    QString errorString() const;

    Timer *timerRead(QTreeWidgetItem *clickedItem);
    QMap<QTreeWidgetItem*, Timer*> timersMap();

    void clear();

public slots:
	QTreeWidgetItem *newTimer(QString name, QString channel, QString playlist, int num);

private:
    QTreeWidgetItem *createChildItem(const QString &tagName);

    QTreeWidget *treeWidget;
    QTreeWidgetItem *item;
    Timer *timer;
    QString currentText;
    QString errorStr;
    bool metTanoTag;

    QMap<QTreeWidgetItem*, Timer*> map;

    QIcon timerIcon;
};

#endif // TANO_TIMERSHANDLER_H_
