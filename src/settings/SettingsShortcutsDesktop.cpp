/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2016 Tadej Novak <tadej@tano.si>
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

#include <QtGui/QStandardItemModel>
#include <QtWidgets/QAction>

#include <QDebug>

#include "SettingsShortcutsDesktop.h"

SettingsShortcutsDesktop::SettingsShortcutsDesktop(QObject *parent)
    : SettingsShortcuts(parent),
      _model(new QStandardItemModel)
{
    QStringList currentKeys = readKeys();

    for (int i = 0; i < SettingsShortcuts::DEFAULT_SHORTCUTS_ACTIONS.size(); i++) {
        QList<QStandardItem *> row;
        row << new QStandardItem(SettingsShortcuts::DEFAULT_SHORTCUTS_STRINGS[i])
            << new QStandardItem(QKeySequence(currentKeys[i], QKeySequence::PortableText).toString(QKeySequence::NativeText))
            << new QStandardItem(SettingsShortcuts::DEFAULT_SHORTCUTS_ACTIONS[i]);
        _model->appendRow(row);
    }

    connect(_model, &QStandardItemModel::itemChanged, this, &SettingsShortcutsDesktop::itemChanged);
}

SettingsShortcutsDesktop::~SettingsShortcutsDesktop() { }

void SettingsShortcutsDesktop::setActions(const QList<QAction *> &list)
{
    _actions = list;

    QStringList currentKeys = readKeys();

    for (int i = 0; i < _actions.size(); i++) {
        _actions[i]->setShortcut(QKeySequence(currentKeys[i]));
        _actions[i]->setShortcutContext(Qt::ApplicationShortcut);
    }
}

void SettingsShortcutsDesktop::itemChanged(QStandardItem *item)
{
    QKeySequence sequence(item->data(Qt::EditRole).toString(), QKeySequence::NativeText);
    QString action = _model->item(item->row(), 2)->data(Qt::DisplayRole).toString();

    writeShortcut(action, sequence);

    int index = SettingsShortcuts::DEFAULT_SHORTCUTS_ACTIONS.indexOf(action);
    _actions[index]->setShortcut(sequence);
    _actions[index]->setShortcutContext(Qt::ApplicationShortcut);
}
