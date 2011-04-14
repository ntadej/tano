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

#ifndef TANO_CSVGENERATOR_H_
#define TANO_CSVGENERATOR_H_

#include <QtCore/QTextStream>
#include <QtGui/QTreeWidget>
#include <QtGui/QTreeWidgetItem>

class Channel;

class CSVGenerator
{
public:
	CSVGenerator(QTreeWidget *treeWidget,
				 QMap<QTreeWidgetItem *, Channel *> map,
				 const bool &header = true);
	~CSVGenerator();

	bool write(QIODevice *device);

private:
	void generateItem(Channel *channel);

	bool _header;

	QTreeWidget *_treeWidget;
	QTextStream _out;

	QMap<QTreeWidgetItem *, Channel *> _map;
};

#endif // TANO_CSVGENERATOR_H_
