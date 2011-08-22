/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
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

#ifndef TANO_EPGPLUGIN_H_
#define TANO_EPGPLUGIN_H_

#include <QtCore/QtPlugin>
#include <QtCore/QString>

class EpgPlugin
{
public:
    virtual ~EpgPlugin() {}

    virtual QString copyright() const = 0;
    virtual QString link() const = 0;
    virtual QString logo() const = 0;

    virtual void getChannel(const QString &id) = 0;
    virtual void getProgramme(const QString &id) = 0;
};

class EpgPluginCreator
{
public:
    virtual ~EpgPluginCreator() {}
    virtual EpgPlugin* createInstance() = 0;
};

Q_DECLARE_INTERFACE(EpgPluginCreator, "EpgPlugin/0.9")

#endif // TANO_EPGPLUGIN_H_

