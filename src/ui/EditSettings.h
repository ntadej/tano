/****************************************************************************
* EditSettings.h: Settings editor
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

#ifndef TANO_EDITSETTINGS_H_
#define TANO_EDITSETTINGS_H_

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtGui/QDialog>

#include "../control/Shortcuts.h"

#include <ui_EditSettings.h>

class EditSettings : public QDialog
{
Q_OBJECT
public:
	EditSettings(QWidget *parent = 0, Shortcuts *s = 0);
	~EditSettings();
signals:
	void apply();

private slots:
	void action(QAbstractButton *button);
	void ok();
	void cancel();

	void toggleCustom();
	void toggleNetwork();
	void togglePlaylist();

	void playlistBrowse();
	void playlistReset();
	void dirBrowse();
	void dirReset();

	void shortcutRestore();
	void shortcutSequence(const QKeySequence &s);
	void shortcutSet();
	void shortcutClear();
	void shortcutEdit(QTableWidgetItem *titem);

	void loadPlugins();

private:
	void createActions();
	void read();
	void shortcutRead();

	Ui::EditSettings ui;
	QSettings *_settings;
	Shortcuts *_shortcuts;
	QTableWidgetItem *_item;
	QKeySequence _sequence;
	QStringList _actionsList;
	QStringList _keysList;
	bool _success;
};

#endif // TANO_EDITSETTINGS_H_
