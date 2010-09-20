/****************************************************************************
* EditSettings.h: Settings editor (Tano Mobile)
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

#ifndef TANOMOBILE_EDITSETTINGS_H_
#define TANOMOBILE_EDITSETTINGS_H_

#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtGui/QAbstractButton>
#include <QtGui/QDialog>

#include "core/Settings.h"

namespace Ui
{
	class EditSettings;
}

class EditSettings : public QDialog
{
Q_OBJECT
public:
	EditSettings(const QString &version, QWidget *parent = 0);
	~EditSettings();

protected:
	void changeEvent(QEvent *e);

private slots:
	void action(QAbstractButton *button);
	void apply();
	void save();
	void cancel();

private:
	void createActions();
	void read();

	void loadLocale();

	Ui::EditSettings *ui;

	Settings *_settings;

	QStringList _locale;
};

#endif // TANOMOBILE_EDITSETTINGS_H_
