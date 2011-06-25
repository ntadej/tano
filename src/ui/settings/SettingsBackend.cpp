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

#include "Config.h"
#include "SettingsBackend.h"
#include "ui_SettingsBackend.h"

#if WITH_EDITOR_VLCQT
    #include <vlc-qt/Instance.h>
#endif

SettingsBackend::SettingsBackend(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsBackend)
{
    ui->setupUi(this);

#if EDITOR
    ui->groupPlayback->hide();
#endif

#if WITH_EDITOR_VLCQT
   ui->labelVlcqtVersion->setText(ui->labelVlcqtVersion->text() + " <b>" + VlcInstance::libVersion() + "</b>");
   ui->labelVlcVersion->setText(ui->labelVlcVersion->text() + " <b>" + VlcInstance::version() + "</b>");
#else
   ui->labelVlcqtVersion->setText(ui->labelVlcqtVersion->text() + " /");
   ui->labelVlcVersion->setText(ui->labelVlcVersion->text() + " /");
   ui->checkGlobal->setDisabled(true);
#endif
   ui->labelUdpxyInfo->setText(ui->labelUdpxyInfo->text().arg("</i>udp://@232.4.1.1:5002<i>", "</i>http://router:port/udp/232.4.1.1:5002<i>"));
}

SettingsBackend::~SettingsBackend()
{
    delete ui;
}

void SettingsBackend::changeEvent(QEvent *e)
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

bool SettingsBackend::globalSettings() const
{
    return ui->checkGlobal->isChecked();
}

void SettingsBackend::setGlobalSettings(const bool &global)
{
    ui->checkGlobal->setChecked(global);
}

bool SettingsBackend::rememberChannelSettings() const
{
    return ui->checkVideoSettings->isChecked();
}

void SettingsBackend::setRememberChannelSettings(const bool &remember)
{
    ui->checkVideoSettings->setChecked(remember);
}

QString SettingsBackend::audio() const
{
    return ui->comboAudio->currentText();
}

void SettingsBackend::setAudio(const QString &audio)
{
    for(int i = 0; i < ui->comboAudio->count(); i++) {
        if(ui->comboAudio->itemText(i) == audio) {
            ui->comboAudio->setCurrentIndex(i);
            break;
        } else if(i == ui->comboAudio->count() - 1) {
            ui->comboAudio->setItemText(i, audio);
            ui->comboAudio->setCurrentIndex(i);
        }
    }
}

QString SettingsBackend::sub() const
{
    return ui->comboSub->currentText();
}

void SettingsBackend::setSub(const QString &sub)
{
    for(int i = 0; i < ui->comboSub->count(); i++) {
        if(ui->comboSub->itemText(i) == sub) {
            ui->comboSub->setCurrentIndex(i);
            break;
        } else if(i == ui->comboSub->count() - 1) {
            ui->comboSub->setItemText(i, sub);
            ui->comboSub->setCurrentIndex(i);
        }
    }
}

bool SettingsBackend::udpxy() const
{
    return ui->checkUdpxy->isChecked();
}

void SettingsBackend::setUdpxy(const bool &enabled)
{
    ui->checkUdpxy->setChecked(enabled);
}

QString SettingsBackend::udpxyUrl() const
{
    return ui->editUdpxyUrl->text();
}

void SettingsBackend::setUdpxyUrl(const QString &url)
{
    ui->editUdpxyUrl->setText(url);
}

int SettingsBackend::udpxyPort() const
{
    return ui->editUdpxyPort->value();
}

void SettingsBackend::setUdpxyPort(const int &port)
{
    ui->editUdpxyPort->setValue(port);
}
