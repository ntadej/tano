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

#ifndef TANO_SHORTCUTS_H_
#define TANO_SHORTCUTS_H_

#include <QtCore/QList>
#include <QtCore/QSettings>
#include <QtCore/QStringList>

class QAction;

class Shortcuts : public QSettings
{
public:
    explicit Shortcuts(const QList<QAction *> &list,
                       QObject *parent = 0);
    ~Shortcuts();

    inline QStringList actionsNames() const { return _actionsName; }
    void apply();
    inline QStringList defaultKeys() const { return _defaultList; }
    QStringList readKeys() const;
    inline void restoreDefaults() { write(_defaultList); }
    void write(const QStringList &keys);

private:
    QList<QAction *> _actions;
    QStringList _actionsName;
    QStringList _defaultList;
};

#endif // TANO_SHORTCUTS_H_
