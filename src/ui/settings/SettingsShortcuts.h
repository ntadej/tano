/****************************************************************************
* SettingsShortcuts.h: Shortcuts editor
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

#ifndef TANO_SETTINGSSHORTCUTS_H_
#define TANO_SETTINGSSHORTCUTS_H_

#include <QtGui/QTableWidgetItem>
#include <QtGui/QWidget>

#include "core/Shortcuts.h"

namespace Ui
{
	class SettingsShortcuts;
}

class SettingsShortcuts : public QWidget
{
Q_OBJECT
public:
	SettingsShortcuts(QWidget *parent = 0);
	~SettingsShortcuts();

	void setShortcuts(Shortcuts *s);
	void shortcutWrite();

protected:
	void changeEvent(QEvent *e);

private slots:
	void shortcutClear();
	void shortcutEdit(QTableWidgetItem *titem);
	void shortcutRestore();
	void shortcutSequence(const QKeySequence &s);
	void shortcutSet();

private:
	void createActions();
	void shortcutRead();

	Ui::SettingsShortcuts *ui;

	Shortcuts *_shortcuts;

	QTableWidgetItem *_item;
	QKeySequence _sequence;
	bool _success;
};

#endif // TANO_SETTINGSSHORTCUTS_H_
