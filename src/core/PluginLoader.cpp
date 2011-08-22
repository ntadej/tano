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

#include <QtCore/QMetaClassInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QPluginLoader>
#include <QtGui/QApplication>

#include "core/PluginLoader.h"
#include "plugins/EpgPlugin.h"

PluginLoader::PluginLoader()
{
    QDir pluginsDir;

    if(qApp->applicationDirPath().contains("src")) {
        pluginsDir = QDir(qApp->applicationDirPath());
        pluginsDir.cdUp();
        pluginsDir.cd("plugins");
        processDir(pluginsDir);
    } else {
#ifdef DEFAULT_PLUGIN_DIR
        pluginsDir = QDir(QString(DEFAULT_PLUGIN_DIR));
        processDir(pluginsDir);
#else
        pluginsDir = QDir(qApp->applicationDirPath());
        pluginsDir.cd("plugins");
        processDir(pluginsDir);
#endif
    }
}

PluginLoader::~PluginLoader()
{
    for(int i = 0; i < _epgPlugins.size(); i++)
        delete _epgPlugins[i];
}

void PluginLoader::processDir(QDir &dir)
{
    foreach (QString folderName, dir.entryList(QDir::Dirs)) {
        if(folderName == "." || folderName == "..")
            continue;

        dir.cd(folderName);
        processDir(dir);
        dir.cdUp();
    }

    foreach (QString fileName, dir.entryList(QDir::Files)) {
        QPluginLoader loader;
        loader.setFileName(dir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if(plugin)
            processPlugin(plugin, fileName);
    }
}

void PluginLoader::processPlugin(QObject *plugin,
                                 const QString &pluginFile)
{
    EpgPluginCreator *epg = qobject_cast<EpgPluginCreator *>(plugin);
    if(epg) {
        _epgPlugins << plugin;
        _epgFiles << pluginFile;
        _epgNames << plugin->metaObject()->classInfo(plugin->metaObject()->indexOfClassInfo( "PLUGINNAME" )).value();
    }
}

EpgPlugin *PluginLoader::epg(QObject *plugin)
{
    EpgPluginCreator *epg = qobject_cast<EpgPluginCreator *>(plugin);
    if(epg)
        return epg->createInstance();
    else
        return 0;
}
