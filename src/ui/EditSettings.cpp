/****************************************************************************
* EditSettings.cpp: Settings editor
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtCore/QDir>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>

#include "EditSettings.h"
#include "Ver.h"
#include "plugins/PluginsLoader.h"

EditSettings::EditSettings(Shortcuts *s, QWidget *parent)
	: QDialog(parent), _shortcuts(s)
{
	ui.setupUi(this);
	createActions();

	ui.shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	_settings = new Settings(this);

	ui.labelVersion->setText(tr("You are using Tano version:")+" <b>"+Version::Tano()+"</b>");
	ui.labelVlcVersion->setText(ui.labelVlcVersion->text()+" <b>"+Version::libVLC()+"</b>");

	loadPlugins();
	read();
	shortcutRead();
}

EditSettings::~EditSettings()
{
	delete _settings;
}

void EditSettings::createActions()
{
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));

	connect(ui.browsePlaylistButton, SIGNAL(clicked()), this, SLOT(playlistBrowse()));
	connect(ui.resetPlaylistButton, SIGNAL(clicked()), this, SLOT(playlistReset()));

	connect(ui.buttonBrowseRecorder, SIGNAL(clicked()), this, SLOT(recorderDirectoryBrowse()));
	connect(ui.buttonResetRecorder, SIGNAL(clicked()), this, SLOT(recorderDirectoryReset()));

	connect(ui.shortcutsWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(shortcutEdit(QTableWidgetItem*)));
	connect(ui.keyEditor, SIGNAL(keySequenceChanged(const QKeySequence)), this, SLOT(shortcutSequence(const QKeySequence)));
	connect(ui.buttonDefaults, SIGNAL(clicked()), this, SLOT(shortcutRestore()));
	connect(ui.buttonSet, SIGNAL(clicked()), this, SLOT(shortcutSet()));
	connect(ui.buttonClear, SIGNAL(clicked()), this, SLOT(shortcutClear()));
}

void EditSettings::action(QAbstractButton *button)
{
	switch(ui.buttonBox->standardButton(button)) {
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

void EditSettings::save()
{
	// General
	_settings->setConfigured(!ui.wizardCheck->isChecked());
	_settings->setUpdatesCheck(ui.updatesCheck->isChecked());
	_settings->setSession(ui.sessionCheck->isChecked());
	if(ui.customLanguageRadio->isChecked()) {
		if(ui.languageComboBox->currentIndex() == 1)
			_settings->setLanguage("sl_SI");
		else if(ui.languageComboBox->currentIndex() == 0)
			_settings->setLanguage("en_US");
	} else {
		_settings->setLanguage("");
	}

	// Channels
	if(ui.radioSiol2->isChecked())
		_settings->setPlaylist(Settings::PLAYLIST_SIOL_MPEG2);
	else if(ui.radioSiol4->isChecked())
		_settings->setPlaylist(Settings::PLAYLIST_SIOL_MPEG4);
	else if(ui.radioT2->isChecked())
		_settings->setPlaylist(Settings::PLAYLIST_T2);
	else if(ui.radioTus->isChecked())
		_settings->setPlaylist(Settings::PLAYLIST_TUS);
	else if(ui.customPlaylistRadio->isChecked())
		_settings->setPlaylist(ui.playlistLineEdit->text());

	// GUI
	_settings->setOsd(ui.osdCheck->isChecked());
	_settings->setHideToTray(ui.trayCheck->isChecked());
	if(ui.radioWheelVolume->isChecked())
		_settings->setMouseWheel("volume");
	else if(ui.radioWheelChannel->isChecked())
		_settings->setMouseWheel("channel");
	_settings->setToolbarLook(ui.toolbarLookComboBox->currentIndex());
	_settings->setStartOnTop(ui.checkTop->isChecked());
	_settings->setStartLite(ui.checkLite->isChecked());
	_settings->setStartControls(ui.checkControls->isChecked());
	_settings->setStartInfo(ui.checkInfo->isChecked());

	// Playback
	_settings->setGlobalSettings(ui.vlcGlobalCheck->isChecked());
	_settings->setRememberVideoSettings(ui.checkVideoSettings->isChecked());
	_settings->setSubtitleLanguage(ui.comboSub->currentText());

	// Recorder
	_settings->setRecorderEnabled(ui.enableRecorderCheck->isChecked());
	_settings->setRecorderDirectory(ui.recorderDirectoryLineEdit->text());
	_settings->setRecorderPlugin(ui.recorderPluginComboBox->currentText());

	_settings->writeSettings();

	shortcutWrite();

	hide();
}

void EditSettings::cancel()
{
	hide();
}

void EditSettings::read()
{
	// General
	ui.wizardCheck->setChecked(!_settings->configured());
	ui.updatesCheck->setChecked(_settings->updatesCheck());
	ui.sessionCheck->setChecked(_settings->session());
	if(_settings->language() != "") {
		ui.customLanguageRadio->setChecked(true);
		if(_settings->language() == "sl")
			ui.languageComboBox->setCurrentIndex(1);
		else if(_settings->language() == "en")
			ui.languageComboBox->setCurrentIndex(0);
	}

	// Channels
	if(_settings->playlist() == Settings::PLAYLIST_SIOL_MPEG2)
		ui.radioSiol2->setChecked(true);
	else if(_settings->playlist() == Settings::PLAYLIST_SIOL_MPEG4)
		ui.radioSiol4->setChecked(true);
	else if(_settings->playlist() == Settings::PLAYLIST_T2)
		ui.radioT2->setChecked(true);
	else if(_settings->playlist() == Settings::PLAYLIST_TUS)
		ui.radioTus->setChecked(true);
	else {
		ui.customPlaylistRadio->setChecked(true);
		ui.playlistLineEdit->setText(_settings->playlist());
	}

	// GUI
	ui.osdCheck->setChecked(_settings->osd());
	ui.trayCheck->setChecked(_settings->hideToTray());
	if(_settings->mouseWheel() == "volume")
		ui.radioWheelVolume->setChecked(true);
	else if(_settings->mouseWheel() == "channel")
		ui.radioWheelChannel->setChecked(true);
	ui.toolbarLookComboBox->setCurrentIndex(_settings->toolbarLook());
	ui.checkTop->setChecked(_settings->startOnTop());
	ui.checkLite->setChecked(_settings->startLite());
	ui.checkControls->setChecked(_settings->startControls());
	ui.checkInfo->setChecked(_settings->startInfo());

	// Playback
	ui.vlcGlobalCheck->setChecked(_settings->globalSettings());
	ui.checkVideoSettings->setChecked(_settings->rememberVideoSettings());
	for(int i=0; i < ui.comboSub->count(); i++) {
		if(ui.comboSub->itemText(i) == _settings->subtitleLanguage()) {
			ui.comboSub->setCurrentIndex(i);
			break;
		} else if(i == ui.comboSub->count()-1) {
			ui.comboSub->setItemText(i, _settings->subtitleLanguage());
			ui.comboSub->setCurrentIndex(i);
		}
	}

	// Recorder
	ui.enableRecorderCheck->setChecked(_settings->recorderEnabled());
	ui.recorderDirectoryLineEdit->setText(_settings->recorderDirectory());
	for(int i=0; i < ui.recorderPluginComboBox->count(); i++) {
		if(ui.recorderPluginComboBox->itemText(i) == _settings->recorderPlugin()) {
			ui.recorderPluginComboBox->setCurrentIndex(i);
			break;
		}
	}
}

void EditSettings::playlistReset()
{
	ui.playlistLineEdit->setText("");
}

void EditSettings::playlistBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
												QDir::homePath(),
												tr("Tano TV Channel list Files(*.m3u)"));
	ui.playlistLineEdit->setText(file);
}

void EditSettings::recorderDirectoryReset()
{
	ui.recorderDirectoryLineEdit->setText(Settings::DEFAULT_RECORDER_DIRECTORY);
}

void EditSettings::recorderDirectoryBrowse()
{
	QString dir;
	if(ui.recorderDirectoryLineEdit->text().isEmpty())
		dir = QDir::homePath();
	else
		dir = ui.recorderDirectoryLineEdit->text();
	QString file = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
													dir, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	ui.recorderDirectoryLineEdit->setText(file);
}

void EditSettings::shortcutRead()
{
	QStringList keys = _shortcuts->readKeys();
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++)
		ui.shortcutsWidget->item(i,1)->setText(keys[i]);
}

void EditSettings::shortcutRestore()
{
	_shortcuts->restoreDefaults();

	QStringList keys = _shortcuts->readKeys();
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++)
		ui.shortcutsWidget->item(i,1)->setText(keys[i]);
}

void EditSettings::shortcutWrite()
{
	QStringList keys;
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++)
		keys << ui.shortcutsWidget->item(i,1)->text();

	_shortcuts->write(keys);
	_shortcuts->apply();
}

void EditSettings::shortcutEdit(QTableWidgetItem *titem)
{
	if(titem->column() == 1) {
		ui.buttonSet->setEnabled(true);
		ui.keyEditor->setKeySequence(QKeySequence(titem->text()));
		_item = titem;
	}
}

void EditSettings::shortcutSequence(const QKeySequence &s)
{
	_sequence = s;
}

void EditSettings::shortcutSet()
{
	_item->setText(_sequence.toString());
	shortcutClear();
}

void EditSettings::shortcutClear()
{
	ui.keyEditor->setKeySequence(QKeySequence(""));
}

void EditSettings::loadPlugins()
{
	PluginsLoader *loader = new PluginsLoader();
	for(int i=0; i < loader->recorderPlugin().size(); i++)
		ui.recorderPluginComboBox->addItem(loader->recorderName()[i]);
	delete loader;
}
