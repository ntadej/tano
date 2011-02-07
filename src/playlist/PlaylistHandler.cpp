/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QFile>
#include <QtCore/QTextCodec>
#include <QtCore/QTextStream>
#include <QtGui/QMessageBox>

#include "PlaylistHandler.h"
#include "container/Channel.h"
#include "playlist/JsHandler.h"
#include "playlist/M3UHandler.h"
#include "xml/TanoHandlerOld.h"

PlaylistHandler::PlaylistHandler(QTreeWidget *treeWidget)
	: _treeWidget(treeWidget)
{
	_name = QObject::tr("Channel list");
	_channelIcon = QIcon(":/icons/16x16/video.png");
}

PlaylistHandler::~PlaylistHandler()
{
	for(int i = 0; i < _treeWidget->topLevelItemCount(); i++) {
		delete _map.value(_treeWidget->topLevelItem(i));
	}
}

QString PlaylistHandler::processNum(const QString &num)
{
	QString newNum = "000";
	newNum.append(num);
	newNum.remove(0, num.size());

	return newNum;
}

int PlaylistHandler::processNewNum(QTreeWidgetItem *channel,
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
	channel->setText(0, processNum(QString().number(num)));

	_treeWidget->sortByColumn(0, Qt::AscendingOrder);

	return num;
}

void PlaylistHandler::clear()
{
	_name = QObject::tr("Channel list");
	_channelNums.clear();
	_categoryList.clear();
	_languageList.clear();
	_epgList.clear();

	if(_treeWidget->topLevelItemCount() > 0)
		for(int i = 0; i < _treeWidget->topLevelItemCount(); i++)
			delete _map.value(_treeWidget->topLevelItem(i));

	_map.clear();
	_nmap.clear();
	_treeWidget->clear();
}

QTreeWidgetItem *PlaylistHandler::createChannel(const QString &name,
										   const QString &url)
{
	int tmpNum;
	for(int i = 1; i < 1000; i++) {
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

void PlaylistHandler::deleteChannel(QTreeWidgetItem *channel)
{
	_channels.removeAll(_map[channel]);
	_channelNums.removeAll(_map[channel]->number());

	_nmap.remove(_map[channel]->number());
	delete _map[channel];
	_map.remove(channel);
	delete channel;
}

void PlaylistHandler::openM3UFile(const QString &m3uFile)
{
	M3UHandler *import = new M3UHandler();
	import->processFile(m3uFile);

	for(int i = 0; i < import->channelList().size(); i++) {
		_channels << import->channelList()[i];
		_channelNums << import->channelList()[i]->number();

		for(int k = 0; k < import->channelList()[i]->categories().size(); k++) {
			if(!_categoryList.contains(import->channelList()[i]->categories()[k]))
				_categoryList << import->channelList()[i]->categories()[k];
		}
		if(!_languageList.contains(import->channelList()[i]->language()))
			_languageList << import->channelList()[i]->language();
		if(!_epgList.contains(import->channelList()[i]->epg()) && !import->channelList()[i]->epg().isEmpty())
			_epgList << import->channelList()[i]->epg();

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

void PlaylistHandler::importJsFormat(const QString &jsFile)
{
	JsHandler *import = new JsHandler();
	import->processFile(jsFile);

	for(int i = 0; i < import->channelList().size(); i++) {
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

	_name = QObject::tr("Sagem JS Imported Playlist");

	delete import;
}

void PlaylistHandler::importOldFormat(const QString &tanoFile)
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

	for(int i = 0; i < import->channelList().size(); i++) {
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

void PlaylistHandler::moveUp(QTreeWidgetItem *channel)
{
	int currentNum = channelRead(channel)->number();

	if(currentNum == 1)
		return;

	int tmpNum;
	int id = _treeWidget->indexOfTopLevelItem(channel);
	QTreeWidgetItem *tmp = _treeWidget->topLevelItem(id-1);
	for(int i = 1; i < 1000; i++) {
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

void PlaylistHandler::moveDown(QTreeWidgetItem *channel)
{
	int currentNum = channelRead(channel)->number();

	if(currentNum == 999)
		return;

	int tmpNum;
	int id = _treeWidget->indexOfTopLevelItem(channel);
	QTreeWidgetItem *tmp = _treeWidget->topLevelItem(id+1);
	for(int i = 1; i < 1000; i++) {
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

bool PlaylistHandler::validate() const
{
	for(int i = 1; i < _channelNums.size(); i++) {
		if(_channelNums[i-1] == _channelNums[i]) {
			return false;
		}
	}

	return true;
}
