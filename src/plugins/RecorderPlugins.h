/****************************************************************************
* RecorderPlugins.h: Definition of Recorder plugins used by Tano application
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

#ifndef TANO_RECORDERPLUGINS_H_
#define TANO_RECORDERPLUGINS_H_

#include <QtCore/QtPlugin>
#include <QtCore/QString>

class RecorderPlugin
{
public:
	virtual ~RecorderPlugin() {};

	virtual void record(const QString &channelName,
						const QString &channelUrl,
						const QString &recordingDir) = 0;
	virtual void stop() = 0;
	virtual bool isRecording() const = 0;
	virtual bool isValid() const = 0;
	virtual QString output() const = 0;
};

class RecorderPluginCreator
{
public:
	virtual ~RecorderPluginCreator() {};
	virtual RecorderPlugin* createInstance() = 0;
};

Q_DECLARE_INTERFACE(RecorderPluginCreator, "RecorderPluginCreator/0.8")

#endif // TANO_RECORDERPLUGINS_H_

