/****************************************************************************
* EpgSloveniaPlugin.h: EPG Slovenia creator class
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

#ifndef TANO_EPGSLOVENIAPLUGINCREATOR_H_
#define TANO_EPGSLOVENIAPLUGINCREATOR_H_

#include <QtCore/QObject>
#include <QtCore/QtPlugin>

#include "plugins/EpgPlugins.h"

class EpgSloveniaPluginCreator : public QObject, public EpgPluginCreator
{
Q_OBJECT;
Q_INTERFACES( EpgPluginCreator );
Q_CLASSINFO("PLUGINTYPE", "EPG");
Q_CLASSINFO("PLUGINNAME", "EpgSlovenia");

public:
	EpgPlugin *createEpgPluginInstance();
};

#endif // TANO_EPGSLOVENIAPLUGINCREATOR_H_
