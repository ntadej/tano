/****************************************************************************
* EpgManager.cpp: EPG manager
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

#include <QtCore/QDate>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "Common.h"
#include "EpgManager.h"

EpgManager::EpgManager(QObject *parent)
	: QObject(parent), _reload(false)
{
	_loader = new EpgLoader(this);
	connect(_loader, SIGNAL(epgDone(QStringList,int)), this, SIGNAL(epg(QStringList,int)));
}

EpgManager::~EpgManager()
{
	delete _loader;
}

void EpgManager::setEpg(const QStringList &epg, const QString &epgPlugin)
{
	_epgList = epg;

	_epgPlugin = epgPlugin;
	_loader->loadPlugin(epgPlugin);
	_path = Common::settingsPath()+_epgPlugin+"/";

	QFile file(_path+"time");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
		_reload = true;

	QTextStream in(&file);
	in.setCodec(QTextCodec::codecForName("UTF-8"));
	if(QDate().fromString(in.readLine(), "dd.mm.yyyy") != QDate::currentDate())
		_reload = true;

	init();
}

void EpgManager::clear()
{
	_day[0].clear();
	_day[1].clear();
	_day[2].clear();
	_day[3].clear();
}

void EpgManager::init()
{
	if(_reload)
		clear();
}

void EpgManager::request(const QString &epg)
{
	_loader->getEpg(epg);
}
