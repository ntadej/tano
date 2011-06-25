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

#include "SettingsEdit.h"
#include "ui_SettingsEdit.h"

#include "Config.h"
#include "core/Settings.h"
#include "core/Shortcuts.h"

SettingsEdit::SettingsEdit(Shortcuts *s,
                           QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SettingsEdit)
{
    ui->setupUi(this);

    _settings = new Settings(this);

    read();

#if EDITOR
    ui->setttingsListWidget->item(1)->setHidden(true);
    ui->setttingsListWidget->item(2)->setHidden(true);
    ui->setttingsListWidget->item(4)->setHidden(true);
    ui->setttingsListWidget->item(5)->setHidden(true);
    ui->setttingsListWidget->item(6)->setHidden(true);
#else
    ui->shortcuts->setShortcuts(s);
#endif

    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
}

SettingsEdit::~SettingsEdit()
{
    delete ui;
    delete _settings;
}

void SettingsEdit::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
        case QEvent::LanguageChange:
            ui->retranslateUi(this);
            break;
        default:
            break;
    }
}

void SettingsEdit::action(QAbstractButton *button)
{
    switch(ui->buttonBox->standardButton(button)) {
        case QDialogButtonBox::Save:
            save();
            break;
        case QDialogButtonBox::Cancel:
            cancel();
            break;
        default:
            break;
    }
}

void SettingsEdit::apply()
{
    // General
    _settings->setConfigured(!ui->general->wizard());
    _settings->setSessionVolume(ui->general->sessionVolume());
    _settings->setSessionAutoplay(ui->general->sessionAutoplay());
    _settings->setLanguage(ui->general->language());

    // Channels
    _settings->setPlaylist(ui->channels->playlist());

    // GUI
    _settings->setOsd(ui->gui->osd());
    _settings->setOsdPlaylist(ui->gui->osdPlaylist());
    _settings->setHideToTray(ui->gui->tray());
    _settings->setMouseWheel(ui->gui->wheel());
    _settings->setToolbarLook(ui->gui->toolbar());
    _settings->setSplash(ui->gui->splash());
    _settings->setStartLite(ui->gui->lite());
    _settings->setStartOnTop(ui->gui->top());
    _settings->setStartControls(ui->gui->controls());
    _settings->setStartInfo(ui->gui->info());

    // Playback
    _settings->setGlobalSettings(ui->backend->globalSettings());
    _settings->setRememberVideoSettings(ui->backend->rememberChannelSettings());
    _settings->setAudioLanguage(ui->backend->audio());
    _settings->setSubtitleLanguage(ui->backend->sub());
    _settings->setUdpxy(ui->backend->udpxy());
    _settings->setUdpxyUrl(ui->backend->udpxyUrl());
    _settings->setUdpxyPort(ui->backend->udpxyPort());

    // Schedule
    _settings->setXmltv(ui->schedule->xmltv());
    _settings->setGrabber(ui->schedule->grabber());

    // Recorder
    _settings->setRecorderDirectory(ui->recorder->directory());

    _settings->writeSettings();

    // Shortcuts
#if EDITOR
#else
    ui->shortcuts->shortcutWrite();
#endif
}

void SettingsEdit::cancel()
{
    hide();
}

void SettingsEdit::read()
{
    // General
    ui->general->setWizard(!_settings->configured());
    ui->general->setSessionVolume(_settings->sessionVolume());
    ui->general->setSessionAutoplay(_settings->sessionAutoplay());
    ui->general->setLanguage(_settings->language());

    // Channels
    ui->channels->setPlaylist(_settings->playlist());

    // GUI
    ui->gui->setOsd(_settings->osd());
    ui->gui->setOsdPlaylist(_settings->osdPlaylist());
    ui->gui->setTray(_settings->hideToTray());
    ui->gui->setWheel(_settings->mouseWheel());
    ui->gui->setToolbar(_settings->toolbarLook());
    ui->gui->setSplash(_settings->splash());
    ui->gui->setLite(_settings->startLite());
    ui->gui->setTop(_settings->startOnTop());
    ui->gui->setControls(_settings->startControls());
    ui->gui->setInfo(_settings->startInfo());

    // Playback
    ui->backend->setGlobalSettings(_settings->globalSettings());
    ui->backend->setRememberChannelSettings(_settings->rememberVideoSettings());
    ui->backend->setAudio(_settings->audioLanguage());
    ui->backend->setSub(_settings->subtitleLanguage());
    ui->backend->setUdpxy(_settings->udpxy());
    ui->backend->setUdpxyUrl(_settings->udpxyUrl());
    ui->backend->setUdpxyPort(_settings->udpxyPort());

    // Schedule
    ui->schedule->setXmltv(_settings->xmltv());
    ui->schedule->setGrabber(_settings->grabber());

    // Recorder
    ui->recorder->setDirectory(_settings->recorderDirectory());
}

void SettingsEdit::save()
{
    apply();
    hide();
}
