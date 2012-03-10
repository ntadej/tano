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

#include "Config.h"
#include "SettingsBackend.h"
#include "ui_SettingsBackend.h"

#include <vlc-qt/Enums.h>
#include <vlc-qt/Instance.h>

SettingsBackend::SettingsBackend(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsBackend)
{
    ui->setupUi(this);

    ui->labelVlcqtVersion->setText(ui->labelVlcqtVersion->text() + " <b>" + VlcInstance::libVersion() + "</b>");
    ui->labelVlcVersion->setText(ui->labelVlcVersion->text() + " <b>" + VlcInstance::version() + "</b>");

    for (int i = 1; i < Vlc::ratioHuman().size(); i++) {
        ui->comboAspectRatio->addItem(Vlc::ratioHuman()[i]);
        ui->comboCropRatio->addItem(Vlc::ratioHuman()[i]);
    }
}

SettingsBackend::~SettingsBackend()
{
    delete ui;
}

void SettingsBackend::changeEvent(QEvent *e)
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

bool SettingsBackend::rememberChannelSettings() const
{
    return ui->checkVideoSettings->isChecked();
}

int SettingsBackend::aspectRatio() const
{
    return ui->comboAspectRatio->currentIndex();
}

void SettingsBackend::setAspectRatio(const int &id)
{
    ui->comboAspectRatio->setCurrentIndex(id);
}

int SettingsBackend::cropRatio() const
{
    return ui->comboCropRatio->currentIndex();
}

void SettingsBackend::setCropRatio(const int &id)
{
    ui->comboCropRatio->setCurrentIndex(id);
}

int SettingsBackend::deinterlacing() const
{
    return ui->comboDeinterlacing->currentIndex();
}

void SettingsBackend::setDeinterlacing(const int &id)
{
    ui->comboDeinterlacing->setCurrentIndex(id);
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
    for (int i = 0; i < ui->comboAudio->count(); i++) {
        if (ui->comboAudio->itemText(i) == audio) {
            ui->comboAudio->setCurrentIndex(i);
            break;
        } else if (i == ui->comboAudio->count() - 1) {
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
    for (int i = 0; i < ui->comboSub->count(); i++) {
        if (ui->comboSub->itemText(i) == sub) {
            ui->comboSub->setCurrentIndex(i);
            break;
        } else if (i == ui->comboSub->count() - 1) {
            ui->comboSub->setItemText(i, sub);
            ui->comboSub->setCurrentIndex(i);
        }
    }
}
