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

#include "container/Channel.h"
#include "playlist/JsGenerator.h"

JsGenerator::JsGenerator(QTreeWidget *treeWidget,
						 QMap<QTreeWidgetItem *, Channel *>map)
	: _treeWidget(treeWidget),
	_map(map) { }

JsGenerator::~JsGenerator() { }

bool JsGenerator::write(QIODevice *device)
{
	_out.setDevice(device);
	_out.setCodec("UTF-8");
	_out << "la=[";
	for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i) {
		generateItem(i+1, _map[_treeWidget->topLevelItem(i)]);
		if(i != _treeWidget->topLevelItemCount()-1) {
			_out << ",";
		}
	}
	_out << "];";
	return true;
}

void JsGenerator::generateItem(const int &id,
							   Channel *channel)
{
	_out << "["
		 << id << ","
		 << "\"" << channel->name() << "\"" << ","
		 << "\"" << channel->numberString() << "\"" << ","
		 << "\"" << channel->url().replace(QRegExp("udp://@"), "").replace(QRegExp(":.*"), "") << "\"" << ","
		 << "\"" << channel->url().replace(QRegExp("udp://@.*:"), "") << "\"" << ","
		 << "\"" << channel->categories()[0] << "\"" << ","
		 << "\"" << channel->language() << "\"" << ","
		 << "\"" << channel->epg() << "\"" << ","
		 << "\"" << "false" << "\""
		 << "]";
}
