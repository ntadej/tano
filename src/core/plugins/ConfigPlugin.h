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

#ifndef TANO_COREPLUGIN_H_
#define TANO_COREPLUGIN_H_

#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>
#include <QtCore/QtPlugin>

#include "CoreSharedExport.h"

class NetworkPlugin;

class TANO_CORE_EXPORT ConfigPlugin
{
public:
    virtual ~ConfigPlugin() { }

    virtual QString applicationDataDir() const = 0;

    virtual QString name() const = 0;
    virtual QString version() const = 0;
    virtual QString email() const = 0;
    virtual QString projectUrl() const = 0;
    virtual QString updateUrl() const = 0;
    virtual QString downloadUrl() const = 0;

    virtual bool requiresAuthentication() const = 0;
    virtual bool editorEnabled() const = 0;

    virtual QVariantMap defaultSettings() const = 0;
    virtual bool disableSettings(const QString &category) const = 0;
    virtual bool disableSettingsGui(const QString &category) const = 0;

    virtual NetworkPlugin *networkPlugin() = 0;
};

#define ConfigPlugin_IID "si.tano.core.ConfigPlugin"
Q_DECLARE_INTERFACE(ConfigPlugin, ConfigPlugin_IID)

#endif // TANO_COREPLUGIN_H_
