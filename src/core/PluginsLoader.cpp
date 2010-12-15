/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#include <QtCore/QMetaClassInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QPluginLoader>
#include <QtGui/QApplication>

#include "Config.h"
#include "core/PluginsLoader.h"

PluginsLoader::PluginsLoader()
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

PluginsLoader::~PluginsLoader()
{
	for(int i = 0; i < _recorderPlugins.size(); i++)
		delete _recorderPlugins[i];
}

void PluginsLoader::processDir(QDir &dir)
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
		if (plugin)
			processPlugin(plugin, fileName);
	}
}

void PluginsLoader::processPlugin(QObject *plugin,
								  const QString &pluginFile)
{
	RecorderPluginCreator *recorder = qobject_cast<RecorderPluginCreator *>(plugin);
	if(recorder) {
		_recorderPlugins << plugin;
		_recorderFiles << pluginFile;
		_recorderNames << plugin->metaObject()->classInfo(plugin->metaObject()->indexOfClassInfo( "PLUGINNAME" )).value();
	}
}

RecorderPlugin *PluginsLoader::recorder(QObject *plugin)
{
	RecorderPluginCreator *recorder = qobject_cast<RecorderPluginCreator *>(plugin);
	if(recorder)
		return recorder->createInstance();
	else
		return 0;
}
