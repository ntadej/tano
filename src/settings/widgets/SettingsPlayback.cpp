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

#include "settings/Settings.h"

#include "SettingsPlayback.h"
#include "ui_SettingsPlayback.h"

#include <VLCQtCore/Enums.h>

SettingsPlayback::SettingsPlayback(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SettingsPlayback)
{
    ui->setupUi(this);

    ui->comboAspectRatio->addItem(tr("Original"));
    ui->comboCropRatio->addItem(tr("Original"));
    for (int i = 2; i < Vlc::ratioHuman().size(); i++) {
        ui->comboAspectRatio->addItem(Vlc::ratioHuman()[i]);
        ui->comboCropRatio->addItem(Vlc::ratioHuman()[i]);
    }

    ui->comboDeinterlacing->addItem(tr("Disabled"));
    for (int i = 1; i < Vlc::deinterlacing().size(); i++) {
        ui->comboDeinterlacing->addItem(Vlc::deinterlacing()[i]);
    }

    QScopedPointer<Settings> settings(new Settings(this));
    if (settings->rememberVideoPerChannel())
        ui->radioSettingsChannel->setChecked(true);
    else if (settings->rememberVideoSettings())
        ui->radioSettingsAll->setChecked(true);
    else
        ui->radioSettingsNone->setChecked(true);

    connect(ui->radioSettingsNone, &QRadioButton::toggled, this, &SettingsPlayback::rememberChanged);
    connect(ui->radioSettingsAll, &QRadioButton::toggled, this, &SettingsPlayback::rememberChanged);
    connect(ui->radioSettingsChannel, &QRadioButton::toggled, this, &SettingsPlayback::rememberChanged);

    ui->comboAspectRatio->setCurrentText(Vlc::ratioHuman()[settings->aspectRatio()]);
    ui->comboCropRatio->setCurrentText(Vlc::ratioHuman()[settings->cropRatio()]);
    ui->comboDeinterlacing->setCurrentIndex(settings->deinterlacing());

    setAudio(settings->audioLanguage());
    setSub(settings->subtitleLanguage());

    connect(ui->comboAspectRatio, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingsPlayback::defaultsChanged);
    connect(ui->comboCropRatio, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingsPlayback::defaultsChanged);
    connect(ui->comboDeinterlacing, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingsPlayback::defaultsChanged);
    connect(ui->comboAudio, &QComboBox::currentTextChanged, this, &SettingsPlayback::defaultsChanged);
    connect(ui->comboSub, &QComboBox::currentTextChanged, this, &SettingsPlayback::defaultsChanged);

#ifndef Q_OS_LINUX
    ui->checkTeletext->hide();
    ui->playbackLayout->removeWidget(ui->checkTeletext);
#else
    ui->checkTeletext->setChecked(settings->teletext());
    connect(ui->checkTeletext, &QCheckBox::toggled, this, &SettingsPlayback::miscChanged);
#endif

    ui->checkSpdif->setChecked(settings->spdif());
    connect(ui->checkSpdif, &QCheckBox::toggled, this, &SettingsPlayback::miscChanged);

#ifndef Q_OS_WIN
    ui->checkYuvRgb->hide();
    ui->playbackLayout->removeWidget(ui->checkYuvRgb);
#else
    ui->checkYuvRgb->setChecked(settings->yuvToRgb());
    connect(ui->checkYuvRgb, &QCheckBox::toggled, this, &SettingsPlayback::miscChanged);
#endif

#ifndef Q_OS_MAC
    ui->comboVout->addItem(tr("Default"));
    for (int i = 1; i < Vlc::videoOutputHuman().size(); i++) {
        ui->comboVout->addItem(Vlc::videoOutputHuman()[i]);
    }
    ui->comboVout->setCurrentIndex(settings->vout());
    connect(ui->comboVout, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &SettingsPlayback::miscChanged);
#else
    ui->labelVout->hide();
    ui->comboVout->hide();
    ui->playbackLayout->removeWidget(ui->labelVout);
    ui->playbackLayout->removeWidget(ui->comboVout);
#endif
}

SettingsPlayback::~SettingsPlayback()
{
    delete ui;
}

void SettingsPlayback::changeEvent(QEvent *e)
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

void SettingsPlayback::rememberChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
    if (ui->radioSettingsChannel->isChecked()) {
        settings->setRememberVideoSettings(false);
        settings->setRememberVideoPerChannel(true);
    } else if (ui->radioSettingsAll->isChecked()) {
        settings->setRememberVideoSettings(true);
        settings->setRememberVideoPerChannel(false);
    } else {
        settings->setRememberVideoSettings(false);
        settings->setRememberVideoPerChannel(false);
    }
    settings->writeSettings();

    emit playbackRememberChanged();
}

void SettingsPlayback::defaultsChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
    if (ui->comboAspectRatio->currentIndex() != 0) {
        settings->setAspectRatio(ui->comboAspectRatio->currentIndex() + 1);
    } else {
        settings->setAspectRatio(0);
    }

    if (ui->comboCropRatio->currentIndex() != 0) {
        settings->setCropRatio(ui->comboCropRatio->currentIndex() + 1);
    } else {
        settings->setCropRatio(0);
    }

    settings->setDeinterlacing(ui->comboDeinterlacing->currentIndex());

    if (ui->comboAudio->currentIndex() != 0) {
        settings->setAudioLanguage(ui->comboAudio->currentText());
    } else {
        settings->setAudioLanguage("");
    }

    if (ui->comboSub->currentIndex() != 0) {
        settings->setSubtitleLanguage(ui->comboSub->currentText());
    } else {
        settings->setSubtitleLanguage("");
    }

    settings->writeSettings();

    emit playbackDefaultsChanged();
}

void SettingsPlayback::miscChanged()
{
    QScopedPointer<Settings> settings(new Settings(this));
#ifdef Q_OS_LINUX
    settings->setTeletext(ui->checkTeletext->isChecked());
#endif
    settings->setSpdif(ui->checkSpdif->isChecked());
#ifdef Q_OS_WIN
    settings->setYuvToRgb(ui->checkYuvRgb->isChecked());
#endif

#ifndef Q_OS_MAC
    settings->setVout(ui->comboVout->currentIndex());
#endif
    settings->writeSettings();

    emit playbackMiscChanged();
}

void SettingsPlayback::setAudio(const QString &audio)
{
    if (audio.isEmpty()) {
        ui->comboAudio->setCurrentIndex(0);
        return;
    }

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

void SettingsPlayback::setSub(const QString &sub)
{
    if (sub.isEmpty()) {
        ui->comboAudio->setCurrentIndex(0);
        return;
    }

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
