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

#include "EpgLoader.h"
#include "../plugins/PluginsLoader.h"

EpgLoader::EpgLoader(QObject *parent)
	: QHttp(parent), _init(false), _show(false), _step(0), _plugin(0),
	_currentArgument(""), _currentRequest(""), _currentList(QStringList()), _codec(QTextCodec::codecForName("UTF-8"))
{
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(now()));
}

EpgLoader::~EpgLoader()
{

}

void EpgLoader::getEpg(const QString &arg, const bool &type)
{
	_show = type;
	_currentArgument = arg;

	if(!_init && !_show) {
		init();
		return;
	}

	_currentRequest = _plugin->load(arg, _step);
	epg();
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

void EpgLoader::now()
{
	QStringList now;
	for(int i = 1; i < _currentList.size(); i+=3) {
		if(QTime::currentTime() > QTime::fromString(_currentList[i], "hh:mm") && QTime::currentTime() < QTime::fromString(_currentList[i+3], "hh:mm")) {
			now << "<a href=\"" + _currentList[i+1] + "\">" + _currentList[i] + " - " + _currentList[i+2] + "</a>"
				<< "<a href=\"" + _currentList[i+4] + "\">" + _currentList[i+3] + " - " + _currentList[i+5] + "</a>";
			emit epgDone(now, 0);
			break;
		}
	}

	_timer->start(60000);
}

void EpgLoader::stop()
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(initDone(bool)));
	disconnect(this, SIGNAL(done(bool)), this, SLOT(schedule(bool)));
	disconnect(this, SIGNAL(done(bool)), this, SLOT(show(bool)));
	abort();
	_timer->stop();
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
		getEpg(_currentArgument, _show);
}

void EpgLoader::epg()
{
	request(_plugin->httpHeader(_currentRequest));

	if(_show)
		connect(this, SIGNAL(done(bool)), this, SLOT(show(bool)));
	else
		connect(this, SIGNAL(done(bool)), this, SLOT(schedule(bool)));
}

void EpgLoader::schedule(const bool &error)
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(schedule(bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();
	QStringList list = _plugin->processSchedule(_codec->toUnicode(httpResponse));

	if(list[0] == "error")
		return;

	emit epgDone(list, _step+1);

	if(_step == 0) {
		_currentList = list;
		now();
	}

	if(_step < 3) {
		_step++;
		getEpg(_currentArgument);
	} else {
		_step = 0;
	}
}

void EpgLoader::show(const bool &error)
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(show(bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();
	QStringList list = _plugin->processShow(_codec->toUnicode(httpResponse));

	if(list[0] == "error")
		return;

	emit epgDone(list, 0);
}
