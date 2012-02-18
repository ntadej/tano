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

#include "core/Shortcuts.h"

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
    switch (e->type())
    {
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
    for(int i = 0; i < ui->shortcutsWidget->rowCount(); i++) {
        if(ui->shortcutsWidget->item(i, 1)) {
            ui->shortcutsWidget->item(i, 1)->setText(keys[i]);
        } else {
            ui->shortcutsWidget->setItem(i, 1, new QTableWidgetItem(keys[i]));
        }
    }
}

void SettingsShortcuts::shortcutRestore()
{
    _shortcuts->restoreDefaults();

    shortcutRead();
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

    for(int i = 0; i < ui->shortcutsWidget->rowCount(); i++) {
        keys << ui->shortcutsWidget->item(i,1)->text();
    }

    _shortcuts->write(keys);
    _shortcuts->apply();

}
