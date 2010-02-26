/****************************************************************************
* PluginsLoader.h: A class for loading plugins
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

#ifndef TANO_PLUGINSLOADER_H_
#define TANO_PLUGINSLOADER_H_

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "EpgPlugins.h"
#include "RecorderPlugins.h"

class PluginsLoader
{
public:
    PluginsLoader();
	~PluginsLoader();

	QList<QObject*> recorderPlugin() const {return _recorderPlugins;};
	QStringList recorderFile() const {return _recorderFiles;};
	QStringList recorderName() const {return _recorderNames;};
	RecorderPlugin *recorder(QObject *plugin);

	QList<QObject*> epgPlugin() const {return _epgPlugins;};
	QStringList epgFile() const {return _epgFiles;};
	QStringList epgName() const {return _epgNames;};
	EpgPlugin *epg(QObject *plugin);

private:
	void processDir(QDir &dir);
	void processPlugin(QObject *plugin, const QString &pluginFile);

	QList<QObject*> _recorderPlugins;
	QStringList _recorderFiles;
	QStringList _recorderNames;

	QList<QObject*> _epgPlugins;
	QStringList _epgFiles;
	QStringList _epgNames;
};

#endif // TANO_PLUGINSLOADER_H_
