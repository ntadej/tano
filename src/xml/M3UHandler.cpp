/****************************************************************************
* M3UHandler.cpp: Reader and handler of modified m3u format
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
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

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "M3UHandler.h"
#include "tanohandler.h"

M3UHandler::M3UHandler(QTreeWidget *treeWidget, const bool &editable)
	: treeWidget(treeWidget), edit(editable)
{
	name = QObject::tr("Channel list");
	channelIcon = QIcon(":/icons/images/video.png");
}

M3UHandler::~M3UHandler() {
	for(int i=0; i<treeWidget->topLevelItemCount(); i++) {
		delete map.value(treeWidget->topLevelItem(i));
	}
}

void M3UHandler::processFile(const QString &m3uFile)
{
	QFile file(m3uFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	while (!in.atEnd()) {
		m3uLineList << in.readLine();
	}

	processList();
}

QString M3UHandler::processNum(const QString &num)
{
	QString newNum = "000";
	newNum.append(num);
	newNum.remove(0,num.size());

	return newNum;
}

void M3UHandler::clear()
{
	name = QObject::tr("Channel list");
	m3uLineList.clear();
	channelNums.clear();
	categoryList.clear();

	if(treeWidget->topLevelItemCount()>0)
		for(int i=0; i<treeWidget->topLevelItemCount(); i++)
			delete map.value(treeWidget->topLevelItem(i));

	map.clear();
	nmap.clear();
	treeWidget->clear();
}

void M3UHandler::processList()
{
	QString tmp;
	QStringList tmpList;
	QStringList tmpCList;

	if(!m3uLineList.at(0).contains("#EXTM3U"))
		return;

	for(int i=1; i<m3uLineList.size(); i++) {
		if(m3uLineList.at(i)=="")
			continue;

		if(m3uLineList.at(i).contains("#EXTNAME")) {
			name = m3uLineList.at(i);
			name.replace(QString("#EXTNAME:"),QString(""));
		} else if(m3uLineList.at(i).contains("#EXTINF")) {
			tmp = m3uLineList.at(i);
			tmp.replace(QString("#EXTINF:"),QString(""));
			tmpList = tmp.split(",");

			item = new QTreeWidgetItem(treeWidget);
			item->setData(0, Qt::UserRole, "channel");
			item->setIcon(0, channelIcon);
			item->setText(0, processNum(tmpList.at(0)));
			item->setText(1, tmpList.at(1));
			channel = new Channel(tmpList.at(1), tmpList.at(0).toInt(), false);

			map.insert(item, channel);
			nmap.insert(tmpList.at(0).toInt(), channel);

			channels << channel;
			channelNums << tmpList.at(0).toInt();
		} else if(m3uLineList.at(i).contains("#EXTTV")) {
			tmp = m3uLineList.at(i);
			tmp.replace(QString("#EXTTV:"),QString(""));
			tmpList = tmp.split(";");

			if(tmpList.size()!=0) {
				tmpCList = tmpList.at(0).split(",");
				channel->setCategoryList(tmpCList);
				for(int i=0;i<tmpCList.size();i++)
					if(!categoryList.contains(tmpCList.at(i)))
						categoryList.append(tmpCList.at(i));
				item->setText(2, tmpList.at(0));
				channel->setLanguage(tmpList.at(1));
				channel->setEpg(tmpList.at(2));
			}
		} else {
			tmp = m3uLineList.at(i);
			channel->setUrl(tmp);
		}
	}
}

Channel *M3UHandler::channelRead(QTreeWidgetItem *clickedItem)
{
	return map[clickedItem];
}

Channel *M3UHandler::channelRead(const int &clickedItem)
{
	return nmap[clickedItem];
}

QTreeWidgetItem *M3UHandler::createChannel()
{
	int tmpNum;
	for(int i=1; i<1000; i++) {
		if(!channelNums.contains(i)) {
			tmpNum = i;
			break;
		}
	}

	item = new QTreeWidgetItem(treeWidget);
	item->setData(0, Qt::UserRole, "channel");
	item->setIcon(0, channelIcon);
	item->setText(0, processNum(QString().number(tmpNum)));
	item->setText(1, QObject::tr("New channel"));

	channel = new Channel(QObject::tr("New channel"), tmpNum, false);

	map.insert(item, channel);
	nmap.insert(tmpNum, channel);

	channels << channel;
	channelNums << tmpNum;

	return item;
}

void M3UHandler::deleteChannel(QTreeWidgetItem *i)
{
	channels.removeAll(map[i]);
	channelNums.removeAll(map[i]->num());

	nmap.remove(map[i]->num());
	delete map[i];
	map.remove(i);
	delete i;
}

void M3UHandler::importOldFormat(const QString &tanoFile)
{
	TanoHandler *import = new TanoHandler();

	QXmlSimpleReader reader;
	reader.setContentHandler(import);
	reader.setErrorHandler(import);

	QFile file(tanoFile);
	if (!file.open(QFile::ReadOnly | QFile::Text))
		return;

	QXmlInputSource xmlInputSource(&file);
	if (!reader.parse(xmlInputSource))
		return;

	for(int i=0; i<import->channelList().size(); i++) {
		channels << import->channelList()[i];
		channelNums << import->channelList()[i]->num();

		item = new QTreeWidgetItem(treeWidget);
		item->setData(0, Qt::UserRole, "channel");
		item->setIcon(0, channelIcon);
		item->setText(0, processNum(QString().number(import->channelList()[i]->num())));
		item->setText(1, import->channelList()[i]->name());
		item->setText(2, import->channelList()[i]->categoryList().join(","));

		map.insert(item, import->channelList()[i]);
		nmap.insert(import->channelList()[i]->num(), import->channelList()[i]);
	}

	name = import->name();

	delete import;
}
