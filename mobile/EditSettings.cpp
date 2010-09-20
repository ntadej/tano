/****************************************************************************
* EditSettings.cpp: Settings editor (Tano Mobile)
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

#include "EditSettings.h"
#include "ui_EditSettings.h"

#include "Config.h"
#include "core/LocaleManager.h"

EditSettings::EditSettings(const QString &version, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EditSettings)
{
	ui->setupUi(this);
	createActions();

	_settings = new Settings(this);

	ui->labelVersion->setText(tr("You are using Tano Mobile version:")+" <b>"+version+"</b>");

	loadLocale();
	read();
}

EditSettings::~EditSettings()
{
	delete ui;
	delete _settings;
}

void EditSettings::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void EditSettings::createActions()
{
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
}

void EditSettings::action(QAbstractButton *button)
{
	switch(ui->buttonBox->standardButton(button)) {
		case QDialogButtonBox::Save:
			save();
			break;
		case QDialogButtonBox::Cancel:
			cancel();
			break;
		default:
			break;
	}
}

void EditSettings::apply()
{
	// General
	if(ui->customLanguageRadio->isChecked())
		_settings->setLanguage(_locale[ui->languageComboBox->currentIndex()]);
	else
		_settings->setLanguage("");

	// Channels
	_settings->setPlaylist(ui->widgetSelectPlaylist->playlist());

	_settings->writeSettings();
}

void EditSettings::save()
{
	apply();
	hide();
}

void EditSettings::cancel()
{
	hide();
}

void EditSettings::read()
{
	// General
	if(_settings->language() != "") {
		ui->customLanguageRadio->setChecked(true);
		for(int i=0; i<_locale.size(); i++)
			if(_settings->language() == _locale[i])
				ui->languageComboBox->setCurrentIndex(i);
	}

	// Channels
	ui->widgetSelectPlaylist->setPlaylist(_settings->playlist());
}

void EditSettings::loadLocale()
{
	_locale = LocaleManager::loadTranslations();

	for(int i=0; i<_locale.size(); i++)
		ui->languageComboBox->addItem(LocaleManager::language(_locale[i]));
}
