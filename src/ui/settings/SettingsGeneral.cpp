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

#include "core/Common.h"
#include "core/LocaleManager.h"
#include "SettingsGeneral.h"
#include "ui_SettingsGeneral.h"

SettingsGeneral::SettingsGeneral(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsGeneral)
{
    ui->setupUi(this);
    loadLocale();

    connect(ui->buttonDefaults, SIGNAL(clicked()), this, SIGNAL(resetDefaults()));

    ui->labelVersion->setText(tr("You are using Tano version:") + " <b>" + Tano::version() + "</b>");
}

SettingsGeneral::~SettingsGeneral()
{
    delete ui;
}

void SettingsGeneral::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

QString SettingsGeneral::language() const
{
    if (ui->radioCustom->isChecked())
        return _locale[ui->comboLanguage->currentIndex()];
    else
        return QString("");
}

void SettingsGeneral::setLanguage(const QString &language)
{
    if (!language.isEmpty()) {
        ui->radioCustom->setChecked(true);
        for (int i = 0; i < _locale.size(); i++) {
            if (language == _locale[i]) {
                ui->comboLanguage->setCurrentIndex(i);
            }
        }
    } else {
        ui->radioDefault->setChecked(true);
    }
}

bool SettingsGeneral::sessionAutoplay() const
{
    return ui->checkAutoplay->isChecked();
}

void SettingsGeneral::setSessionAutoplay(const bool &enabled)
{
    ui->checkAutoplay->setChecked(enabled);
}

bool SettingsGeneral::sessionVolume() const
{
    return ui->checkVolume->isChecked();
}

void SettingsGeneral::setSessionVolume(const bool &enabled)
{
    ui->checkVolume->setChecked(enabled);
}

void SettingsGeneral::loadLocale()
{
    _locale = LocaleManager::loadTranslations();

    foreach (const QString &language, _locale)
        ui->comboLanguage->addItem(LocaleManager::language(language));
}
