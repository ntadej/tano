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

#ifndef TANO_DESKTOPSHORTCUTS_H_
#define TANO_DESKTOPSHORTCUTS_H_

#include "core/settings/SettingsShortcuts.h"

class QAction;

class DesktopShortcuts : public SettingsShortcuts
{
public:
    explicit DesktopShortcuts(const QList<QAction *> &list,
                              QObject *parent = 0);
    ~DesktopShortcuts();

    void apply();

private:
    QList<QAction *> _actions;
};

#endif // TANO_DESKTOPSHORTCUTS_H_
