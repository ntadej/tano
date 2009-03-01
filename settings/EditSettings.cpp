#include <QtGui>
#include <QMessageBox>
#include <QTextEdit>
#include <QDir>
#include <QFileDialog>

#include "EditSettings.h"
#include "../Common.h"

EditSettings::EditSettings(QWidget *parent, Shortcuts *shortcuts)
	: QDialog(parent)
{
	ui.setupUi(this);
	createActions();

	ui.buttonSet->setEnabled(false);
	ui.buttonBrowse->setEnabled(false);
	ui.buttonReset->setEnabled(false);

	ui.shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.shortcutsWidget->verticalHeader()->hide();

	keys = shortcuts;

	settings = new SettingsMain(Common::settingsFile(), Common::settingsDefault());
	sshortcuts = new SettingsShortcuts(Common::settingsFile("shortcuts"), keys->defaultKeys());

	read();
	shortcutRead();
}

void EditSettings::createActions()
{
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));

	connect(ui.radioDefault, SIGNAL(clicked()), this, SLOT(toggleCustom()));
	connect(ui.radioCustom, SIGNAL(clicked()), this, SLOT(toggleCustom()));

	connect(ui.radioSiol, SIGNAL(clicked()), this, SLOT(togglePlaylist()));
	connect(ui.radioT2, SIGNAL(clicked()), this, SLOT(togglePlaylist()));
	connect(ui.radioBrowse, SIGNAL(clicked()), this, SLOT(togglePlaylist()));

	connect(ui.buttonBrowse, SIGNAL(clicked()), this, SLOT(playlistBrowse()));
	connect(ui.buttonReset, SIGNAL(clicked()), this, SLOT(playlistReset()));

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
	if (ui.radioDefault->isChecked())
	{
		settingsList[0] = QString("Default");
		if (settingsList.size() < 2) settingsList << "-";
		else settingsList[1] = "-";
	} else
	{
		settingsList[0] = QString("Custom");
		if (settingsList.size() < 2) settingsList << QString(ui.comboBox->currentIndex());
		else settingsList[1].setNum(ui.comboBox->currentIndex());
	}

	if(ui.checkSession->isChecked()) {
		if (settingsList.size() < 3) settingsList << "1";
		else settingsList[2] = "1";
	} else {
		if (settingsList.size() < 3) settingsList << "0";
		else settingsList[2] = "0";
	}

	if(ui.radioSiol->isChecked()) {
		if (settingsList.size() < 4) settingsList << "0";
		else settingsList[3] = "0";
	} else if(ui.radioT2->isChecked()) {
		if (settingsList.size() < 4) settingsList << "1";
		else settingsList[3] = "1";
	} else {
		if (settingsList.size() < 4) settingsList << ui.pEdit->text();
		else settingsList[3] = ui.pEdit->text();
	}

	success = settings->write(settingsList);

	if (success == true) {
		for(int i=0;i < ui.shortcutsWidget->rowCount();i++) {
			item = ui.shortcutsWidget->item(i,1);
			keysList[i] = item->text();
		}
		success = sshortcuts->write(keysList);
	}
	if(success == true) {
		keys->apply();
		hide();
	}
}

void EditSettings::cancel()
{
	hide();
}

void EditSettings::read()
{
	settingsList = settings->read();

	if(settingsList[0] != "Default")
	{
		ui.radioCustom->setChecked(true);
		ui.comboBox->setEnabled(true);
		bool okint;
		ui.comboBox->setCurrentIndex(settingsList[1].toInt(&okint,10));
	}

	if(settingsList.size() > 2) {
		if (settingsList[2] == "1") ui.checkSession->setChecked(true);
		else ui.checkSession->setChecked(false);

		if (settingsList[3] == "0")
			ui.radioSiol->setChecked(true);
		else if(settingsList[3] == "1")
			ui.radioT2->setChecked(true);
		else {
			ui.radioBrowse->setChecked(true);
			ui.buttonBrowse->setEnabled(true);
			ui.buttonReset->setEnabled(true);
			ui.pEdit->setText(settingsList[3]);
		}
	}
}

void EditSettings::toggleCustom()
{
	if (ui.radioCustom->isChecked())
	{
		ui.comboBox->setEnabled(true);
	} else
	{
		ui.comboBox->setEnabled(false);
	}
}

void EditSettings::togglePlaylist()
{
	if (ui.radioBrowse->isChecked())
	{
		ui.buttonBrowse->setEnabled(true);
		ui.buttonReset->setEnabled(true);
	} else
	{
		ui.buttonBrowse->setEnabled(false);
		ui.buttonReset->setEnabled(false);
	}
}

void EditSettings::playlistBrowse()
{
	QString dfile = QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
            QDir::homePath(),
            tr("Tano TV Channel list Files(*.tano *.xml)"));
	ui.pEdit->setText(dfile);
}

void EditSettings::playlistReset()
{
	ui.pEdit->setText("");
}

void EditSettings::shortcutRead()
{
	keysList = sshortcuts->read();

	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++) {
		item = ui.shortcutsWidget->item(i,1);
		item->setText(keysList[i]);
	}
}

void EditSettings::shortcutRestore()
{
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++) {
		item = ui.shortcutsWidget->item(i,1);
		item->setText(keys->defaultKeys().at(i));
	}
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
