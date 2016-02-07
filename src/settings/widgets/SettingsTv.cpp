/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include "SettingsTv.h"
#include "ui_SettingsTv.h"

SettingsTv::SettingsTv(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsTv)
{
    ui->setupUi(this);
    ui->channels->setType(File::M3U);

    QScopedPointer<Settings> settings(new Settings(this));
    ui->channels->setValue(settings->channelsList());

    ui->editHd->setText(settings->hdCategory());
    ui->editRadio->setText(settings->radioCategory());

    ui->checkUdpxy->setChecked(settings->udpxy());
    ui->editUdpxyUrl->setText(settings->udpxyUrl());
    ui->editUdpxyPort->setValue(settings->udpxyPort());

    ui->radioNetwork->setChecked(settings->xmltvUpdateRemote());
    ui->browseXmltv->setValue(settings->xmltvUpdateLocation());
    ui->spinOffset->setValue(settings->xmltvHoursOffset());

    _channelsTimer = new QTimer();
    _channelsTimer->setSingleShot(true);
    connect(_channelsTimer, &QTimer::timeout, this, &SettingsTv::channelsChanged);

    connect(ui->channels, &BrowseWidget::valueChanged, this, &SettingsTv::channelsSettingsChanged);
    connect(ui->editHd, &QLineEdit::textChanged, this, &SettingsTv::channelsSettingsChanged);
    connect(ui->editRadio, &QLineEdit::textChanged, this, &SettingsTv::channelsSettingsChanged);

    connect(ui->checkUdpxy, &QCheckBox::toggled, this, &SettingsTv::udpxySettingsChanged);
    connect(ui->editUdpxyUrl, &QLineEdit::textChanged, this, &SettingsTv::udpxySettingsChanged);
    connect(ui->editUdpxyPort, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SettingsTv::udpxySettingsChanged);

    connect(ui->radioFile, &QRadioButton::toggled, this, &SettingsTv::xmltvSettingsChanged);
    connect(ui->browseXmltv, &BrowseWidget::valueChanged, this, &SettingsTv::xmltvSettingsChanged);
    connect(ui->spinOffset, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SettingsTv::xmltvSettingsChanged);
}

SettingsTv::~SettingsTv()
{
    delete ui;
    delete _channelsTimer;
}

void SettingsTv::changeEvent(QEvent *e)
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

void SettingsTv::channelsSettingsChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
    settings->setChannelsList(ui->channels->value());
    settings->setHdCategory(ui->editHd->text());
    settings->setRadioCategory(ui->editRadio->text());
    settings->writeSettings();

    _channelsTimer->stop();
    _channelsTimer->start(1000);
}

void SettingsTv::udpxySettingsChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
    settings->setUdpxy(ui->checkUdpxy->isChecked());
    settings->setUdpxyUrl(ui->editUdpxyUrl->text());
    settings->setUdpxyPort(ui->editUdpxyPort->value());
    settings->writeSettings();

    emit udpxyChanged();
}

void SettingsTv::xmltvSettingsChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
    settings->setXmltvUpdateRemote(ui->radioNetwork->isChecked());
    settings->setXmltvUpdateLocation(ui->browseXmltv->value());
    settings->setXmltvHoursOffset(ui->spinOffset->value());
    settings->writeSettings();

    emit xmltvChanged();
}
