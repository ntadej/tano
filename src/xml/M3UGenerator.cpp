/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "M3UGenerator.h"

M3UGenerator::M3UGenerator(QTreeWidget *treeWidget,
						   const QString &name,
						   const QString &epgType,
						   QMap<QTreeWidgetItem *, Channel *>map)
	: _treeWidget(treeWidget),
	_name(name),
	_epgType(Tano::epgType(epgType)),
	_map(map) { }

M3UGenerator::~M3UGenerator() { }

bool M3UGenerator::write(QIODevice *device)
{
	_out.setDevice(device);
	_out.setCodec("UTF-8");
	_out << "#EXTM3U\n"
		<< "#EXTNAME:"
		<< _name
		<< "\n"
		<< "#EXTEPG:"
		<< Tano::epgType(_epgType)
		<< "\n\n";
	for (int i = 0; i < _treeWidget->topLevelItemCount(); ++i)
		generateItem(_map[_treeWidget->topLevelItem(i)]);
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
	if(!channel->logo().isEmpty())
		_out << ";" << channel->logo();
	_out << "\n";

	_out << channel->url();
	_out << "\n\n";
}
