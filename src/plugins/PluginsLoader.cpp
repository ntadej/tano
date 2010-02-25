/****************************************************************************
* PluginsLoader.cpp: A class for loading plugins
*****************************************************************************
* Copyright (C) 2008-2010 Tadej Novak
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* This file may be used under the terms of the
* GNU General Public License version 3.0 as published by the
* Free Software Foundation and appearing in the file LICENSE.GPL
* included in the packaging of this file.
*****************************************************************************/

#include <QtCore/QMetaClassInfo>
#include <QtCore/QMetaObject>
#include <QtCore/QPluginLoader>
#include <QtGui/QApplication>

#include "PluginsLoader.h"

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
	for(int i=0; i<_recorderPlugins.size(); i++)
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

void PluginsLoader::processPlugin(QObject *plugin, const QString &pluginFile)
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
		return recorder->createRecorderPluginInstance();
}
