/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include "SettingsEditorDialog.h"
#include "ui_SettingsEditorDialog.h"

#include "core/Enums.h"
#include "core/Settings.h"
#include "core/Shortcuts.h"

SettingsEditorDialog::SettingsEditorDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SettingsEditorDialog)
{
    ui->setupUi(this);

    _settings = new Settings(this);

    read();

    connect(ui->general, SIGNAL(resetDefaults()), this, SLOT(defaults()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
}

SettingsEditorDialog::~SettingsEditorDialog()
{
    delete ui;
    delete _settings;
}

void SettingsEditorDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsEditorDialog::action(QAbstractButton *button)
{
    switch(ui->buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Save:
        save();
        break;
    case QDialogButtonBox::Cancel:
        close();
        break;
    default:
        break;
    }
}

void SettingsEditorDialog::apply()
{
    // General
    _settings->setLanguage(ui->general->language());

    // Editor
    _settings->setRadioCategory(ui->editor->radioCategory());
    _settings->setUdpxy(ui->editor->udpxy());
    _settings->setUdpxyUrl(ui->editor->udpxyUrl());
    _settings->setUdpxyPort(ui->editor->udpxyPort());
    _settings->setToolbarLook(ui->editor->toolbar());
    _settings->setSplash(ui->editor->splash());

    _settings->writeSettings();
}

void SettingsEditorDialog::defaults()
{
    // General
    ui->general->setLanguage(Settings::DEFAULT_LANGUAGE);

    // Editor
    ui->editor->setRadioCategory(Settings::DEFAULT_RADIO_CATEGORY);
    ui->editor->setUdpxy(Settings::DEFAULT_UDPXY);
    ui->editor->setUdpxyUrl(Settings::DEFAULT_UDPXY_URL);
    ui->editor->setUdpxyPort(Settings::DEFAULT_UDPXY_PORT);
    ui->editor->setToolbar(Settings::DEFAULT_TOOLBAR_LOOK);
    ui->editor->setSplash(Settings::DEFAULT_SPLASH);
}

void SettingsEditorDialog::read()
{
    // General
    ui->general->setLanguage(_settings->language());

    // Editor
    ui->editor->setRadioCategory(_settings->radioCategory());
    ui->editor->setUdpxy(_settings->udpxy());
    ui->editor->setUdpxyUrl(_settings->udpxyUrl());
    ui->editor->setUdpxyPort(_settings->udpxyPort());
    ui->editor->setToolbar(_settings->toolbarLook());
    ui->editor->setSplash(_settings->splash());
}

void SettingsEditorDialog::save()
{
    apply();
    hide();
}
