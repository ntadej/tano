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

#include "common/File.h"
#include "settings/Settings.h"

#include "SettingsSchedule.h"
#include "ui_SettingsSchedule.h"

SettingsSchedule::SettingsSchedule(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsSchedule)
{
    ui->setupUi(this);

    QScopedPointer<Settings> settings(new Settings(this));
    ui->location->setType(File::XmltvFile);
    ui->location->setResetValue(settings->defaultValue(Settings::KEY_XMLTV_UPDATE_LOCATION).toString());
}

SettingsSchedule::~SettingsSchedule()
{
    delete ui;
}

void SettingsSchedule::changeEvent(QEvent *e)
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

QString SettingsSchedule::location() const
{
    return ui->location->value();
}

void SettingsSchedule::setLocation(const QString &location)
{
    ui->location->setValue(location);
}

bool SettingsSchedule::update() const
{
    return ui->checkRefresh->isChecked();
}

void SettingsSchedule::setUpdate(bool enabled)
{
    ui->checkRefresh->setChecked(enabled);
}

bool SettingsSchedule::remote() const
{
    return ui->radioNetwork->isChecked();
}

void SettingsSchedule::setRemote(bool enabled)
{
    ui->radioFile->setChecked(!enabled);
    ui->radioNetwork->setChecked(enabled);
}

int SettingsSchedule::offset() const
{
    return ui->spinOffset->value();
}

void SettingsSchedule::setOffset(int hours)
{
    ui->spinOffset->setValue(hours);
}
