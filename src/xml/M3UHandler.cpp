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
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtGui/QMessageBox>

#include "M3UHandler.h"
#include "TanoHandlerOld.h"
#include "core/Settings.h"

M3UHandler::M3UHandler(QTreeWidget *treeWidget)
	: _treeWidget(treeWidget)
{
	_name = QObject::tr("Channel list");
	_epgPlugin = Settings::DEFAULT_EPG_PLUGIN;
	_channelIcon = QIcon(":/icons/128x128/video.png");
}

M3UHandler::~M3UHandler()
{
	for(int i=0; i<_treeWidget->topLevelItemCount(); i++) {
		delete _map.value(_treeWidget->topLevelItem(i));
	}
}

void M3UHandler::processFile(const QString &m3uFile)
{
	QFile file(m3uFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	in.setCodec(QTextCodec::codecForName("UTF-8"));
	while (!in.atEnd()) {
		_m3uLineList << in.readLine();
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

int M3UHandler::processNewNum(QTreeWidgetItem *channel,
							  const int &num)
{
	if(_channelNums.contains(num)) {
		QMessageBox::warning(_treeWidget, QObject::tr("Tano"),
							QObject::tr("A channel with this number already exists!"));
		return channelRead(channel)->number();
	}

	_channelNums.removeAll(channelRead(channel)->number());
	_channelNums.append(num);

	channelRead(channel)->setNumber(num);
	channel->setText(0,processNum(QString().number(num)));

	_treeWidget->sortByColumn(0, Qt::AscendingOrder);

	return num;
}

void M3UHandler::clear()
{
	_name = QObject::tr("Channel list");
	_m3uLineList.clear();
	_channelNums.clear();
	_categoryList.clear();
	_epgList.clear();

	if(_treeWidget->topLevelItemCount()>0)
		for(int i=0; i<_treeWidget->topLevelItemCount(); i++)
			delete _map.value(_treeWidget->topLevelItem(i));

	_map.clear();
	_nmap.clear();
	_treeWidget->clear();
}

void M3UHandler::processList()
{
	QString tmp;
	QStringList tmpList;
	QStringList tmpCList;

	if(!_m3uLineList[0].contains("#EXTM3U"))
		return;

	for(int i=1; i<_m3uLineList.size(); i++) {
		if(_m3uLineList[i]=="")
			continue;

		if(_m3uLineList[i].contains("#EXTNAME")) {
			_name = _m3uLineList[i];
			_name.replace(QString("#EXTNAME:"),QString(""));
		} else if(_m3uLineList[i].contains("#EXTEPG")) {
			_epgPlugin = _m3uLineList[i];
			_epgPlugin.replace(QString("#EXTEPG:"),QString(""));
		} else if(_m3uLineList[i].contains("#EXTINF")) {
			tmp = _m3uLineList[i];
			tmp.replace(QString("#EXTINF:"),QString(""));
			tmpList = tmp.split(",");

			_item = new QTreeWidgetItem(_treeWidget);
			_item->setData(0, Qt::UserRole, "channel");
			_item->setIcon(0, _channelIcon);
			_item->setText(0, processNum(tmpList[0]));
			_item->setText(1, tmpList[1]);
			_channel = new Channel(tmpList[1], tmpList[0].toInt());

			_map.insert(_item, _channel);
			_nmap.insert(tmpList[0].toInt(), _channel);

			_channels << _channel;
			_channelNums << tmpList[0].toInt();
		} else if(_m3uLineList[i].contains("#EXTTV")) {
			tmp = _m3uLineList[i];
			tmp.replace(QString("#EXTTV:"),QString(""));
			tmpList = tmp.split(";");

			if(tmpList.size()!=0) {
				tmpCList = tmpList[0].split(",");
				_channel->setCategories(tmpCList);
				for(int k=0;k<tmpCList.size();k++)
					if(!_categoryList.contains(tmpCList[k]))
						_categoryList << tmpCList[k];
				_item->setText(2, tmpList[0]);
				_channel->setLanguage(tmpList[1]);
				_channel->setEpg(tmpList[2]);
				if(!_epgList.contains(tmpList[2]) && !tmpList[2].isEmpty())
					_epgList << tmpList[2];
				if(tmpList.size()==4)
					_channel->setLogo(tmpList[3]);
			}
		} else {
			tmp = _m3uLineList[i];
			_channel->setUrl(tmp);
		}
	}
}

QTreeWidgetItem *M3UHandler::createChannel(const QString &name,
										   const QString &url)
{
	int tmpNum;
	for(int i=1; i<1000; i++) {
		if(!_channelNums.contains(i)) {
			tmpNum = i;
			break;
		}
	}

	QString cname;
	if(name == 0)
		cname = QObject::tr("New channel");
	else
		cname = name;

	_item = new QTreeWidgetItem(_treeWidget);
	_item->setData(0, Qt::UserRole, "channel");
	_item->setIcon(0, _channelIcon);
	_item->setText(0, processNum(QString().number(tmpNum)));
	_item->setText(1, cname);

	_channel = new Channel(cname, tmpNum);
	if(url != 0)
		_channel->setUrl(url);

	_map.insert(_item, _channel);
	_nmap.insert(tmpNum, _channel);

	_channels << _channel;
	_channelNums << tmpNum;

	return _item;
}

void M3UHandler::deleteChannel(QTreeWidgetItem *i)
{
	_channels.removeAll(_map[i]);
	_channelNums.removeAll(_map[i]->number());

	_nmap.remove(_map[i]->number());
	delete _map[i];
	_map.remove(i);
	delete i;
}

void M3UHandler::importOldFormat(const QString &tanoFile)
{
	TanoHandlerOld *import = new TanoHandlerOld();

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
		_channels << import->channelList()[i];
		_channelNums << import->channelList()[i]->number();

		_item = new QTreeWidgetItem(_treeWidget);
		_item->setData(0, Qt::UserRole, "channel");
		_item->setIcon(0, _channelIcon);
		_item->setText(0, processNum(QString().number(import->channelList()[i]->number())));
		_item->setText(1, import->channelList()[i]->name());
		_item->setText(2, import->channelList()[i]->categories().join(","));

		_map.insert(_item, import->channelList()[i]);
		_nmap.insert(import->channelList()[i]->number(), import->channelList()[i]);
	}

	_name = import->name();

	delete import;
}

void M3UHandler::moveUp(QTreeWidgetItem *channel)
{
	int currentNum = channelRead(channel)->number();

	if(currentNum == 1)
		return;

	int tmpNum;
	int id = _treeWidget->indexOfTopLevelItem(channel);
	QTreeWidgetItem *tmp = _treeWidget->topLevelItem(id-1);
	for(int i=1; i<1000; i++) {
		if(!_channelNums.contains(i)) {
			tmpNum = i;
			break;
		}
	}

	if(channelRead(tmp)->number() == currentNum-1) {
		processNewNum(tmp, tmpNum);
		processNewNum(channel, currentNum-1);
		processNewNum(tmp, currentNum);
	} else {
		processNewNum(channel, currentNum-1);
	}
}

void M3UHandler::moveDown(QTreeWidgetItem *channel)
{
	int currentNum = channelRead(channel)->number();

	if(currentNum == 999)
		return;

	int tmpNum;
	int id = _treeWidget->indexOfTopLevelItem(channel);
	QTreeWidgetItem *tmp = _treeWidget->topLevelItem(id+1);
	for(int i=1; i<1000; i++) {
		if(!_channelNums.contains(i)) {
			tmpNum = i;
			break;
		}
	}

	if(channelRead(tmp)->number() == currentNum+1) {
		processNewNum(tmp, tmpNum);
		processNewNum(channel, currentNum+1);
		processNewNum(tmp, currentNum);
	} else {
		processNewNum(channel, currentNum+1);
	}
}
