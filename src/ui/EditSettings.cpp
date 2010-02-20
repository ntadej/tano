#include <QtGui>
#include <QMessageBox>
#include <QTextEdit>
#include <QDir>
#include <QFileDialog>
#include <QVlc/VlcInstance.h>

#include "EditSettings.h"
#include "../Common.h"
#include "../Ver.h"

EditSettings::EditSettings(QWidget *parent, Shortcuts *s)
	: QDialog(parent), shortcuts(s)
{
	ui.setupUi(this);
	createActions();

	ui.buttonSet->setEnabled(false);
	ui.buttonBrowse->setEnabled(false);
	ui.buttonReset->setEnabled(false);
	ui.editNetwork->setEnabled(false);

	ui.shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.shortcutsWidget->verticalHeader()->hide();

	keysList = shortcuts->defaultKeys();
	actionsList = shortcuts->actionsNames();

	settings = Common::settings();
	settings->sync();

	ui.labelVersion->setText(tr("You are using Tano version:")+" "+Version::Tano());
	ui.labelVlcVersion->setText(ui.labelVlcVersion->text()+" "+VlcInstance::version());

	read();
	shortcutRead();
}

EditSettings::~EditSettings()
{
	delete settings;
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
		settings->remove("locale");
	} else {
		if(ui.languageComboBox->currentIndex() == 1)
			settings->setValue("locale","sl");
		else if(ui.languageComboBox->currentIndex() == 0)
			settings->setValue("locale","en");
	}

	settings->setValue("registered",!ui.checkWizard->isChecked());
	settings->setValue("updates",ui.checkUpdates->isChecked());
	settings->setValue("session",ui.checkSession->isChecked());

	if(ui.radioSiol2->isChecked()) {
		settings->setValue("playlist","playlists/siol-mpeg2.m3u");
	} else if(ui.radioSiol4->isChecked()) {
		settings->setValue("playlist","playlists/siol-mpeg4.m3u");
	} else if(ui.radioT2->isChecked()) {
		settings->setValue("playlist","playlists/t-2-cat.m3u");
	} else if(ui.radioT2full->isChecked()) {
			settings->setValue("playlist","playlists/t-2-full.m3u");
	} else {
		settings->setValue("playlist",ui.editPlaylist->text());
	}

	settings->beginGroup("VLC");
	if(ui.radioNetworkCustom->isChecked()) {
		settings->setValue("network", ui.editNetwork->text());
	} else {
		settings->remove("network");
	}
	if(ui.checkVlc->isChecked()) {
		settings->setValue("ignore-config", false);
	} else {
		settings->remove("ignore-config");
	}
	if(ui.checkVideoSettings->isChecked()) {
		settings->setValue("remember-video-config", true);
	} else {
		settings->remove("remember-video-config");
	}
	settings->setValue("default-sub-lang", ui.comboSub->currentText());
	settings->endGroup();

	settings->beginGroup("GUI");
	if(ui.checkLite->isChecked()) {
		settings->setValue("lite",true);
	} else {
		settings->setValue("lite",false);
	}
	if(ui.checkTop->isChecked()) {
		settings->setValue("ontop",true);
	} else {
		settings->setValue("ontop",false);
	}
	if(ui.checkOsd->isChecked()) {
		settings->setValue("OSD",true);
	} else {
		settings->setValue("OSD",false);
	}
	if(ui.checkInfo->isChecked()) {
		settings->setValue("info",true);
	} else {
		settings->setValue("info",false);
	}
	if(ui.radioWheelVolume->isChecked()) {
		settings->setValue("wheel",true);
	} else {
		settings->setValue("wheel",false);
	}
	if(ui.radioTray->isChecked()) {
		settings->setValue("tray",true);
	} else {
		settings->setValue("tray",false);
	}
	settings->endGroup();

	settings->beginGroup("Recorder");
	if(ui.checkRecorder->isChecked()) {
		settings->setValue("enabled",true);
		if(ui.editRecorder->text() != "")
			settings->setValue("dir",ui.editRecorder->text());
	} else {
		settings->setValue("enabled",false);
		settings->remove("dir");
	}
	settings->endGroup();

	settings->beginGroup("Shortcuts");
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++) {
		item = ui.shortcutsWidget->item(i,1);
		if(item->text() != keysList.at(i))
			settings->setValue(actionsList.at(i),item->text());
	}
	settings->endGroup();

	shortcuts->apply();
	hide();
}

void EditSettings::cancel()
{
	hide();
}

void EditSettings::read()
{
	if(settings->value("locale","Default").toString() != "Default")
	{
		ui.radioCustomLanguage->setChecked(true);
		ui.languageComboBox->setEnabled(true);
		bool okint;
		if(settings->value("locale").toString() == "sl")
			ui.languageComboBox->setCurrentIndex(1);
		else if(settings->value("locale").toString() == "en")
			ui.languageComboBox->setCurrentIndex(0);
	}

	ui.checkUpdates->setChecked(settings->value("updates",true).toBool());
	ui.checkSession->setChecked(settings->value("session",true).toBool());

	if(settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/siol-mpeg2.m3u")
		ui.radioSiol2->setChecked(true);
	else if(settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/siol-mpeg4.m3u")
		ui.radioSiol4->setChecked(true);
	else if(settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/t-2-cat.m3u")
		ui.radioT2->setChecked(true);
	else if(settings->value("playlist","playlists/siol-mpeg2.m3u").toString() == "playlists/t-2-full.m3u")
			ui.radioT2full->setChecked(true);
	else {
		ui.radioPresetPlaylist->setChecked(true);
		ui.buttonBrowse->setEnabled(true);
		ui.buttonReset->setEnabled(true);
		ui.presetsBox->setEnabled(false);
		ui.editPlaylist->setText(settings->value("playlist").toString());
	}

	settings->beginGroup("VLC");
	if(settings->value("network","").toString() != "")
	{
		ui.radioNetworkCustom->setChecked(true);
		ui.editNetwork->setText(settings->value("network","").toString());
		ui.editNetwork->setEnabled(true);
	} else {
		ui.radioNetworkDefault->setChecked(true);
	}
	ui.checkVlc->setChecked(!settings->value("ignore-config",true).toBool());
	ui.checkVideoSettings->setChecked(settings->value("remember-video-config", false).toBool());
	for(int i=0;i<ui.comboSub->count();i++) {
		if(ui.comboSub->itemText(i)==settings->value("default-sub-lang", tr("Disabled")).toString()) {
			ui.comboSub->setCurrentIndex(i);
			break;
		} else if(i == ui.comboSub->count()-1) {
			ui.comboSub->setItemText(i, settings->value("default-sub-lang", tr("Disabled")).toString());
			ui.comboSub->setCurrentIndex(i);
		}
	}
	settings->endGroup();

	settings->beginGroup("GUI");
	ui.checkLite->setChecked(settings->value("lite",false).toBool());
	ui.checkTop->setChecked(settings->value("ontop",false).toBool());
	ui.checkOsd->setChecked(settings->value("OSD",true).toBool());
	ui.checkInfo->setChecked(settings->value("info",true).toBool());
	ui.radioWheelChannel->setChecked(!settings->value("wheel",false).toBool());
	ui.radioWheelVolume->setChecked(settings->value("wheel",false).toBool());
	ui.radioExit->setChecked(!settings->value("tray",false).toBool());
	settings->endGroup();

	settings->beginGroup("Recorder");
	if(settings->value("enabled",true).toBool()) {
		ui.checkRecorder->toggle();
		ui.editRecorder->setText(settings->value("dir","").toString());
	}
	settings->endGroup();
}

void EditSettings::toggleCustom()
{
	if (ui.radioCustomLanguage->isChecked())
	{
		ui.languageComboBox->setEnabled(true);
	} else
	{
		ui.languageComboBox->setEnabled(false);
	}
}

void EditSettings::toggleNetwork()
{
	if (ui.radioNetworkDefault->isChecked())
	{
		ui.editNetwork->setEnabled(false);
	} else
	{
		ui.editNetwork->setEnabled(true);
	}
}

void EditSettings::togglePlaylist()
{
	if (ui.radioCustomPlaylist->isChecked()) {
		ui.buttonBrowse->setEnabled(true);
		ui.buttonReset->setEnabled(true);
		ui.radioSiol2->setCheckable(false);
		ui.radioSiol4->setCheckable(false);
		ui.radioT2->setCheckable(false);
		ui.radioT2full->setCheckable(false);
		ui.presetsBox->setEnabled(false);
	} else {
		ui.buttonBrowse->setEnabled(false);
		ui.buttonReset->setEnabled(false);
		ui.radioSiol2->setCheckable(true);
		ui.radioSiol4->setCheckable(true);
		ui.radioT2->setCheckable(true);
		ui.radioT2full->setCheckable(true);
		ui.presetsBox->setEnabled(true);
	}
}

void EditSettings::playlistBrowse()
{
	QString dfile = QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
												QDir::homePath(),
												tr("Tano TV Channel list Files(*.m3u)"));
	ui.editPlaylist->setText(dfile);
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
	settings->beginGroup("Shortcuts");
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++) {
		item = ui.shortcutsWidget->item(i,1);
		item->setText(settings->value(actionsList.at(i),keysList.at(i)).toString());
	}
	settings->endGroup();
}

void EditSettings::shortcutRestore()
{
	settings->beginGroup("Shortcuts");
	for(int i=0; i < actionsList.size(); i++) {
		settings->remove(actionsList.at(i));
		item = ui.shortcutsWidget->item(i,1);
		item->setText(keysList.at(i));
	}
	settings->endGroup();
}

void EditSettings::shortcutEdit(QTableWidgetItem *titem)
{
	if(titem->column() == 1) {
		ui.buttonSet->setEnabled(true);
		item = titem;
	}
}

void EditSettings::shortcutSequence(const QKeySequence &s)
{
	sequence = s;
}

void EditSettings::shortcutSet()
{
	item->setText(sequence.toString());
	shortcutClear();
}

void EditSettings::shortcutClear()
{
	ui.keyEditor->setKeySequence(QKeySequence(""));
}
