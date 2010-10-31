/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2008-2010 Tadej Novak <ntadej@users.sourceforge.net>
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

#include <QtCore/QTime>

#include "epg/EpgSloveniaLoader.h"

EpgSloveniaLoader::EpgSloveniaLoader(QObject *parent)
	: QHttp(parent),
	_codec(QTextCodec::codecForName("UTF-8")),
	_currentArgument(""),
	_currentRequest(""),
	_init(false)
{
	_slovenia = new EpgSlovenia();
	setHost(_slovenia->host());
}

EpgSloveniaLoader::~EpgSloveniaLoader() { }

void EpgSloveniaLoader::getSchedule(const QString &arg,
									const int &day)
{
	_currentArgument = arg;

	if(!_init) {
		init();
		return;
	}

	_currentRequest = _slovenia->load(arg, day);

	int r =	request(_slovenia->httpHeader(_currentRequest));
	_mapArg.insert(r, arg);
	_mapStep.insert(r, day);
	connect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(processSchedule(int, bool)));
}

void EpgSloveniaLoader::getShowInfo(const QString &arg)
{
	_currentArgument = arg;
	_currentRequest = _slovenia->load(arg);

	request(_slovenia->httpHeader(_currentRequest));
	connect(this, SIGNAL(done(bool)), this, SLOT(processShowInfo(bool)));
}

void EpgSloveniaLoader::stop()
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(initDone(bool)));
	disconnect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(processSchedule(int, bool)));
	disconnect(this, SIGNAL(done(bool)), this, SLOT(processShowInfo(bool)));
	abort();
}

void EpgSloveniaLoader::init()
{
	request(_slovenia->httpHeader("init"));
	connect(this, SIGNAL(done(bool)), this, SLOT(initDone(bool)));
}

void EpgSloveniaLoader::initDone(const bool &error)
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(initDone(bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();

	_init = _slovenia->init(_codec->toUnicode(httpResponse));

	if(_currentArgument != "")
		getSchedule(_currentArgument);
}

void EpgSloveniaLoader::processSchedule(const int &req,
										const bool &error)
{
	disconnect(this, SIGNAL(requestFinished(int, bool)), this, SLOT(processSchedule(int, bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();
	EpgDayList list = _slovenia->processSchedule(_mapArg[req], _mapStep[req], _codec->toUnicode(httpResponse));

	if(!list.isValid()) {
		return;
	}

	emit schedule(list);

	if(_mapStep[req] == 0)
		getSchedule(_currentArgument, 1);
	else if(_mapStep[req] == 1)
		getSchedule(_currentArgument, 2);
	else if(_mapStep[req] == 2)
		getSchedule(_currentArgument, 3);
}

void EpgSloveniaLoader::processShowInfo(const bool &error)
{
	disconnect(this, SIGNAL(done(bool)), this, SLOT(processShowInfo(bool)));

	if(error)
		return;

	QByteArray httpResponse = readAll();
	EpgShowInfo info = _slovenia->processShow(_codec->toUnicode(httpResponse));

	if(!info.isValid())
		return;

	emit showInfo(info);
}
