/****************************************************************************
* SettingsPage.h: First run wizard - Settings page
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

#ifndef TANO_SETTINGSPAGE_H_
#define TANO_SETTINGSPAGE_H_

#include <QtGui/QWizardPage>

namespace Ui
{
	class SettingsPage;
}

class SettingsPage : public QWizardPage
{
Q_OBJECT
public:
	SettingsPage(QWidget *parent = 0);
	~SettingsPage();

	int nextId() const;

protected:
	void changeEvent(QEvent *e);

private:
	Ui::SettingsPage *ui;
};

#endif // TANO_SETTINGSPAGE_H_
