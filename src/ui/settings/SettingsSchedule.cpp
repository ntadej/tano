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

#if EDITOR
#else
    #include "epg/XmltvController.h"
#endif

#include "SettingsSchedule.h"
#include "ui_SettingsSchedule.h"

SettingsSchedule::SettingsSchedule(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsSchedule)
{
    ui->setupUi(this);

#if EDITOR
#else
    _controller = new XmltvController(this);
    listGrabbers();
#endif
}

SettingsSchedule::~SettingsSchedule()
{
    delete ui;
#if EDITOR
#else
    delete _controller;
#endif
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

void SettingsSchedule::listGrabbers()
{
#if EDITOR
#else
    QStringList grabbers = _controller->grabbers();

    ui->comboGrabber->clear();

    for(int i = 0; i < grabbers.size(); i++) {
        ui->comboGrabber->addItem(grabbers[i].split("|")[1], grabbers[i].split("|")[0]);
    }
#endif
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
    return ui->comboGrabber->itemData(ui->comboGrabber->currentIndex()).toString();
}

void SettingsSchedule::setGrabber(const QString &grabber)
{
    ui->comboGrabber->setCurrentIndex(ui->comboGrabber->findData(grabber));
}
