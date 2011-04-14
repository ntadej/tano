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
#include "CSVGenerator.h"

CSVGenerator::CSVGenerator(QTreeWidget *treeWidget,
						   QMap<QTreeWidgetItem *, Channel *> map,
						   const bool &header)
	: _treeWidget(treeWidget),
	_map(map),
	_header(header) { }

CSVGenerator::~CSVGenerator() { }

bool CSVGenerator::write(QIODevice *device)
{
	_out.setDevice(device);
	_out.setCodec("UTF-8");
	if(_header) {
		_out << QObject::tr("Number") << ";"
			 << QObject::tr("Channel") << ";"
			 << QObject::tr("URL") << ";"
			 << QObject::tr("Categories") << ";"
			 << QObject::tr("Language") << ";"
			 << QObject::tr("EPG ID") << "\n";
	}
	for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i) {
		generateItem(_map[_treeWidget->topLevelItem(i)]);
	}
	return true;
}

void CSVGenerator::generateItem(Channel *channel)
{
	_out << channel->numberString() << ";"
		 << channel->name() << ";"
		 << channel->url() << ";"
		 << channel->categories().join(",") << ";"
		 << channel->language() << ";"
		 << channel->epg();
	_out << "\n";
}
