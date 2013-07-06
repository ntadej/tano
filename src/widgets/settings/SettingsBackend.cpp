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

#include "core/platform/Features.h"

#include "common/Backend.h"

#include "SettingsBackend.h"
#include "ui_SettingsBackend.h"

#include <vlc-qt/Enums.h>

SettingsBackend::SettingsBackend(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsBackend)
{
    ui->setupUi(this);

    ui->labelVlcqtIcon->setPixmap(QIcon(":/logo/48x48/vlc-qt.png").pixmap(48));
    ui->labelVlcqtVersion->setText(ui->labelVlcqtVersion->text() + " <b>" + Tano::Backend::versionLibrary() + "</b>");
    ui->labelVlcVersionValue->setText("<b>" + Tano::Backend::versionCore() + "</b>");

    for (int i = 0; i < Vlc::audioOutputHuman().size() - 1; i++) {
        ui->comboAout->addItem(Vlc::audioOutputHuman()[i]);
    }

    for (int i = 0; i < Vlc::videoOutputHuman().size() - 1; i++) {
        ui->comboVout->addItem(Vlc::videoOutputHuman()[i]);
    }

    for (int i = 1; i < Vlc::ratioHuman().size(); i++) {
        ui->comboAspectRatio->addItem(Vlc::ratioHuman()[i]);
        ui->comboCropRatio->addItem(Vlc::ratioHuman()[i]);
    }

#if !defined(Q_OS_WIN32)
    ui->checkYTR->hide();
#endif

#if !FEATURE_TELETEXT
    ui->checkTeletext->hide();
#endif
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

int SettingsBackend::vout() const
{
    return ui->comboVout->currentIndex() - 1;
}

void SettingsBackend::setVout(int id)
{
    ui->comboVout->setCurrentIndex(id + 1);
}

int SettingsBackend::aout() const
{
    return ui->comboAout->currentIndex() - 1;
}

void SettingsBackend::setAout(int id)
{
    ui->comboAout->setCurrentIndex(id + 1);
}

#if defined(Q_OS_WIN32)
bool SettingsBackend::yuvToRgb() const
{
    return ui->checkYTR->isChecked();
}

void SettingsBackend::setYuvToRgb(bool enabled)
{
    ui->checkYTR->setChecked(enabled);
}
#endif

bool SettingsBackend::spdif() const
{
    return ui->checkSpdif->isChecked();
}

void SettingsBackend::setSpdif(bool enabled)
{
    ui->checkSpdif->setChecked(enabled);
}

bool SettingsBackend::rememberChannelSettings() const
{
    return ui->checkVideoSettings->isChecked();
}

void SettingsBackend::setRememberChannelSettings(bool remember)
{
    ui->checkVideoSettings->setChecked(remember);
}

bool SettingsBackend::rememberPerChannel() const
{
    return ui->radioChannel->isChecked();
}

void SettingsBackend::setRememberPerChannel(bool remember)
{
    ui->radioChannel->setChecked(remember);
}

int SettingsBackend::aspectRatio() const
{
    return ui->comboAspectRatio->currentIndex();
}

void SettingsBackend::setAspectRatio(int id)
{
    ui->comboAspectRatio->setCurrentIndex(id);
}

int SettingsBackend::cropRatio() const
{
    return ui->comboCropRatio->currentIndex();
}

void SettingsBackend::setCropRatio(int id)
{
    ui->comboCropRatio->setCurrentIndex(id);
}

int SettingsBackend::deinterlacing() const
{
    return ui->comboDeinterlacing->currentIndex();
}

void SettingsBackend::setDeinterlacing(int id)
{
    ui->comboDeinterlacing->setCurrentIndex(id);
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

bool SettingsBackend::mute() const
{
    return ui->checkMute->isChecked();
}

void SettingsBackend::setMute(bool enabled)
{
    ui->checkMute->setChecked(enabled);
}

bool SettingsBackend::teletext() const
{
    return ui->checkTeletext->isChecked();
}

void SettingsBackend::setTeletext(bool enabled)
{
    ui->checkTeletext->setChecked(enabled);
}
