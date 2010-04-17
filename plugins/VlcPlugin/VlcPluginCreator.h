/****************************************************************************
* VlcPluginCreator.h: VlcPlugin Creator Class
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

#ifndef TANO_VLCPLUGINCREATOR_H_
#define TANO_VLCPLUGINCREATOR_H_

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include "plugins/RecorderPlugins.h"

class VlcPluginCreator : public QObject, public RecorderPluginCreator
{
Q_OBJECT;
Q_INTERFACES( RecorderPluginCreator );
Q_CLASSINFO("PLUGINTYPE", "Recorder");
Q_CLASSINFO("PLUGINNAME", "Vlc");

public:
	RecorderPlugin *createRecorderPluginInstance();
};

#endif // TANO_VLCPLUGINCREATOR_H_
