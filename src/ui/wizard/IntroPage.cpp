/****************************************************************************
* IntroPage.cpp: First run wizard - Intro page
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

#include "IntroPage.h"
#include "ui_IntroPage.h"

#include "core/Settings.h"
#include "core/Version.h"
#include "FirstRunWizard.h"

IntroPage::IntroPage(QWidget *parent)
	: QWizardPage(parent),
	ui(new Ui::IntroPage)
{
	ui->setupUi(this);

	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/wizard.png"));

	Settings *settings = new Settings();
	if(settings->configurationVersion() != Version::version())
		ui->labelVersion->setText(tr("You previously used version %1 of <i>Tano</i>.")
								  .arg(settings->configurationVersion()) +"<br>"+
								  tr("Please re-set your settings."));
	else
		ui->labelVersion->clear();
	delete settings;
}

IntroPage::~IntroPage()
{
	delete ui;
}

void IntroPage::changeEvent(QEvent *e)
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

int IntroPage::nextId() const
{
	return FirstRunWizard::Settings;
}
