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

#include "SettingsPage.h"
#include "ui_SettingsPage.h"

#include "FirstRunWizard.h"
#include "core/Enums.h"
#include "core/Settings.h"

SettingsPage::SettingsPage(QWidget *parent)
    : QWizardPage(parent),
      ui(new Ui::SettingsPage)
{
    ui->setupUi(this);

    QScopedPointer<Settings> settings(new Settings(this));
    ui->sessionVolumeCheckBox->setChecked(settings->sessionVolume());
    ui->sessionAutoplayCheckBox->setChecked(settings->sessionAutoplay());
    ui->playlist->setType(Tano::M3U);
    ui->playlist->setValue(settings->playlist());

    registerField("sessionvolume", ui->sessionVolumeCheckBox);
    registerField("sessionplay", ui->sessionAutoplayCheckBox);
    registerField("playlist", ui->playlist->edit());
}

SettingsPage::~SettingsPage()
{
    delete ui;
}

void SettingsPage::changeEvent(QEvent *e)
{
    QWizardPage::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

int SettingsPage::nextId() const
{
    return FirstRunWizard::Conclusion;
}
