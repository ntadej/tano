/****************************************************************************
* ConclusionPage.cpp: First run wizard - Conclusion page
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

#include "ConclusionPage.h"
#include "ui_ConclusionPage.h"

#include "core/Settings.h"
#include "core/Version.h"

ConclusionPage::ConclusionPage(QWidget *parent)
	: QWizardPage(parent),
	ui(new Ui::ConclusionPage)
{
	ui->setupUi(this);

	registerField("agree*", ui->agreeCheckBox);
}

ConclusionPage::~ConclusionPage()
{
	delete ui;
}

void ConclusionPage::changeEvent(QEvent *e)
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

int ConclusionPage::nextId() const
{
	Settings *settings = new Settings();
	settings->setConfigurationVersion(Version::version());
	settings->setConfigured(true);
	if(!field("playlist").toString().isEmpty())
		settings->setPlaylist(field("playlist").toString());
	settings->setSessionVolume(field("sessionvolume").toBool());
	settings->setSessionAutoplay(field("sessionplay").toBool());
	settings->setGlobalSettings(field("vlc").toBool());

	settings->writeSettings();

	delete settings;

	return -1;
}
