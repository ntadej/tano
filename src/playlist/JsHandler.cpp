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

#include "container/Channel.h"
#include "playlist/JsHandler.h"

JsHandler::JsHandler() { }

JsHandler::~JsHandler() { }

void JsHandler::processFile(const QString &jsFile)
{
	QFile file(jsFile);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		return;

	QTextStream in(&file);
	in.setCodec(QTextCodec::codecForName("UTF-8"));
	while (!in.atEnd()) {
		_lineList << in.readLine();
	}

	processList();
}

void JsHandler::processList()
{
	QString line;
	for(int i = 0; i < _lineList.size(); i++) {
		if(_lineList[i].contains("la=")) {
			line = _lineList[i];
			break;
		}
	}

	line = line.replace("la=[[", "");
	line = line.replace("]];", "");
	line = line.replace("\"", "");

	QStringList channels = line.split("],[");
	QStringList currentChannel;
	for(int i = 0; i < channels.size(); i++) {
		currentChannel = channels[i].split(",");

		Channel *channel = new Channel(currentChannel[1], currentChannel[2].toInt());
		channel->setUrl("udp://@" + currentChannel[3] + ":" + currentChannel[4]);
		channel->setCategories(QStringList() << currentChannel[5]);
		channel->setLanguage(currentChannel[6]);
		channel->setEpg(currentChannel[7]);

		_channelList << channel;
	}
}
