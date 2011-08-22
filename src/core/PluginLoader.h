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

#ifndef TANO_PLUGINLOADER_H_
#define TANO_PLUGINLOADER_H_

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

class EpgPlugin;

class PluginLoader
{
public:
    PluginLoader();
    ~PluginLoader();

    QList<QObject *> epgPlugin() const { return _epgPlugins; }
    QStringList epgFile() const { return _epgFiles; }
    QStringList epgName() const { return _epgNames; }
    EpgPlugin *epg(QObject *plugin);

private:
    void processDir(QDir &dir);
    void processPlugin(QObject *plugin,
                       const QString &pluginFile);

    QList<QObject *> _epgPlugins;
    QStringList _epgFiles;
    QStringList _epgNames;
};

#endif // TANO_PLUGINLOADER_H_
