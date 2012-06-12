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

#include "core/Enums.h"
#include "core/Settings.h"

#include "SettingsRecorder.h"
#include "ui_SettingsRecorder.h"

SettingsRecorder::SettingsRecorder(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsRecorder)
{
    ui->setupUi(this);
    ui->edit->setType(Tano::Directory);
    ui->edit->setResetValue(Settings::DEFAULT_RECORDER_DIRECTORY);
}

SettingsRecorder::~SettingsRecorder()
{
    delete ui;
}

void SettingsRecorder::changeEvent(QEvent *e)
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

QString SettingsRecorder::directory() const
{
    return ui->edit->value();
}

void SettingsRecorder::setDirectory(const QString &value)
{
    ui->edit->setValue(value);
}


QString SettingsRecorder::snapshot() const
{
    return ui->editS->value();
}

void SettingsRecorder::setSnapshot(const QString &value)
{
    ui->editS->setValue(value);
}
