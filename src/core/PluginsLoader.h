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

#ifndef TANO_PLUGINSLOADER_H_
#define TANO_PLUGINSLOADER_H_

#include <QtCore/QDir>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

class RecorderPlugin;

class PluginsLoader
{
public:
    PluginsLoader();
	~PluginsLoader();

	QList<QObject *> recorderPlugin() const { return _recorderPlugins; }
	QStringList recorderFile() const { return _recorderFiles; }
	QStringList recorderName() const { return _recorderNames; }
	RecorderPlugin *recorder(QObject *plugin);

private:
	void processDir(QDir &dir);
	void processPlugin(QObject *plugin,
					   const QString &pluginFile);

	QList<QObject *> _recorderPlugins;
	QStringList _recorderFiles;
	QStringList _recorderNames;
};

#endif // TANO_PLUGINSLOADER_H_
