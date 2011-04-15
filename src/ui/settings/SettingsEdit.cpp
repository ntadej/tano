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

#include <QtCore/QDir>
#include <QtGui/QFileDialog>

#include <vlc-qt/Instance.h>

#include "SettingsEdit.h"
#include "ui_SettingsEdit.h"

#include "Config.h"
#include "core/Common.h"
#include "core/LocaleManager.h"
#include "core/Settings.h"
#include "core/Shortcuts.h"

SettingsEdit::SettingsEdit(Shortcuts *s,
						   QWidget *parent)
	: QDialog(parent),
	ui(new Ui::SettingsEdit)
{
	ui->setupUi(this);
	createActions();

	ui->shortcutsWidget->setShortcuts(s);

	_settings = new Settings(this);

	ui->labelVersion->setText(tr("You are using Tano version:")+" <b>"+Tano::version()+"</b>");
	ui->labelVlcqtVersion->setText(ui->labelVlcqtVersion->text()+" <b>"+VlcInstance::libVersion()+"</b>");
	ui->labelVlcVersion->setText(ui->labelVlcVersion->text()+" <b>"+VlcInstance::version()+"</b>");

	loadLocale();
	read();

#if WITH_RECORDER
#else
	ui->setttingsListWidget->item(5)->setHidden(true);
#endif
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

void SettingsEdit::createActions()
{
	connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));

	connect(ui->buttonBrowseRecorder, SIGNAL(clicked()), this, SLOT(recorderDirectoryBrowse()));
	connect(ui->buttonResetRecorder, SIGNAL(clicked()), this, SLOT(recorderDirectoryReset()));
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
	_settings->setConfigured(!ui->wizardCheck->isChecked());
	_settings->setSessionVolume(ui->sessionVolumeCheck->isChecked());
	_settings->setSessionAutoplay(ui->sessionAutoplayCheck->isChecked());
	if(ui->customLanguageRadio->isChecked())
		_settings->setLanguage(_locale[ui->languageComboBox->currentIndex()]);
	else
		_settings->setLanguage("");

	// Channels
	_settings->setPlaylist(ui->widgetSelectPlaylist->playlist());

	// GUI
	_settings->setOsd(ui->osdCheck->isChecked());
	_settings->setPlaylistOsd(ui->playlistOsdCheck->isChecked());
	_settings->setHideToTray(ui->trayCheck->isChecked());
	if(ui->radioWheelVolume->isChecked())
		_settings->setMouseWheel("volume");
	else if(ui->radioWheelChannel->isChecked())
		_settings->setMouseWheel("channel");
	_settings->setToolbarLook(ui->toolbarLookComboBox->currentIndex());
	_settings->setSplash(ui->checkSplash->isChecked());
	_settings->setStartOnTop(ui->checkTop->isChecked());
	_settings->setStartLite(ui->checkLite->isChecked());
	_settings->setStartControls(ui->checkControls->isChecked());
	_settings->setStartInfo(ui->checkInfo->isChecked());

	// Playback
	_settings->setGlobalSettings(ui->vlcGlobalCheck->isChecked());
	_settings->setRememberVideoSettings(ui->checkVideoSettings->isChecked());
	_settings->setAudioLanguage(ui->comboAudio->currentText());
	_settings->setSubtitleLanguage(ui->comboSub->currentText());

	// Recorder
	_settings->setRecorderDirectory(ui->recorderDirectoryLineEdit->text());

	_settings->writeSettings();

	ui->shortcutsWidget->shortcutWrite();
}

void SettingsEdit::save()
{
	apply();
	hide();
}

void SettingsEdit::cancel()
{
	hide();
}

void SettingsEdit::read()
{
	// General
	ui->wizardCheck->setChecked(!_settings->configured());
	ui->sessionVolumeCheck->setChecked(_settings->sessionVolume());
	ui->sessionAutoplayCheck->setChecked(_settings->sessionAutoplay());
	if(_settings->language() != "") {
		ui->customLanguageRadio->setChecked(true);
		for(int i = 0; i < _locale.size(); i++) {
			if(_settings->language() == _locale[i]) {
				ui->languageComboBox->setCurrentIndex(i);
			}
		}
	}

	// Channels
	ui->widgetSelectPlaylist->setPlaylist(_settings->playlist());

	// GUI
	ui->osdCheck->setChecked(_settings->osd());
	ui->playlistOsdCheck->setChecked(_settings->playlistOsd());
	ui->trayCheck->setChecked(_settings->hideToTray());
	if(_settings->mouseWheel() == "volume")
		ui->radioWheelVolume->setChecked(true);
	else if(_settings->mouseWheel() == "channel")
		ui->radioWheelChannel->setChecked(true);
	ui->toolbarLookComboBox->setCurrentIndex(_settings->toolbarLook());
	ui->checkSplash->setChecked(_settings->splash());
	ui->checkTop->setChecked(_settings->startOnTop());
	ui->checkLite->setChecked(_settings->startLite());
	ui->checkControls->setChecked(_settings->startControls());
	ui->checkInfo->setChecked(_settings->startInfo());

	// Playback
	ui->vlcGlobalCheck->setChecked(_settings->globalSettings());
	ui->checkVideoSettings->setChecked(_settings->rememberVideoSettings());
	for(int i = 0; i < ui->comboAudio->count(); i++) {
		if(ui->comboAudio->itemText(i) == _settings->audioLanguage()) {
			ui->comboAudio->setCurrentIndex(i);
			break;
		} else if(i == ui->comboAudio->count() - 1) {
			ui->comboAudio->setItemText(i, _settings->audioLanguage());
			ui->comboAudio->setCurrentIndex(i);
		}
	}
	for(int i = 0; i < ui->comboSub->count(); i++) {
		if(ui->comboSub->itemText(i) == _settings->subtitleLanguage()) {
			ui->comboSub->setCurrentIndex(i);
			break;
		} else if(i == ui->comboSub->count() - 1) {
			ui->comboSub->setItemText(i, _settings->subtitleLanguage());
			ui->comboSub->setCurrentIndex(i);
		}
	}

	// Recorder
	ui->recorderDirectoryLineEdit->setText(_settings->recorderDirectory());
}

void SettingsEdit::recorderDirectoryReset()
{
	ui->recorderDirectoryLineEdit->setText(Settings::DEFAULT_RECORDER_DIRECTORY);
}

void SettingsEdit::recorderDirectoryBrowse()
{
	QString dir;
	if(ui->recorderDirectoryLineEdit->text().isEmpty())
		dir = QDir::homePath();
	else
		dir = ui->recorderDirectoryLineEdit->text();
	QString file = QFileDialog::getExistingDirectory(this, tr("Open directory"),
													dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui->recorderDirectoryLineEdit->setText(file);
}

void SettingsEdit::loadLocale()
{
	_locale = LocaleManager::loadTranslations();

	for(int i = 0; i < _locale.size(); i++)
		ui->languageComboBox->addItem(LocaleManager::language(_locale[i]));
}
