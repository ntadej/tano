/****************************************************************************
* Shortcuts.h: Setting shortcuts on actions
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
