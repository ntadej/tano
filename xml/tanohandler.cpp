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

#include "tanohandler.h"

TanoHandler::TanoHandler(QTreeWidget *treeWidget, bool editable)
    : treeWidget(treeWidget)
{
	num = 0;
	cat = 0;
	edit = editable;
    item = 0;
    metTanoTag = false;
    name = QObject::tr("Channel list");
    treeStyle();
}

void TanoHandler::clear()
{
	num = 0;
	cat = 0;
    item = 0;
    limits.clear();
}

void TanoHandler::treeStyle()
{
	categoryIcon.addPixmap(QPixmap(":/icons/images/folder.png"),
	                     QIcon::Normal, QIcon::Off);
	categoryIcon.addPixmap(QPixmap(":/icons/images/folder_video.png"),
	                     QIcon::Normal, QIcon::On);
	channelIcon = QIcon(":/icons/images/video.png");
}

bool TanoHandler::startElement(const QString & /* namespaceURI */,
                               const QString & /* localName */,
                               const QString &qName,
                               const QXmlAttributes &attributes)
{
    if (!metTanoTag && qName != "tano") {
        errorStr = QObject::tr("The file is not a Tano TV channel list file.");
        return false;
    }

    if (qName == "tano") {
        QString version = attributes.value("version");
        if (!version.isEmpty() && !(version == "1.1" || version == "1.2" || version == "1.3")) {
            errorStr = QObject::tr("The file is not a Tano TV channel list 1.1 or later.");
            return false;
        }
        metTanoTag = true;
    } else if (qName == "category") {
    	if (attributes.value("type") == "main") {
    		cat++;
    		num = channelNumSync(cat);
    	}
        item = createChildItem(qName);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setIcon(0, categoryIcon);
        item->setText(0, QObject::tr("Category"));
        if (attributes.value("expanded") == "true")
        	treeWidget->setItemExpanded(item, true);
        else
        	treeWidget->setItemExpanded(item, false);
        channel = createChannel(QObject::tr("Unknown title"), cat, true);
    } else if (qName == "channel") {
    	num++;
        item = createChildItem(qName);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        item->setIcon(0, channelIcon);
        item->setText(0, QObject::tr("Unknown title"));
        channel = createChannel(QObject::tr("Unknown title"), num, false);
        nmap.insert(num, channel);
    } else if (qName == "separator") {
        item = createChildItem(qName);
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setText(0, QString(30, 0xB7));
    }

    currentText.clear();
    return true;
}

bool TanoHandler::endElement(const QString & /* namespaceURI */,
                             const QString & /* localName */,
                             const QString &qName)
{
    if (qName == "title") {
        if (item && channel) {
        	if(!edit && !channel->isCategory())
        		item->setText(0, channel->numToString() + ". " + currentText);
        	else
        		item->setText(0, currentText);
            channel->setName(currentText);
            map.insert(item, channel);
        }
    } else if (qName == "epg") {
        if (item && channel) {
            channel->setEpg(currentText);
            if(edit) item->setText(2, currentText);
        }
    } else if (qName == "language") {
        if (item && channel) {
        	channel->setLanguage(currentText);
        	if(edit) item->setText(1, currentText);
        }
    } else if (qName == "url") {
    	if (item && channel) {
    		channel->setUrl(currentText);
    		if(edit) item->setText(3, currentText);
    	}
    } else if (qName == "category" || qName == "channel") {
        item = item->parent();
    } else if (qName == "playlist") {
    	name = currentText;
    }
    return true;
}

bool TanoHandler::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool TanoHandler::fatalError(const QXmlParseException &exception)
{
    QMessageBox::information(treeWidget->window(), QObject::tr("Tano"),
                             QObject::tr("Parse error at line %1, column %2:\n"
                                         "%3")
                             .arg(exception.lineNumber())
                             .arg(exception.columnNumber())
                             .arg(exception.message()));
    return false;
}

QString TanoHandler::errorString() const
{
    return errorStr;
}

QTreeWidgetItem *TanoHandler::createChildItem(const QString &tagName)
{
    QTreeWidgetItem *childItem;
    if (item) {
        childItem = new QTreeWidgetItem(item);
    } else {
        childItem = new QTreeWidgetItem(treeWidget);
    }
    childItem->setData(0, Qt::UserRole, tagName);
    return childItem;
}

Channel *TanoHandler::createChannel(QString name, int num, bool cat)
{
    Channel *childChannel = new Channel(name,num, cat);
    return childChannel;
}

Channel *TanoHandler::channelRead(QTreeWidgetItem *clickedItem)
{
	Channel *newChannel = map[clickedItem];

	return newChannel;
}

Channel *TanoHandler::channelReadNum(int clickedItem)
{
	Channel *newChannel = nmap[clickedItem];

	return newChannel;
}

int TanoHandler::channelNumSync(int c)
{
	if(c>1) {
		limits.append(num);
		int i=c*100;
		while(i<num)
			i+=100;
		limits.append(i);
		return i-1;
	} else {
		limits.append(num+1);
		return 0;
	}
}

QList<int> TanoHandler::limit()
{
	limits.append(num);
	return limits;
}

QString TanoHandler::getName()
{
	return name;
}
