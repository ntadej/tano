/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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
}

SettingsUi::~SettingsUi()
{
    delete ui;
}

void SettingsUi::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
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

bool SettingsUi::osdPlaylist() const
{
    return ui->checkOsdPlaylist->isChecked();
}

void SettingsUi::setOsdPlaylist(const bool &enabled)
{
    ui->checkOsdPlaylist->setChecked(enabled);
}

bool SettingsUi::tray() const
{
    return ui->checkTray->isChecked();
}

void SettingsUi::setTray(const bool &enabled)
{
    ui->checkTray->setChecked(enabled);
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

bool SettingsUi::splash() const
{
    return ui->checkSplash->isChecked();
}

void SettingsUi::setSplash(const bool &enabled)
{
    ui->checkSplash->setChecked(enabled);
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

bool SettingsUi::controls() const
{
    return ui->checkControls->isChecked();
}

void SettingsUi::setControls(const bool &enabled)
{
    ui->checkControls->setChecked(enabled);
}

bool SettingsUi::info() const
{
    return ui->checkInfo->isChecked();
}

void SettingsUi::setInfo(const bool &enabled)
{
    ui->checkInfo->setChecked(enabled);
}
