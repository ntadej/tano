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

#if !EDITOR
    _xmltv = new XmltvSystem();
    connect(_xmltv, SIGNAL(grabbers(QList<XmltvGrabber>)), this, SLOT(listGrabbers(QList<XmltvGrabber>)));
    _xmltv->requestGrabbers();
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
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsSchedule::listGrabbers(const QList<XmltvGrabber> &list)
{
#if !EDITOR
    ui->comboGrabber->clear();

    for(int i = 0; i < list.size(); i++) {
        ui->comboGrabber->addItem(list[i].name, list[i].path);
    }

    ui->comboGrabber->setCurrentIndex(ui->comboGrabber->findData(_grabber));
#else
    Q_UNUSED(list)
#endif
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

void SettingsSchedule::setUpdate(const bool &enabled)
{
    ui->checkRefresh->setChecked(enabled);
}

bool SettingsSchedule::updateGrabber() const
{
    return ui->radioGrabber->isChecked();
}

void SettingsSchedule::setUpdateGrabber(const bool &enabled)
{
    ui->radioGrabber->setChecked(enabled);
    ui->radioNetwork->setChecked(!enabled);
}

QString SettingsSchedule::updateUrl() const
{
    return ui->editUrl->text();
}

void SettingsSchedule::setUpdateUrl(const QString &url)
{
    ui->editUrl->setText(url);
}

QString SettingsSchedule::grabber() const
{
    return ui->comboGrabber->currentText();
}

QString SettingsSchedule::grabberPath() const
{
    return ui->comboGrabber->itemData(ui->comboGrabber->currentIndex()).toString();
}

void SettingsSchedule::setGrabber(const QString &grabber)
{
    _grabber = grabber;
    ui->comboGrabber->setCurrentIndex(ui->comboGrabber->findData(_grabber));
}
