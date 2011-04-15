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
#include "playlist/M3UGenerator.h"

M3UGenerator::M3UGenerator(QTreeWidget *treeWidget,
						   const QString &name,
						   QMap<QTreeWidgetItem *, Channel *> map,
						   const bool &clean)
	: _treeWidget(treeWidget),
	_name(name),
	_map(map),
	_clean(clean) { }

M3UGenerator::~M3UGenerator() { }

bool M3UGenerator::write(QIODevice *device)
{
	_out.setDevice(device);
	_out.setCodec("UTF-8");
	_out << "#EXTM3U\n";
	if(!_clean) {
		_out << "#EXTNAME:"
			 << _name
			 << "\n\n";
	}
	for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i) {
		if(_clean) {
			generateItemClean(_map[_treeWidget->topLevelItem(i)]);
		} else {
			generateItem(_map[_treeWidget->topLevelItem(i)]);
		}
	}
	return true;
}

void M3UGenerator::generateItem(Channel *channel)
{
	_out << "#EXTINF:"
		 << channel->numberString() << ","
		 << channel->name() << "\n";

	_out << "#EXTTV:"
		 << channel->categories().join(",") << ";"
		 << channel->language() << ";"
		 << channel->epg();
	_out << "\n";

	_out << channel->url();
	_out << "\n\n";
}

void M3UGenerator::generateItemClean(Channel *channel)
{
	_out << "#EXTINF:"
		 << channel->numberString() << ","
		 << channel->name() << "\n";

	_out << channel->url();
	_out << "\n\n";
}

