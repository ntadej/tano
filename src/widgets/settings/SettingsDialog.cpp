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
#include "settings/Settings.h"

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    _settings = new Settings(this);

    _buttonGroup = new QButtonGroup(this);
    _buttonGroup->setExclusive(true);

    _buttonGroup->addButton(ui->buttonGeneral, 0);
    _buttonGroup->addButton(ui->buttonPlaylist, 1);
    _buttonGroup->addButton(ui->buttonInterface, 2);
    _buttonGroup->addButton(ui->buttonPlayback, 3);
    _buttonGroup->addButton(ui->buttonSchedule, 4);
    _buttonGroup->addButton(ui->buttonRecorder, 5);

    read();

    connect(_buttonGroup, SIGNAL(buttonClicked(int)), ui->settingsWidget, SLOT(setCurrentIndex(int)));
    connect(ui->general, SIGNAL(resetDefaults()), this, SLOT(defaults()));
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
    delete _buttonGroup;
    delete _settings;
}

void SettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::action(QAbstractButton *button)
{
    switch(ui->buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Save:
        save();
        break;
    case QDialogButtonBox::Cancel:
        close();
        break;
    default:
        break;
    }
}

void SettingsDialog::apply()
{
    // General
    _settings->setSessionRememberVolume(ui->general->sessionVolume());
    _settings->setSessionAutoplay(ui->general->sessionAutoplay());
    _settings->setLanguage(ui->general->language());

    // Playlist
    if (ui->buttonPlaylist->isEnabled()) {
        _settings->setPlaylist(ui->playlist->playlist());
        _settings->setPlaylistUpdate(ui->playlist->playlistUpdate());
        _settings->setPlaylistUpdateUrl(ui->playlist->playlistUpdateUrl());
        _settings->setRadioCategory(ui->playlist->radioCategory());
        _settings->setHdCategory(ui->playlist->hdCategory());
        _settings->setUdpxy(ui->playlist->udpxy());
        _settings->setUdpxyUrl(ui->playlist->udpxyUrl());
        _settings->setUdpxyPort(ui->playlist->udpxyPort());
    }

    // GUI
    if (ui->buttonInterface->isEnabled()) {
        _settings->setOsd(ui->gui->osd());
        _settings->setTrayEnabled(ui->gui->tray());
        _settings->setHideToTray(ui->gui->trayHide());
        _settings->setMouseWheel(ui->gui->wheel());
        _settings->setRememberGuiSession(ui->gui->rememberSize());
        _settings->setIcons(ui->gui->icons());
    }

    // Playback
    if (ui->buttonPlayback->isEnabled()) {
        _settings->setVout(ui->backend->vout());
        _settings->setAout(ui->backend->aout());
#if defined(Q_OS_WIN32)
        _settings->setYuvToRgb(ui->backend->yuvToRgb());
#endif
        _settings->setSpdif(ui->backend->spdif());

        _settings->setRememberVideoSettings(ui->backend->rememberChannelSettings());
        _settings->setRememberVideoPerChannel(ui->backend->rememberPerChannel());
        _settings->setAspectRatio(ui->backend->aspectRatio());
        _settings->setCropRatio(ui->backend->cropRatio());
        _settings->setDeinterlacing(ui->backend->deinterlacing());
        _settings->setAudioLanguage(ui->backend->audio());
        _settings->setSubtitleLanguage(ui->backend->sub());

        _settings->setMuteOnMinimize(ui->backend->mute());
        _settings->setTeletext(ui->backend->teletext());
    }

    // Schedule
    if (ui->buttonSchedule->isEnabled()) {
        _settings->setXmltvUpdate(ui->schedule->update());
        _settings->setXmltvUpdateLocation(ui->schedule->location());
        _settings->setXmltvUpdateRemote(ui->schedule->remote());
        _settings->setXmltvHoursOffset(ui->schedule->offset());
    }

    // Recorder
    if (ui->buttonRecorder->isEnabled()) {
        _settings->setRecorderDirectory(ui->recorder->directory());
        _settings->setSnapshotsDirectory(ui->recorder->snapshot());
    }

    _settings->writeSettings();
}

void SettingsDialog::defaults()
{
    // General
    ui->general->setSessionVolume(_settings->defaultValue(Settings::KEY_SESSION_REMEMBER_VOLUME).toInt());
    ui->general->setSessionAutoplay(_settings->defaultValue(Settings::KEY_SESSION_AUTOPLAY).toBool());
    ui->general->setLanguage(_settings->defaultValue(Settings::KEY_LANGUAGE).toString());

    // Playlist
    ui->playlist->setPlaylist(_settings->defaultValue(Settings::KEY_PLAYLIST).toString());
    ui->playlist->setPlaylistUpdate(_settings->defaultValue(Settings::KEY_PLAYLIST_UPDATE).toBool());
    ui->playlist->setPlaylistUpdateUrl(_settings->defaultValue(Settings::KEY_PLAYLIST_UPDATE_URL).toString());
    ui->playlist->setRadioCategory(_settings->defaultValue(Settings::KEY_RADIO_CATEGORY).toString());
    ui->playlist->setHdCategory(_settings->defaultValue(Settings::KEY_HD_CATEGORY).toString());
    ui->playlist->setUdpxy(_settings->defaultValue(Settings::KEY_UDPXY).toBool());
    ui->playlist->setUdpxyUrl(_settings->defaultValue(Settings::KEY_UDPXY_URL).toString());
    ui->playlist->setUdpxyPort(_settings->defaultValue(Settings::KEY_UDPXY_PORT).toInt());

    // GUI
    ui->gui->setOsd(_settings->defaultValue(Settings::KEY_OSD).toBool());
    ui->gui->setTray(_settings->defaultValue(Settings::KEY_TRAY_ENABLED).toBool());
    ui->gui->setTrayHide(_settings->defaultValue(Settings::KEY_HIDE_TO_TRAY).toBool());
    ui->gui->setWheel(_settings->defaultValue(Settings::KEY_MOUSE_WHEEL).toString());
    ui->gui->setRememberSize(_settings->defaultValue(Settings::KEY_REMEMBER_GUI_SESSION).toBool());
    ui->gui->setIcons(_settings->defaultValue(Settings::KEY_ICONS).toString());

    // Playback
    ui->backend->setVout(_settings->defaultValue(Settings::KEY_VOUT).toInt());
    ui->backend->setAout(_settings->defaultValue(Settings::KEY_AOUT).toInt());
#if defined(Q_OS_WIN32)
    ui->backend->setYuvToRgb(_settings->defaultValue(Settings::KEY_YUV_TO_RGB).toBool());
#endif
    ui->backend->setSpdif(_settings->defaultValue(Settings::KEY_SPDIF).toBool());

    ui->backend->setRememberChannelSettings(_settings->defaultValue(Settings::KEY_REMEMBER_VIDEO_SETTINGS).toBool());
    ui->backend->setRememberPerChannel(_settings->defaultValue(Settings::KEY_REMEMBER_VIDEO_PER_CHANNEL).toBool());
    ui->backend->setAspectRatio(_settings->defaultValue(Settings::KEY_ASPECT_RATIO).toInt());
    ui->backend->setCropRatio(_settings->defaultValue(Settings::KEY_CROP_RATIO).toInt());
    ui->backend->setDeinterlacing(_settings->defaultValue(Settings::KEY_DEINTERLACING).toInt());
    ui->backend->setAudio(_settings->defaultValue(Settings::KEY_AUDIO_LANGUAGE).toString());
    ui->backend->setSub(_settings->defaultValue(Settings::KEY_SUBTITLE_LANGUAGE).toString());

    ui->backend->setMute(_settings->defaultValue(Settings::KEY_MUTE_ON_MINIMIZE).toBool());
    ui->backend->setTeletext(_settings->defaultValue(Settings::KEY_TELETEXT).toBool());

    // Schedule
    ui->schedule->setUpdate(_settings->defaultValue(Settings::KEY_XMLTV_UPDATE).toBool());
    ui->schedule->setLocation(_settings->defaultValue(Settings::KEY_XMLTV_UPDATE_LOCATION).toString());
    ui->schedule->setRemote(_settings->defaultValue(Settings::KEY_XMLTV_UPDATE_REMOTE).toBool());
    ui->schedule->setOffset(_settings->defaultValue(Settings::KEY_XMLTV_HOURS_OFFSET).toInt());

    // Recorder
    ui->recorder->setDirectory(_settings->defaultValue(Settings::KEY_RECORDER_DIRECTORY).toString());
    ui->recorder->setSnapshot(_settings->defaultValue(Settings::KEY_SNAPSHOTS_DIRECTORY).toString());
}

void SettingsDialog::read()
{
    // General
    ui->general->setSessionVolume(_settings->sessionRememberVolume());
    ui->general->setSessionAutoplay(_settings->sessionAutoplay());
    ui->general->setLanguage(_settings->language());

    // Playlist
    ui->playlist->setPlaylist(_settings->playlist());
    ui->playlist->setPlaylistUpdate(_settings->playlistUpdate());
    ui->playlist->setPlaylistUpdateUrl(_settings->playlistUpdateUrl());
    ui->playlist->setRadioCategory(_settings->radioCategory());
    ui->playlist->setHdCategory(_settings->hdCategory());
    ui->playlist->setUdpxy(_settings->udpxy());
    ui->playlist->setUdpxyUrl(_settings->udpxyUrl());
    ui->playlist->setUdpxyPort(_settings->udpxyPort());

    // GUI
    ui->gui->setOsd(_settings->osd());
    ui->gui->setTray(_settings->trayEnabled());
    ui->gui->setTrayHide(_settings->hideToTray());
    ui->gui->setWheel(_settings->mouseWheel());
    ui->gui->setRememberSize(_settings->rememberGuiSession());
    ui->gui->setIcons(_settings->icons());

    // Playback
    ui->backend->setVout(_settings->vout());
    ui->backend->setAout(_settings->aout());
#if defined(Q_OS_WIN32)
    ui->backend->setYuvToRgb(_settings->yuvToRgb());
#endif
    ui->backend->setSpdif(_settings->spdif());

    ui->backend->setRememberChannelSettings(_settings->rememberVideoSettings());
    ui->backend->setRememberPerChannel(_settings->rememberVideoPerChannel());
    ui->backend->setAspectRatio(_settings->aspectRatio());
    ui->backend->setCropRatio(_settings->cropRatio());
    ui->backend->setDeinterlacing(_settings->deinterlacing());
    ui->backend->setAudio(_settings->audioLanguage());
    ui->backend->setSub(_settings->subtitleLanguage());

    ui->backend->setMute(_settings->muteOnMinimize());
    ui->backend->setTeletext(_settings->teletext());

    // Schedule
    ui->schedule->setUpdate(_settings->xmltvUpdate());
    ui->schedule->setLocation(_settings->xmltvUpdateLocation());
    ui->schedule->setRemote(_settings->xmltvUpdateRemote());
    ui->schedule->setOffset(_settings->xmltvHoursOffset());

    // Recorder
    ui->recorder->setDirectory(_settings->recorderDirectory());
    ui->recorder->setSnapshot(_settings->snapshotsDirectory());
}

void SettingsDialog::save()
{
    apply();
    hide();
}
