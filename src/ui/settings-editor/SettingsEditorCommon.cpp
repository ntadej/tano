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

#include "SettingsEditorCommon.h"
#include "ui_SettingsEditorCommon.h"

SettingsEditorCommon::SettingsEditorCommon(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsEditorCommon)
{
    ui->setupUi(this);
}

SettingsEditorCommon::~SettingsEditorCommon()
{
    delete ui;
}

void SettingsEditorCommon::changeEvent(QEvent *e)
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

QString SettingsEditorCommon::radioCategory() const
{
    return ui->editRadio->text();
}

void SettingsEditorCommon::setRadioCategory(const QString &category)
{
    ui->editRadio->setText(category);
}

bool SettingsEditorCommon::udpxy() const
{
    return ui->checkUdpxy->isChecked();
}

void SettingsEditorCommon::setUdpxy(const bool &enabled)
{
    ui->checkUdpxy->setChecked(enabled);
}

QString SettingsEditorCommon::udpxyUrl() const
{
    return ui->editUdpxyUrl->text();
}

void SettingsEditorCommon::setUdpxyUrl(const QString &url)
{
    ui->editUdpxyUrl->setText(url);
}

int SettingsEditorCommon::udpxyPort() const
{
    return ui->editUdpxyPort->value();
}

void SettingsEditorCommon::setUdpxyPort(const int &port)
{
    ui->editUdpxyPort->setValue(port);
}

int SettingsEditorCommon::toolbar() const
{
    return ui->comboToolbarLook->currentIndex();
}

void SettingsEditorCommon::setToolbar(const int &id)
{
    ui->comboToolbarLook->setCurrentIndex(id);
}

bool SettingsEditorCommon::splash() const
{
    return ui->checkSplash->isChecked();
}

void SettingsEditorCommon::setSplash(const bool &enabled)
{
    ui->checkSplash->setChecked(enabled);
}
