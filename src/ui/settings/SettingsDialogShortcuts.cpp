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

#include "Config.h"
#include "core/Shortcuts.h"

#include "SettingsDialogShortcuts.h"
#include "ui_SettingsDialogShortcuts.h"

SettingsDialogShortcuts::SettingsDialogShortcuts(Shortcuts *shortcuts,
                                     QWidget *parent)
    : QDialog(parent),
      ui(new Ui::SettingsDialogShortcuts)
{
    ui->setupUi(this);
    createActions();

    ui->shortcutsWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);

#if !TELETEXT
    ui->shortcutsWidget->removeRow(8);
#endif

    _shortcuts = shortcuts;

    shortcutRead();
}

SettingsDialogShortcuts::~SettingsDialogShortcuts()
{
    delete ui;
}

void SettingsDialogShortcuts::changeEvent(QEvent *e)
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

void SettingsDialogShortcuts::action(QAbstractButton *button)
{
    switch(ui->buttonBox->standardButton(button))
    {
    case QDialogButtonBox::Save:
        shortcutWrite();
        close();
        break;
    case QDialogButtonBox::Cancel:
        close();
        break;
    default:
        break;
    }
}

void SettingsDialogShortcuts::createActions()
{
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(action(QAbstractButton*)));
    connect(ui->shortcutsWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(shortcutEdit(QTableWidgetItem*)));
    connect(ui->keyEditor, SIGNAL(keySequenceChanged(const QKeySequence)), this, SLOT(shortcutSequence(const QKeySequence)));
    connect(ui->buttonDefaults, SIGNAL(clicked()), this, SLOT(shortcutRestore()));
    connect(ui->buttonSet, SIGNAL(clicked()), this, SLOT(shortcutSet()));
    connect(ui->buttonClear, SIGNAL(clicked()), this, SLOT(shortcutClear()));
}

void SettingsDialogShortcuts::shortcutClear()
{
    ui->keyEditor->setKeySequence(QKeySequence(""));
}

void SettingsDialogShortcuts::shortcutEdit(QTableWidgetItem *titem)
{
    if(titem->column() == 1) {
        ui->buttonSet->setEnabled(true);
        ui->keyEditor->setKeySequence(QKeySequence(titem->text()));
        _item = titem;
    }
}

void SettingsDialogShortcuts::shortcutRead()
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

void SettingsDialogShortcuts::shortcutRestore()
{
    _shortcuts->restoreDefaults();

    shortcutRead();
}

void SettingsDialogShortcuts::shortcutSequence(const QKeySequence &s)
{
    _sequence = s;
}

void SettingsDialogShortcuts::shortcutSet()
{
    _item->setText(_sequence.toString());
    shortcutClear();
}

void SettingsDialogShortcuts::shortcutWrite()
{
    QStringList keys;

    for(int i = 0; i < ui->shortcutsWidget->rowCount(); i++) {
        keys << ui->shortcutsWidget->item(i,1)->text();
    }

    _shortcuts->write(keys);
    _shortcuts->apply();

}
