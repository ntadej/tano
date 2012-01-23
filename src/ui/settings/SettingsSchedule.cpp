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

#include "core/Settings.h"

#if !EDITOR
    #include "xmltv/XmltvSystem.h"
#endif

#include "SettingsSchedule.h"
#include "ui_SettingsSchedule.h"

SettingsSchedule::SettingsSchedule(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsSchedule)
{
    ui->setupUi(this);

    ui->location->setType(Tano::XmltvFile);
    ui->location->setResetValue(Settings::DEFAULT_XMLTV_LOCATION);

    connect(ui->comboGrabber, SIGNAL(currentIndexChanged(QString)), this, SLOT(processGrabber(QString)));

#if !EDITOR
    _xmltv = new XmltvSystem();
    listGrabbers();
#endif
}

SettingsSchedule::~SettingsSchedule()
{
    delete ui;
#if !EDITOR
    delete _xmltv;
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
#if !EDITOR
    QStringList grabbers = _xmltv->grabbers();

    ui->comboGrabber->clear();

    for(int i = 0; i < grabbers.size(); i++) {
        ui->comboGrabber->addItem(grabbers[i].split("|")[1], grabbers[i].split("|")[0]);
    }

    ui->comboGrabber->addItem(tr("Custom"), "custom");
#endif
}

void SettingsSchedule::processGrabber(const QString &grabber)
{
    ui->labelCustom->setEnabled(grabber == tr("Custom"));
    ui->location->setEnabled(grabber == tr("Custom"));

    if(grabber != tr("Custom"))
        ui->location->reset();
}

QString SettingsSchedule::xmltvGrabber() const
{
    return ui->comboGrabber->itemData(ui->comboGrabber->currentIndex()).toString();
}

void SettingsSchedule::setXmltvGrabber(const QString &grabber)
{
    ui->comboGrabber->setCurrentIndex(ui->comboGrabber->findData(grabber));
}

QString SettingsSchedule::xmltvLocation() const
{
    return ui->location->value();
}

void SettingsSchedule::setXmltvLocation(const QString &location)
{
    ui->location->setValue(location);
}
