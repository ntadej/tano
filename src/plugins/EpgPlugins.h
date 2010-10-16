/****************************************************************************
* EpgPlugins.h: Definition of EPG plugins used by Tano application
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

#ifndef TANO_EPGPLUGINS_H_
#define TANO_EPGPLUGINS_H_

#include <QtCore/QtPlugin>
#include <QtCore/QString>
#include <QtNetwork/QHttpRequestHeader>

class EpgPlugin
{
public:
	virtual ~EpgPlugin() {};

	/* CURRENTLY NOT IN USE! */
};

class EpgPluginCreator
{
public:
	virtual ~EpgPluginCreator() {};
	virtual EpgPlugin* createInstance() = 0;
};

Q_DECLARE_INTERFACE(EpgPluginCreator, "EpgPluginCreator/0.8")

#endif // TANO_EPGPLUGINS_H_
