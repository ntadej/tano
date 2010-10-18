/****************************************************************************
* SettingsEdit.h: Settings editor
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

#ifndef TANO_SETTINGSEDIT_H_
#define TANO_SETTINGSEDIT_H_

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

#include "core/Settings.h"
#include "core/Shortcuts.h"

namespace Ui
{
	class SettingsEdit;
}

class SettingsEdit : public QDialog
{
Q_OBJECT
public:
	SettingsEdit(Shortcuts *s = 0,
				 QWidget *parent = 0);
	~SettingsEdit();

protected:
	void changeEvent(QEvent *e);

private slots:
	void action(QAbstractButton *button);
	void apply();
	void cancel();
	void save();

	void recorderDirectoryBrowse();
	void recorderDirectoryReset();

private:
	void createActions();
	void loadLocale();
	void loadPlugins();
	void read();

	Ui::SettingsEdit *ui;

	Settings *_settings;

	QStringList _locale;
};

#endif // TANO_SETTINGSEDIT_H_
