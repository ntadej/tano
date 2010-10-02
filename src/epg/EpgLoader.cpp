/****************************************************************************
* EpgLoader.cpp: EPG loader and processer
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

#include <QtCore/QTime>

#include "epg/EpgLoader.h"
#include "plugins/PluginsLoader.h"

EpgLoader::EpgLoader(QObject *parent)
	: QHttp(parent),
	_init(false),
	_plugin(0),
	_currentArgument(""),
	_currentRequest(""),
	_codec(QTextCodec::codecForName("UTF-8")) { }

EpgLoader::~EpgLoader() { }

void EpgLoader::getSchedule(const QString &arg,
							const int &day)
{
	_currentArgument = arg;

	if(!_init) {
		init();
		return;
	}

	_currentRequest = _plugin->load(arg, day);

	int r =	request(_plugin->httpHeader(_currentRequest));
	_mapArg.insert(r, arg);
	_mapStep.insert(r, day);
	connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(processSchedule(int, bool)));
}

void EpgLoader::getShow(const QString &arg)
{
	_currentArgument = arg;
	_currentRequest = _plugin->load(arg);

	request(_plugin->httpHeader(_currentRequest));
	connect(this, SIGNAL(done(bool)), this, SLOT(processShow(bool)));
}

void EpgLoader::loadPlugin(const QString &plugin)
{
	if(_plugin != 0)
		delete _plugin;

	PluginsLoader *loader = new PluginsLoader();
	for(int i=0; i < loader->epgPlugin().size(); i++)
		if(loader->epgName()[i] == plugin)
			_plugin = loader->epg(loader->epgPlugin()[i]);
	delete loader;

	setHost(_plugin->host());
}

void EpgLoader::stop()
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(initDone(bool)));
	disconnect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(processSchedule(int, bool)));
	disconnect(this, SIGNAL(done(bool)), this, SLOT(processShow(bool)));
	abort();
}

void EpgLoader::init()
{
	request(_plugin->httpHeader("init"));
	connect(this, SIGNAL(done(bool)), this, SLOT(initDone(bool)));
}

void EpgLoader::initDone(const bool &error)
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(initDone(bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();

	_init = _plugin->init(_codec->toUnicode(httpResponse));

	if(_currentArgument != "")
		getSchedule(_currentArgument);
}

void EpgLoader::processSchedule(const int &req,
								const bool &error)
{
	disconnect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(processSchedule(int, bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();
	QStringList list = _plugin->processSchedule(_codec->toUnicode(httpResponse));

	if(list[0] == "error")
		return;

	emit schedule(_mapArg[req], _mapStep[req], list);

	if(_mapStep[req] == 0)
		getSchedule(_currentArgument, 1);
	else if(_mapStep[req] == 1)
		getSchedule(_currentArgument, 2);
	else if(_mapStep[req] == 2)
		getSchedule(_currentArgument, 3);
}

void EpgLoader::processShow(const bool &error)
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(processShow(bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();
	QStringList list = _plugin->processShow(_codec->toUnicode(httpResponse));

	if(list[0] == "error")
		return;

	emit show(list);
}
