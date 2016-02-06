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

#ifndef TANO_SETTINGSSHORTCUTSDESKTOP_H_
#define TANO_SETTINGSSHORTCUTSDESKTOP_H_

#include "settings/SettingsShortcuts.h"

class QAction;
class QStandardItem;
class QStandardItemModel;

class SettingsShortcutsDesktop : public SettingsShortcuts
{
public:
    explicit SettingsShortcutsDesktop(QObject *parent = 0);
    ~SettingsShortcutsDesktop();

    QStandardItemModel *model() { return _model; }

    void setActions(const QList<QAction *> &list);

private slots:
    void itemChanged(QStandardItem *item);

private:
    QList<QAction *> _actions;
    QStandardItemModel *_model;
};

#endif // TANO_SETTINGSSHORTCUTSDESKTOP_H_
