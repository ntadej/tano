/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_TANOCONFIG_H_
#define TANO_TANOCONFIG_H_

#include "plugins/ConfigPlugin.h"

class TanoConfig : public QObject, public ConfigPlugin
{
Q_OBJECT
Q_PLUGIN_METADATA(IID "si.tano.core.ConfigPlugin")
Q_INTERFACES(ConfigPlugin)

public:
    QString applicationDataDir() const;

    QString name() const;
    QString version() const;
    QString email() const;
    QString projectUrl() const;

    bool editorEnabled() const;

    QVariantMap defaultSettings() const { return QVariantMap(); }
    bool disableSettings(const QString &category) const { Q_UNUSED(category) return false; }
    bool disableSettingsGui(const QString &category) const { Q_UNUSED(category) return false; }
};

#endif // TANO_TANOCONFIG_H_
