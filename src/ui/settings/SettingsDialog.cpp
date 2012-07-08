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

#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include "core/Enums.h"
#include "core/Settings.h"

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
    _settings->setSessionVolume(ui->general->sessionVolume());
    _settings->setSessionAutoplay(ui->general->sessionAutoplay());
    _settings->setLanguage(ui->general->language());

    // Playlist
    _settings->setPlaylist(ui->playlist->playlist());
    _settings->setPlaylistUpdate(ui->playlist->playlistUpdate());
    _settings->setPlaylistUpdateUrl(ui->playlist->playlistUpdateUrl());
    _settings->setRadioCategory(ui->playlist->radioCategory());
    _settings->setUdpxy(ui->playlist->udpxy());
    _settings->setUdpxyUrl(ui->playlist->udpxyUrl());
    _settings->setUdpxyPort(ui->playlist->udpxyPort());

    // GUI
    _settings->setOsd(ui->gui->osd());
    _settings->setInfo(ui->gui->info());
    _settings->setTrayEnabled(ui->gui->tray());
    _settings->setHideToTray(ui->gui->trayHide());
    _settings->setMouseWheel(ui->gui->wheel());
    _settings->setToolbarLook(ui->gui->toolbar());
    _settings->setRememberMainSize(ui->gui->rememberSize());
    _settings->setSplash(ui->gui->splash());
    _settings->setStartLite(ui->gui->lite());
    _settings->setStartOnTop(ui->gui->top());
    _settings->setStartControls(ui->gui->osdStart());
    _settings->setStartInfo(ui->gui->infoStart());

    // Playback
    _settings->setVout(ui->backend->vout());
    _settings->setAout(ui->backend->aout());
#if defined(Q_OS_WIN32)
    _settings->setYuvToRgb(ui->backend->yuvToRgb());
#endif
    _settings->setSpdif(ui->backend->spdif());
    _settings->setInterfaceIndex(ui->backend->interface());

    _settings->setRememberVideoSettings(ui->backend->rememberChannelSettings());
    _settings->setAspectRatio(ui->backend->aspectRatio());
    _settings->setCropRatio(ui->backend->cropRatio());
    _settings->setDeinterlacing(ui->backend->deinterlacing());
    _settings->setAudioLanguage(ui->backend->audio());
    _settings->setSubtitleLanguage(ui->backend->sub());

    _settings->setMuteOnMinimize(ui->backend->mute());
    _settings->setTeletext(ui->backend->teletext());

    // Schedule
    _settings->setXmltvLocation(ui->schedule->location());
    _settings->setXmltvUpdate(ui->schedule->update());
    _settings->setXmltvUpdateGrabber(ui->schedule->updateGrabber());
    _settings->setXmltvUpdateUrl(ui->schedule->updateUrl());
    _settings->setXmltvGrabber(ui->schedule->grabber());
    _settings->setXmltvGrabberPath(ui->schedule->grabberPath());

    // Recorder
    _settings->setRecorderDirectory(ui->recorder->directory());
    _settings->setSnapshotsDirectory(ui->recorder->snapshot());

    _settings->writeSettings();
}

void SettingsDialog::defaults()
{
    // General
    ui->general->setSessionVolume(Settings::DEFAULT_SESSION_VOLUME);
    ui->general->setSessionAutoplay(Settings::DEFAULT_SESSION_AUTOPLAY);
    ui->general->setLanguage(Settings::DEFAULT_LANGUAGE);

    // Playlist
    ui->playlist->setPlaylist(Settings::DEFAULT_PLAYLIST);
    ui->playlist->setPlaylistUpdate(Settings::DEFAULT_PLAYLIST_UPDATE);
    ui->playlist->setPlaylistUpdateUrl(Settings::DEFAULT_PLAYLIST_UPDATE_URL);
    ui->playlist->setRadioCategory(Settings::DEFAULT_RADIO_CATEGORY);
    ui->playlist->setUdpxy(Settings::DEFAULT_UDPXY);
    ui->playlist->setUdpxyUrl(Settings::DEFAULT_UDPXY_URL);
    ui->playlist->setUdpxyPort(Settings::DEFAULT_UDPXY_PORT);

    // GUI
    ui->gui->setOsd(Settings::DEFAULT_OSD);
    ui->gui->setInfo(Settings::DEFAULT_INFO);
    ui->gui->setTray(Settings::DEFAULT_TRAY_ENABLED);
    ui->gui->setTrayHide(Settings::DEFAULT_HIDE_TO_TRAY);
    ui->gui->setWheel(Settings::DEFAULT_MOUSE_WHEEL);
    ui->gui->setToolbar(Settings::DEFAULT_TOOLBAR_LOOK);
    ui->gui->setRememberSize(Settings::DEFAULT_REMEMBER_MAIN_SIZE);
    ui->gui->setSplash(Settings::DEFAULT_SPLASH);
    ui->gui->setLite(Settings::DEFAULT_START_LITE);
    ui->gui->setTop(Settings::DEFAULT_START_ON_TOP);
    ui->gui->setOsdStart(Settings::DEFAULT_START_CONTROLS);
    ui->gui->setInfoStart(Settings::DEFAULT_START_INFO);

    // Playback
    ui->backend->setVout(Settings::DEFAULT_VOUT);
    ui->backend->setAout(Settings::DEFAULT_AOUT);
#if defined(Q_OS_WIN32)
    ui->backend->setYuvToRgb(Settings::DEFAULT_YUV_TO_RGB);
#endif
    ui->backend->setSpdif(Settings::DEFAULT_SPDIF);
    ui->backend->setInterface(Settings::DEFAULT_INTERFACE_INDEX);

    ui->backend->setRememberChannelSettings(Settings::DEFAULT_REMEMBER_VIDEO_SETTINGS);
    ui->backend->setAspectRatio(Settings::DEFAULT_ASPECT_RATIO);
    ui->backend->setCropRatio(Settings::DEFAULT_CROP_RATIO);
    ui->backend->setDeinterlacing(Settings::DEFAULT_DEINTERLACING);
    ui->backend->setAudio(Settings::DEFAULT_AUDIO_LANGUAGE);
    ui->backend->setSub(Settings::DEFAULT_SUBTITLE_LANGUAGE);

    ui->backend->setMute(Settings::DEFAULT_MUTE_ON_MINIMIZE);
    ui->backend->setTeletext(Settings::DEFAULT_TELETEXT);

    // Schedule
    ui->schedule->setLocation(Settings::DEFAULT_XMLTV_LOCATION);
    ui->schedule->setUpdate(Settings::DEFAULT_XMLTV_UPDATE);
    ui->schedule->setUpdateGrabber(Settings::DEFAULT_XMLTV_UPDATE_GRABBER);
    ui->schedule->setUpdateUrl(Settings::DEFAULT_XMLTV_UPDATE_URL);
    ui->schedule->setGrabber(Settings::DEFAULT_XMLTV_GRABBER_PATH);

    // Recorder
    ui->recorder->setDirectory(Settings::DEFAULT_RECORDER_DIRECTORY);
    ui->recorder->setSnapshot(Settings::DEFAULT_SNAPSHOTS_DIRECTORY);
}

void SettingsDialog::read()
{
    // General
    ui->general->setSessionVolume(_settings->sessionVolume());
    ui->general->setSessionAutoplay(_settings->sessionAutoplay());
    ui->general->setLanguage(_settings->language());

    // Playlist
    ui->playlist->setPlaylist(_settings->playlist());
    ui->playlist->setPlaylistUpdate(_settings->playlistUpdate());
    ui->playlist->setPlaylistUpdateUrl(_settings->playlistUpdateUrl());
    ui->playlist->setRadioCategory(_settings->radioCategory());
    ui->playlist->setUdpxy(_settings->udpxy());
    ui->playlist->setUdpxyUrl(_settings->udpxyUrl());
    ui->playlist->setUdpxyPort(_settings->udpxyPort());

    // GUI
    ui->gui->setOsd(_settings->osd());
    ui->gui->setInfo(_settings->info());
    ui->gui->setTray(_settings->trayEnabled());
    ui->gui->setTrayHide(_settings->hideToTray());
    ui->gui->setWheel(_settings->mouseWheel());
    ui->gui->setToolbar(_settings->toolbarLook());
    ui->gui->setRememberSize(_settings->rememberMainSize());
    ui->gui->setSplash(_settings->splash());
    ui->gui->setLite(_settings->startLite());
    ui->gui->setTop(_settings->startOnTop());
    ui->gui->setOsdStart(_settings->startControls());
    ui->gui->setInfoStart(_settings->startInfo());

    // Playback
    ui->backend->setVout(_settings->vout());
    ui->backend->setAout(_settings->aout());
#if defined(Q_OS_WIN32)
    ui->backend->setYuvToRgb(_settings->yuvToRgb());
#endif
    ui->backend->setSpdif(_settings->spdif());
    ui->backend->setInterface(_settings->interfaceIndex());

    ui->backend->setRememberChannelSettings(_settings->rememberVideoSettings());
    ui->backend->setAspectRatio(_settings->aspectRatio());
    ui->backend->setCropRatio(_settings->cropRatio());
    ui->backend->setDeinterlacing(_settings->deinterlacing());
    ui->backend->setAudio(_settings->audioLanguage());
    ui->backend->setSub(_settings->subtitleLanguage());

    ui->backend->setMute(_settings->muteOnMinimize());
    ui->backend->setTeletext(_settings->teletext());

    // Schedule
    ui->schedule->setLocation(_settings->xmltvLocation());
    ui->schedule->setUpdate(_settings->xmltvUpdate());
    ui->schedule->setUpdateGrabber(_settings->xmltvUpdateGrabber());
    ui->schedule->setUpdateUrl(_settings->xmltvUpdateUrl());
    ui->schedule->setGrabber(_settings->xmltvGrabberPath());

    // Recorder
    ui->recorder->setDirectory(_settings->recorderDirectory());
    ui->recorder->setSnapshot(_settings->snapshotsDirectory());
}

void SettingsDialog::save()
{
    apply();
    hide();
}
