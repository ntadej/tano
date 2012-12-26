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

#include "SettingsUi.h"
#include "ui_SettingsUi.h"

SettingsUi::SettingsUi(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsUi)
{
    ui->setupUi(this);

#if !defined(Q_OS_LINUX)
    ui->comboIcons->removeItem(1);
#endif
}

SettingsUi::~SettingsUi()
{
    delete ui;
}

void SettingsUi::changeEvent(QEvent *e)
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

bool SettingsUi::osd() const
{
    return ui->checkOsd->isChecked();
}

void SettingsUi::setOsd(const bool &enabled)
{
    ui->checkOsd->setChecked(enabled);
}

bool SettingsUi::info() const
{
    return ui->checkInfo->isChecked();
}

void SettingsUi::setInfo(const bool &enabled)
{
    ui->checkInfo->setChecked(enabled);
}

bool SettingsUi::tray() const
{
    return ui->checkTray->isChecked();
}

void SettingsUi::setTray(const bool &enabled)
{
    ui->checkTray->setChecked(enabled);
    ui->checkTrayHide->setEnabled(enabled);
}

bool SettingsUi::trayHide() const
{
    return ui->checkTrayHide->isChecked();
}

void SettingsUi::setTrayHide(const bool &enabled)
{
    ui->checkTrayHide->setChecked(enabled);
}

QString SettingsUi::wheel() const
{
    if(ui->radioWheelChannel->isChecked())
        return("channel");
    else if(ui->radioWheelVolume->isChecked())
        return("volume");
    else
        return("error");
}

void SettingsUi::setWheel(const QString &wheel)
{
    if(wheel == "channel")
        ui->radioWheelChannel->setChecked(true);
    else if(wheel == "volume")
        ui->radioWheelVolume->setChecked(true);
}

int SettingsUi::toolbar() const
{
    return ui->comboToolbarLook->currentIndex();
}

void SettingsUi::setToolbar(const int &id)
{
    ui->comboToolbarLook->setCurrentIndex(id);
}

bool SettingsUi::rememberSize() const
{
    return ui->checkRememberSize->isChecked();
}

void SettingsUi::setRememberSize(const bool &enabled)
{
    ui->checkRememberSize->setChecked(enabled);
}

bool SettingsUi::filter() const
{
    return ui->checkFilter->isChecked();
}

void SettingsUi::setFilter(const bool &enabled)
{
    ui->checkFilter->setChecked(enabled);
}

QString SettingsUi::icons() const
{
    if(ui->comboIcons->currentIndex() == 0)
        return("tano-default");
#if defined(Q_OS_LINUX)
    else if(ui->comboIcons->currentIndex() == 1)
        return("");
#endif
    else
        return("tano-default");
}

void SettingsUi::setIcons(const QString &icons)
{
    if (icons == "tano-default")
        ui->comboIcons->setCurrentIndex(0);
#if defined(Q_OS_LINUX)
    else if (icons.isEmpty())
        ui->comboIcons->setCurrentIndex(1);
#endif
    else
        ui->comboIcons->setCurrentIndex(0);
}

bool SettingsUi::lite() const
{
    return ui->checkLite->isChecked();
}

void SettingsUi::setLite(const bool &enabled)
{
    ui->checkLite->setChecked(enabled);
}

bool SettingsUi::top() const
{
    return ui->checkTop->isChecked();
}

void SettingsUi::setTop(const bool &enabled)
{
    ui->checkTop->setChecked(enabled);
}
