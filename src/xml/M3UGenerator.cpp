/****************************************************************************
* M3UGenerator.cpp: Generator and writer of modified m3u format
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

#include "M3UGenerator.h"

M3UGenerator::M3UGenerator(QTreeWidget *treeWidget,
						   const QString &name,
						   const QString &epg,
						   QMap<QTreeWidgetItem *,
						   Channel *>map)
	: _treeWidget(treeWidget),
	_name(name),
	_epgType(epg),
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
		<< _epgType
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
