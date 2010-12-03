/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <info@tano.si>
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

#ifndef TANO_SHORTCUTS_H_
#define TANO_SHORTCUTS_H_

#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtGui/QAction>

class Shortcuts : public QSettings
{
public:
	Shortcuts(const QList<QAction*> &list,
			  QObject *parent = 0);
	~Shortcuts();

	void apply();
	void restoreDefaults() { write(_defaultList); }
	void write(const QStringList &keys);
	QStringList defaultKeys() const { return _defaultList; }
	QStringList actionsNames() const { return _actionsName; }
	QStringList readKeys() const;

private:
	QList<QAction*> _actions;
	QStringList _defaultList;
	QStringList _actionsName;
};

#endif // TANO_SHORTCUTS_H_
