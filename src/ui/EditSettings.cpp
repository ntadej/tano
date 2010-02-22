/****************************************************************************
* EditSettings.cpp: _settings editor
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
#include <QtGui/QTextEdit>

#include "EditSettings.h"
#include "../Common.h"
#include "../Ver.h"
#include "../plugins/PluginsLoader.h"

EditSettings::EditSettings(QWidget *parent, Shortcuts *s)
	: QDialog(parent), _shortcuts(s)
{
	ui.setupUi(this);
	createActions();

	ui.buttonSet->setEnabled(false);
	ui.buttonBrowse->setEnabled(false);
	ui.buttonReset->setEnabled(false);
	ui.editNetwork->setEnabled(false);

	ui.shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

	_keysList = _shortcuts->defaultKeys();
	_actionsList = _shortcuts->actionsNames();

	_settings = Common::settings();
	_settings->sync();

	ui.labelVersion->setText(tr("You are using Tano version:")+" "+Version::Tano());
	ui.labelVlcVersion->setText(ui.labelVlcVersion->text()+" "+Version::libVLC());

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

	connect(ui.radioCustomLanguage, SIGNAL(clicked()), this, SLOT(toggleCustom()));
	connect(ui.radioDefaultLanguage, SIGNAL(clicked()), this, SLOT(toggleCustom()));

	connect(ui.radioPresetPlaylist, SIGNAL(clicked()), this, SLOT(togglePlaylist()));
	connect(ui.radioCustomPlaylist, SIGNAL(clicked()), this, SLOT(togglePlaylist()));

	connect(ui.radioNetworkDefault, SIGNAL(clicked()), this, SLOT(toggleNetwork()));
	connect(ui.radioNetworkCustom, SIGNAL(clicked()), this, SLOT(toggleNetwork()));

	connect(ui.buttonBrowse, SIGNAL(clicked()), this, SLOT(playlistBrowse()));
	connect(ui.buttonReset, SIGNAL(clicked()), this, SLOT(playlistReset()));

	connect(ui.buttonBrowseRecorder, SIGNAL(clicked()), this, SLOT(dirBrowse()));
	connect(ui.buttonResetRecorder, SIGNAL(clicked()), this, SLOT(dirReset()));

	connect(ui.shortcutsWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(shortcutEdit(QTableWidgetItem*)));
	connect(ui.keyEditor, SIGNAL(keySequenceChanged(const QKeySequence)), this, SLOT(shortcutSequence(const QKeySequence)));
	connect(ui.buttonDefaults, SIGNAL(clicked()), this, SLOT(shortcutRestore()));
	connect(ui.buttonSet, SIGNAL(clicked()), this, SLOT(shortcutSet()));
	connect(ui.buttonClear, SIGNAL(clicked()), this, SLOT(shortcutClear()));
}

void EditSettings::action(QAbstractButton *button)
{
	switch(ui.buttonBox->standardButton(button)) {
	case 0x00000800:
		ok();
		break;
	case 0x00400000:
		cancel();
		break;
	default:
		break;
	}
}

void EditSettings::ok()
{
	if (ui.radioDefaultLanguage->isChecked()) {
		_settings->remove("locale");
	} else {
		if(ui.languageComboBox->currentIndex() == 1)
			_settings->setValue("locale","sl");
		else if(ui.languageComboBox->currentIndex() == 0)
			_settings->setValue("locale","en");
	}

	_settings->setValue("registered",!ui.checkWizard->isChecked());
	_settings->setValue("updates",ui.checkUpdates->isChecked());
	_settings->setValue("session",ui.checkSession->isChecked());

	if(ui.radioSiol2->isChecked()) {
		_settings->setValue("playlist","playlists/siol-mpeg2.m3u");
	} else if(ui.radioSiol4->isChecked()) {
		_settings->setValue("playlist","playlists/siol-mpeg4.m3u");
	} else if(ui.radioT2->isChecked()) {
		_settings->setValue("playlist","playlists/t-2.m3u");
	} else if(ui.radioTus->isChecked()) {
		_settings->setValue("playlist","playlists/tus.m3u");
	} else {
		_settings->setValue("playlist",ui.editPlaylist->text());
	}

	_settings->beginGroup("VLC");
	if(ui.radioNetworkCustom->isChecked())
		_settings->setValue("network", ui.editNetwork->text());
	else
		_settings->remove("network");

	_settings->setValue("ignore-config", !ui.checkVlc->isChecked());
	_settings->setValue("remember-video-config", ui.checkVideoSettings->isChecked());
	_settings->setValue("default-sub-lang", ui.comboSub->currentText());
	_settings->endGroup();

	_settings->beginGroup("GUI");
	_settings->setValue("lite",ui.checkLite->isChecked());
	_settings->setValue("ontop",ui.checkTop->isChecked());
	_settings->setValue("OSD",ui.checkOsd->isChecked());
	_settings->setValue("info",ui.checkInfo->isChecked());
	_settings->setValue("controls",ui.checkControls->isChecked());
	_settings->setValue("wheel",ui.radioWheelVolume->isChecked());
	_settings->setValue("tray",ui.radioTray->isChecked());
	_settings->endGroup();

	_settings->beginGroup("Recorder");
	if(ui.checkRecorder->isChecked()) {
		_settings->setValue("enabled",true);
		if(ui.editRecorder->text() != "")
			_settings->setValue("dir",ui.editRecorder->text());
		_settings->setValue("backend",ui.comboRecorderPlugin->currentText());
	} else {
		_settings->setValue("enabled",false);
		_settings->remove("dir");
		_settings->remove("backend");
	}
	_settings->endGroup();

	_settings->beginGroup("Shortcuts");
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++)
		if(ui.shortcutsWidget->item(i,1)->text() != _keysList.at(i))
			_settings->setValue(_actionsList.at(i),ui.shortcutsWidget->item(i,1)->text());
	_settings->endGroup();

	_shortcuts->apply();
	hide();
}

void EditSettings::cancel()
{
	hide();
}

void EditSettings::read()
{
	if(_settings->value("locale","Default").toString() != "Default")
	{
		ui.radioCustomLanguage->setChecked(true);
		ui.languageComboBox->setEnabled(true);
		bool okint;
		if(_settings->value("locale").toString() == "sl")
			ui.languageComboBox->setCurrentIndex(1);
		else if(_settings->value("locale").toString() == "en")
			ui.languageComboBox->setCurrentIndex(0);
	}

	ui.checkUpdates->setChecked(_settings->value("updates",true).toBool());
	ui.checkSession->setChecked(_settings->value("session",true).toBool());

	if(_settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/siol-mpeg2.m3u")
		ui.radioSiol2->setChecked(true);
	else if(_settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/siol-mpeg4.m3u")
		ui.radioSiol4->setChecked(true);
	else if(_settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/t-2.m3u")
		ui.radioT2->setChecked(true);
	else if(_settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/tus.m3u")
		ui.radioTus->setChecked(true);
	else {
		ui.radioPresetPlaylist->setChecked(true);
		ui.buttonBrowse->setEnabled(true);
		ui.buttonReset->setEnabled(true);
		ui.presetsBox->setEnabled(false);
		ui.editPlaylist->setText(_settings->value("playlist").toString());
	}

	_settings->beginGroup("VLC");
	if(_settings->value("network","").toString() != "")
	{
		ui.radioNetworkCustom->setChecked(true);
		ui.editNetwork->setText(_settings->value("network","").toString());
		ui.editNetwork->setEnabled(true);
	} else {
		ui.radioNetworkDefault->setChecked(true);
	}
	ui.checkVlc->setChecked(!_settings->value("ignore-config",true).toBool());
	ui.checkVideoSettings->setChecked(_settings->value("remember-video-config", false).toBool());
	for(int i=0;i<ui.comboSub->count();i++) {
		if(ui.comboSub->itemText(i)==_settings->value("default-sub-lang", tr("Disabled")).toString()) {
			ui.comboSub->setCurrentIndex(i);
			break;
		} else if(i == ui.comboSub->count()-1) {
			ui.comboSub->setItemText(i, _settings->value("default-sub-lang", tr("Disabled")).toString());
			ui.comboSub->setCurrentIndex(i);
		}
	}
	_settings->endGroup();

	_settings->beginGroup("GUI");
	ui.checkLite->setChecked(_settings->value("lite",false).toBool());
	ui.checkTop->setChecked(_settings->value("ontop",false).toBool());
	ui.checkOsd->setChecked(_settings->value("OSD",true).toBool());
	ui.checkControls->setChecked(_settings->value("OSD",true).toBool());
	ui.checkInfo->setChecked(_settings->value("info",true).toBool());
	ui.radioWheelChannel->setChecked(!_settings->value("wheel",false).toBool());
	ui.radioWheelVolume->setChecked(_settings->value("wheel",false).toBool());
	ui.radioExit->setChecked(!_settings->value("tray",false).toBool());
	_settings->endGroup();

	_settings->beginGroup("Recorder");
	if(_settings->value("enabled",true).toBool()) {
		ui.checkRecorder->toggle();
		ui.editRecorder->setText(_settings->value("dir","").toString());
		for(int i=0;i<ui.comboRecorderPlugin->count();i++) {
			if(ui.comboRecorderPlugin->itemText(i)==_settings->value("backend", "FripPlugin").toString()) {
				ui.comboSub->setCurrentIndex(i);
				break;
			}
		}
	}
	_settings->endGroup();
}

void EditSettings::toggleCustom()
{
	ui.languageComboBox->setEnabled(ui.radioCustomLanguage->isChecked());
}

void EditSettings::toggleNetwork()
{
	ui.editNetwork->setEnabled(!ui.radioNetworkDefault->isChecked());
}

void EditSettings::togglePlaylist()
{
	ui.buttonBrowse->setEnabled(ui.radioCustomPlaylist->isChecked());
	ui.buttonReset->setEnabled(ui.radioCustomPlaylist->isChecked());
	ui.radioSiol2->setCheckable(!ui.radioCustomPlaylist->isChecked());
	ui.radioSiol4->setCheckable(!ui.radioCustomPlaylist->isChecked());
	ui.radioT2->setCheckable(!ui.radioCustomPlaylist->isChecked());
	ui.radioTus->setCheckable(!ui.radioCustomPlaylist->isChecked());
	ui.presetsBox->setEnabled(!ui.radioCustomPlaylist->isChecked());
}

void EditSettings::playlistBrowse()
{
	QString file = QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
												QDir::homePath(),
												tr("Tano TV Channel list Files(*.m3u)"));
	ui.editPlaylist->setText(file);
}

void EditSettings::dirReset()
{
	ui.editRecorder->setText("");
}

void EditSettings::dirBrowse()
{
	QString dir;
	if(ui.editRecorder->text() == "")
		dir = QDir::homePath();
	else
		dir = ui.editRecorder->text();
	QString dfile = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
															dir,
															QFileDialog::ShowDirsOnly
															| QFileDialog::DontResolveSymlinks);
	ui.editRecorder->setText(dfile);
}

void EditSettings::playlistReset()
{
	ui.editPlaylist->setText("");
}

void EditSettings::shortcutRead()
{
	_settings->beginGroup("Shortcuts");
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++)
		ui.shortcutsWidget->item(i,1)->setText(_settings->value(_actionsList.at(i),_keysList.at(i)).toString());
	_settings->endGroup();
}

void EditSettings::shortcutRestore()
{
	_settings->beginGroup("Shortcuts");
	for(int i=0; i < _actionsList.size(); i++) {
		_settings->remove(_actionsList.at(i));
		ui.shortcutsWidget->item(i,1)->setText(_keysList.at(i));
	}
	_settings->endGroup();
}

void EditSettings::shortcutEdit(QTableWidgetItem *titem)
{
	if(titem->column() == 1) {
		ui.buttonSet->setEnabled(true);
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
		ui.comboRecorderPlugin->addItem(loader->recorderName()[i]);
	delete loader;
}
