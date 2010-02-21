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

private:
	void processDir(QDir &dir);
	void processPlugin(QObject *plugin, const QString &pluginFile);

	QList<QObject*> _recorderPlugins;
	QStringList _recorderFiles;
	QStringList _recorderNames;
};

#endif // TANO_PLUGINSLOADER_H_
