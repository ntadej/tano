/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_M3UGENERATOR_H_
#define TANO_M3UGENERATOR_H_

#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

#include "container/Channel.h"
#include "core/Common.h"

class M3UGenerator
{
public:
	M3UGenerator(QTreeWidget *treeWidget,
				 const QString &name,
				 const QString &epgType,
				 QMap<QTreeWidgetItem *, Channel *> map);
	~M3UGenerator();

	bool write(QIODevice *device);

private:
	void generateItem(Channel *channel);

	QTreeWidget *_treeWidget;
	QString _name;
	Tano::EpgType _epgType;
	QTextStream _out;

	QMap<QTreeWidgetItem *, Channel *> _map;
};

#endif // TANO_M3UGENERATOR_H_
