/****************************************************************************
* SettingsPage.cpp: First run wizard - Settings page
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

#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include "FirstRunWizard.h"

SettingsPage::SettingsPage(QWidget *parent) :
	QWizardPage(parent),
	ui(new Ui::SettingsPage)
{
	ui->setupUi(this);

	registerField("session", ui->sessionCheckBox);
	registerField("vlc", ui->vlcCheckBox);
}

SettingsPage::~SettingsPage()
{
	delete ui;
}

void SettingsPage::changeEvent(QEvent *e)
{
	QWizardPage::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

int SettingsPage::nextId() const
{
	if(ui->playlistCheckBox->isChecked())
		return FirstRunWizard::Playlist;
	else
		return FirstRunWizard::Conclusion;
}
