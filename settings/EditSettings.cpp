#include <QtGui>
#include <QMessageBox>
#include <QTextEdit>
#include <QDir>
#include <QFileDialog>
#include "EditSettings.h"

EditSettings::EditSettings(QWidget *parent, QString file, QStringList dl)
	: QDialog(parent)
{
	ui.setupUi(this);
	createActions();

	ui.shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	ui.shortcutsWidget->verticalHeader()->hide();

	settings = new SettingsMain(file, dl);

	read();
}

void EditSettings::createActions()
{
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
	connect(ui.defaultRadio, SIGNAL(clicked()), this, SLOT(custom()));
	connect(ui.customRadio, SIGNAL(clicked()), this, SLOT(custom()));
	connect(ui.browseButton, SIGNAL(clicked()), this, SLOT(browse()));
	connect(ui.resetButton, SIGNAL(clicked()), this, SLOT(reset()));
}

void EditSettings::action(QAbstractButton *button)
{
	switch(ui.buttonBox->standardButton(button)) {
	case 0x00000800:
		qDebug() << "Save";
		ok();
		break;
	case 0x00400000:
		qDebug() << "Cancel";
		cancel();
		break;
	case 0x02000000:
		qDebug() << "Apply";
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

	if (success == true)
	hide();
}

void EditSettings::apply()
{
//TODO: Apply
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
