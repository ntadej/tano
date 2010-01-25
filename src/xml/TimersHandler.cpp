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

#include <QMessageBox>

#include "TimersHandler.h"

TimersHandler::TimersHandler(QTreeWidget *treeWidget)
    : treeWidget(treeWidget)
{
    item = 0;
    timer = 0;
    metTanoTag = false;
	timerIcon = QIcon(":/icons/images/clock.png");
}

void TimersHandler::clear()
{
//TODO: Clean timers!
}

bool TimersHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    if (!metTanoTag && qName != "tano") {
        errorStr = QObject::tr("The file is not a Tano Timers list.");
        return false;
    }

    if(qName == "tano") {
        metTanoTag = true;
    } else if(qName == "timer") {
        item = new QTreeWidgetItem(treeWidget);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setIcon(0, timerIcon);
        item->setText(0, QObject::tr("Unknown title"));
        timer = new Timer(QObject::tr("Unknown title"));
    } else if(qName == "playlist") {
    	if(item && timer) {
    		timer->setNum(attributes.value("channelId").toInt());
    	}
    }

    currentText.clear();
    return true;
}

bool TimersHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    if (qName == "name") {
        if (item && timer) {
       		item->setText(0,currentText);
            timer->setName(currentText);
            map.insert(item, timer);
        }
    } else if (qName == "channel") {
        if (item && timer) {
            timer->setChannel(currentText);
        }
    } else if (qName == "playlist") {
        if (item && timer) {
        	timer->setPlaylist(currentText);
        }
    } else if (qName == "date") {
    	if (item && timer) {
    		timer->setDate(QDate::fromString(currentText,Qt::ISODate));
    	}
    } else if (qName == "start") {
    	if (item && timer) {
    		timer->setStartTime(QTime::fromString(currentText,Qt::ISODate));
    	}
    } else if (qName == "end") {
    	if (item && timer) {
    		timer->setEndTime(QTime::fromString(currentText,Qt::ISODate));
    	}
    }
    return true;
}

bool TimersHandler::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool TimersHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(treeWidget->window(), QObject::tr("Tano"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}

QString TimersHandler::errorString() const
{
    return errorStr;
}

Timer *TimersHandler::timerRead(QTreeWidgetItem *clickedItem)
{
	return map[clickedItem];
}

QTreeWidgetItem *TimersHandler::newTimer(QString name, QString channel, QString playlist, int num)
{
	item = new QTreeWidgetItem(treeWidget);
	item->setIcon(0, timerIcon);
	item->setFlags(item->flags() | Qt::ItemIsEditable);
	item->setText(0,name);

	timer = new Timer(name, channel, playlist, num);
	map.insert(item, timer);

	return item;
}

QMap<QTreeWidgetItem*, Timer*> TimersHandler::timersMap()
{
	return map;
}
