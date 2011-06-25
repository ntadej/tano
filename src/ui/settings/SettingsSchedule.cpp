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

#include "SettingsSchedule.h"
#include "ui_SettingsSchedule.h"

SettingsSchedule::SettingsSchedule(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsSchedule)
{
    ui->setupUi(this);
}

SettingsSchedule::~SettingsSchedule()
{
    delete ui;
}

void SettingsSchedule::changeEvent(QEvent *e)
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

bool SettingsSchedule::xmltv() const
{
    return ui->radioXmltv->isChecked();
}

void SettingsSchedule::setXmltv(const bool &enabled)
{
    ui->radioXmltv->setChecked(enabled);
}

QString SettingsSchedule::grabber() const
{
    return ui->comboGrabber->currentText();
}

void SettingsSchedule::setGrabber(const QString &grabber)
{

}
