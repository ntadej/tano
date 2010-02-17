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

class Shortcuts
{
public:
	Shortcuts(const QList<QAction*> &list);
	~Shortcuts();

	void apply();
	QStringList defaultKeys() const {return defaultList;};
	QStringList actionsNames() const {return actionsName;};

private:
	QList<QAction*> actions;
	QStringList defaultList;
	QStringList actionsName;
	QStringList keys;

	QSettings *settings;
};

#endif // TANO_SHORTCUTS_H_
