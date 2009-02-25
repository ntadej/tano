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

	ui.buttonSet->setDisabled(true);

	ui.shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.shortcutsWidget->verticalHeader()->hide();

	keys = shortcuts;

	settings = new SettingsMain(Common::settingsFile(), Common::settingsDefault());
	sshortcuts = new SettingsShortcuts(Common::settingsFile("shortcuts"), keys->defaultKeys());

	read();
	readS();
}

void EditSettings::createActions()
{
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
	connect(ui.shortcutsWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(editShortcut(QTableWidgetItem*)));

	connect(ui.defaultRadio, SIGNAL(clicked()), this, SLOT(custom()));
	connect(ui.customRadio, SIGNAL(clicked()), this, SLOT(custom()));
	connect(ui.browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(reset()));
	connect(ui.buttonDefaults, SIGNAL(clicked()), this, SLOT(restoreS()));
	connect(ui.buttonSet, SIGNAL(clicked()), this, SLOT(set()));
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

void EditSettings::custom()
{
	if (ui.customRadio->isChecked() == true)
	{
		ui.comboBox->setEnabled(true);
	} else
	{
		ui.comboBox->setEnabled(false);
	}
}

void EditSettings::read()
{
	settingsList = settings->read();

	if (settingsList[0] == "Default")
	{
		ui.defaultRadio->setChecked(true);
	} else
	{
		ui.customRadio->setChecked(true);
		ui.comboBox->setEnabled(true);
		bool okint;
		ui.comboBox->setCurrentIndex(settingsList[1].toInt(&okint,10));
	}

	if (settingsList.size() > 2) {
		if (settingsList[2] != "Custom" && settingsList[2] != "Default") ui.pEdit->setText(settingsList[2]);
	}
	else ui.pEdit->setText("");
}

void EditSettings::readS()
{
	keysList = sshortcuts->read();

	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++) {
		item = ui.shortcutsWidget->item(i,1);
		item->setText(keysList[i]);
	}
}

void EditSettings::restoreS()
{
	for(int i=0; i < ui.shortcutsWidget->rowCount(); i++) {
		item = ui.shortcutsWidget->item(i,1);
		item->setText(keys->defaultKeys().at(i));
	}
}

void EditSettings::ok()
{
	if (ui.defaultRadio->isChecked() == true)
	{
		settingsList[0] = QString("Default");
	} else
	{
		settingsList[0] = "Custom";
		settingsList[1].setNum(ui.comboBox->currentIndex());
	}

	if (settingsList.size() < 3) settingsList << ui.pEdit->text();
	else settingsList[2] = ui.pEdit->text();

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

void EditSettings::browse()
{
	QString dfile = QFileDialog::getOpenFileName(this, tr("Open Channel list File"),
            QDir::homePath(),
            tr("Tano TV Channel list Files(*.tano *.xml)"));
	ui.pEdit->setText(dfile);
}

void EditSettings::reset()
{
	ui.pEdit->setText("");
}

void EditSettings::editShortcut(QTableWidgetItem *titem)
{
	if(titem->column() == 1) {
		ui.buttonSet->setEnabled(true);
		item = titem;
	}
}

void EditSettings::set()
{
	item->setText(ui.lineEdit->text());
	ui.lineEdit->setText("");
}
