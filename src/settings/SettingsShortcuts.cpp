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

#include <QtCore/QStandardPaths>

#include "application/Common.h"
#include "settings/SettingsShortcuts.h"

SettingsShortcuts::SettingsShortcuts(QObject *parent)
    : QSettings(SettingsShortcuts::fileName(),
                QSettings::IniFormat,
                parent) { }

SettingsShortcuts::~SettingsShortcuts() { }

QString SettingsShortcuts::fileName()
{
#if QT_VERSION < 0x050400
    return QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/Shortcuts.ini";
#else
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/Shortcuts.ini";
#endif
}

QStringList SettingsShortcuts::readKeys() const
{
    QStringList list;
    for (int i = 0; i < DEFAULT_SHORTCUTS_ACTIONS.size(); i++)
        list << value(DEFAULT_SHORTCUTS_ACTIONS[i], DEFAULT_SHORTCUTS_KEYS[i]).toString();

    return list;
}

void SettingsShortcuts::writeShortcut(const QString &action,
                                      const QKeySequence &sequence)
{
    setValue(action, sequence.toString());

    sync();
}
