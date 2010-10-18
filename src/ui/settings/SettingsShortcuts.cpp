/****************************************************************************
* SettingsShortcuts.cpp: Shortcuts editor
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

#include "SettingsShortcuts.h"
#include "ui_SettingsShortcuts.h"

SettingsShortcuts::SettingsShortcuts(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::SettingsShortcuts)
{
	ui->setupUi(this);
	createActions();

	ui->shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
}

SettingsShortcuts::~SettingsShortcuts()
{
	delete ui;
}

void SettingsShortcuts::changeEvent(QEvent *e)
{
	QWidget::changeEvent(e);
	switch (e->type()) {
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;
		default:
			break;
	}
}

void SettingsShortcuts::createActions()
{
	connect(ui->shortcutsWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(shortcutEdit(QTableWidgetItem*)));
	connect(ui->keyEditor, SIGNAL(keySequenceChanged(const QKeySequence)), this, SLOT(shortcutSequence(const QKeySequence)));
	connect(ui->buttonDefaults, SIGNAL(clicked()), this, SLOT(shortcutRestore()));
	connect(ui->buttonSet, SIGNAL(clicked()), this, SLOT(shortcutSet()));
	connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(shortcutClear()));
}

void SettingsShortcuts::setShortcuts(Shortcuts *s)
{
	_shortcuts = s;

	shortcutRead();
}

void SettingsShortcuts::shortcutClear()
{
	ui->keyEditor->setKeySequence(QKeySequence(""));
}

void SettingsShortcuts::shortcutEdit(QTableWidgetItem *titem)
{
	if(titem->column() == 1) {
		ui->buttonSet->setEnabled(true);
		ui->keyEditor->setKeySequence(QKeySequence(titem->text()));
		_item = titem;
	}
}

void SettingsShortcuts::shortcutRead()
{
	QStringList keys = _shortcuts->readKeys();
	for(int i=0; i < ui->shortcutsWidget->rowCount(); i++)
		ui->shortcutsWidget->item(i,1)->setText(keys[i]);
}

void SettingsShortcuts::shortcutRestore()
{
	_shortcuts->restoreDefaults();

	QStringList keys = _shortcuts->readKeys();
	for(int i=0; i < ui->shortcutsWidget->rowCount(); i++)
		ui->shortcutsWidget->item(i,1)->setText(keys[i]);
}

void SettingsShortcuts::shortcutSequence(const QKeySequence &s)
{
	_sequence = s;
}

void SettingsShortcuts::shortcutSet()
{
	_item->setText(_sequence.toString());
	shortcutClear();
}

void SettingsShortcuts::shortcutWrite()
{
	QStringList keys;
	for(int i=0; i < ui->shortcutsWidget->rowCount(); i++)
		keys << ui->shortcutsWidget->item(i,1)->text();

	_shortcuts->write(keys);
	_shortcuts->apply();
}
