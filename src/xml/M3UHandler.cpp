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
#include <QtGui/QMessageBox>

#include "M3UHandler.h"
#include "tanohandler.h"

M3UHandler::M3UHandler(QTreeWidget *treeWidget)
	: _treeWidget(treeWidget)
{
	_name = QObject::tr("Channel list");
	_channelIcon = QIcon(":/icons/images/video.png");
}

M3UHandler::~M3UHandler() {
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

int M3UHandler::processNewNum(QTreeWidgetItem *channel, const int &num)
{
	if(_channelNums.contains(num)) {
		QMessageBox::warning(_treeWidget, QObject::tr("Tano"),
							QObject::tr("A channel with this number already exists!"));
		return channelRead(channel)->num();
	}

	_channelNums.removeAll(channelRead(channel)->num());
	_channelNums.append(num);

	channelRead(channel)->setNum(num);
	channel->setText(0,processNum(QString().number(num)));

	return num;
}

void M3UHandler::clear()
{
	_name = QObject::tr("Channel list");
	_m3uLineList.clear();
	_channelNums.clear();
	_categoryList.clear();

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

	if(!_m3uLineList.at(0).contains("#EXTM3U"))
		return;

	for(int i=1; i<_m3uLineList.size(); i++) {
		if(_m3uLineList.at(i)=="")
			continue;

		if(_m3uLineList.at(i).contains("#EXTNAME")) {
			_name = _m3uLineList.at(i);
			_name.replace(QString("#EXTNAME:"),QString(""));
		} else if(_m3uLineList.at(i).contains("#EXTINF")) {
			tmp = _m3uLineList.at(i);
			tmp.replace(QString("#EXTINF:"),QString(""));
			tmpList = tmp.split(",");

			_item = new QTreeWidgetItem(_treeWidget);
			_item->setData(0, Qt::UserRole, "channel");
			_item->setIcon(0, _channelIcon);
			_item->setText(0, processNum(tmpList.at(0)));
			_item->setText(1, tmpList.at(1));
			_channel = new Channel(tmpList.at(1), tmpList.at(0).toInt(), false);

			_map.insert(_item, _channel);
			_nmap.insert(tmpList.at(0).toInt(), _channel);

			_channels << _channel;
			_channelNums << tmpList.at(0).toInt();
		} else if(_m3uLineList.at(i).contains("#EXTTV")) {
			tmp = _m3uLineList.at(i);
			tmp.replace(QString("#EXTTV:"),QString(""));
			tmpList = tmp.split(";");

			if(tmpList.size()!=0) {
				tmpCList = tmpList.at(0).split(",");
				_channel->setCategoryList(tmpCList);
				for(int i=0;i<tmpCList.size();i++)
					if(!_categoryList.contains(tmpCList.at(i)))
						_categoryList.append(tmpCList.at(i));
				_item->setText(2, tmpList.at(0));
				_channel->setLanguage(tmpList.at(1));
				_channel->setEpg(tmpList.at(2));
			}
		} else {
			tmp = _m3uLineList.at(i);
			_channel->setUrl(tmp);
		}
	}
}

Channel *M3UHandler::channelRead(QTreeWidgetItem *clickedItem)
{
	return _map[clickedItem];
}

Channel *M3UHandler::channelRead(const int &clickedItem)
{
	return _nmap[clickedItem];
}

QTreeWidgetItem *M3UHandler::createChannel()
{
	int tmpNum;
	for(int i=1; i<1000; i++) {
		if(!_channelNums.contains(i)) {
			tmpNum = i;
			break;
		}
	}

	_item = new QTreeWidgetItem(_treeWidget);
	_item->setData(0, Qt::UserRole, "channel");
	_item->setIcon(0, _channelIcon);
	_item->setText(0, processNum(QString().number(tmpNum)));
	_item->setText(1, QObject::tr("New channel"));

	_channel = new Channel(QObject::tr("New channel"), tmpNum, false);

	_map.insert(_item, _channel);
	_nmap.insert(tmpNum, _channel);

	_channels << _channel;
	_channelNums << tmpNum;

	return _item;
}

void M3UHandler::deleteChannel(QTreeWidgetItem *i)
{
	_channels.removeAll(_map[i]);
	_channelNums.removeAll(_map[i]->num());

	_nmap.remove(_map[i]->num());
	delete _map[i];
	_map.remove(i);
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
		_channels << import->channelList()[i];
		_channelNums << import->channelList()[i]->num();

		_item = new QTreeWidgetItem(_treeWidget);
		_item->setData(0, Qt::UserRole, "channel");
		_item->setIcon(0, _channelIcon);
		_item->setText(0, processNum(QString().number(import->channelList()[i]->num())));
		_item->setText(1, import->channelList()[i]->name());
		_item->setText(2, import->channelList()[i]->categoryList().join(","));

		_map.insert(_item, import->channelList()[i]);
		_nmap.insert(import->channelList()[i]->num(), import->channelList()[i]);
	}

	_name = import->name();

	delete import;
}
