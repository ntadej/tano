/****************************************************************************
* M3UGenerator.h: Generator and writer of modified m3u format
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

#ifndef TANO_M3UGENERATOR_H_
#define TANO_M3UGENERATOR_H_

#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "container/Channel.h"

class M3UGenerator
{
public:
	M3UGenerator(QTreeWidget *treeWidget,
				 const QString &name,
				 const QString &epg,
				 QMap<QTreeWidgetItem*,
				 Channel*> map);
	~M3UGenerator();

	bool write(QIODevice *device);

private:
	void generateItem(Channel *channel);

	QTreeWidget *_treeWidget;
	QString _name;
	QString _epgPlugin;
	QTextStream _out;

	QMap<QTreeWidgetItem*, Channel*> _map;
};

#endif // TANO_M3UGENERATOR_H_
