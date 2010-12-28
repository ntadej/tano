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

#ifndef TANO_RECORDERPLUGINS_H_
#define TANO_RECORDERPLUGINS_H_

#include <QtCore/QtPlugin>
#include <QtCore/QString>

class RecorderPlugin
{
public:
	virtual ~RecorderPlugin() {}

	virtual void record(const QString &channelName,
						const QString &channelUrl,
						const QString &file) = 0;
	virtual void stop() = 0;
	virtual bool isRecording() const = 0;
	virtual bool isValid() const = 0;
};

class RecorderPluginCreator
{
public:
	virtual ~RecorderPluginCreator() {}
	virtual RecorderPlugin* createInstance() = 0;
};

Q_DECLARE_INTERFACE(RecorderPluginCreator, "RecorderPlugin/0.8")

#endif // TANO_RECORDERPLUGINS_H_

