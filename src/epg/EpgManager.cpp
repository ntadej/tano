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
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "Common.h"
#include "EpgManager.h"

EpgManager::EpgManager(QObject *parent)
	: QObject(parent), _ready(false), _reload(false), _currentEpg(""), _currentLoadEpg(""), _currentRequest("")
{
	_loader = new EpgLoader(this);
	connect(_loader, SIGNAL(schedule(QString, int, QStringList)), this, SLOT(set(QString, int, QStringList)));

	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(now()));
}

EpgManager::~EpgManager()
{
	delete _loader;
	delete _timer;
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
	clear();

	load();
}

void EpgManager::request(const QString &epg)
{
	if(epg == "") {
		_timer->stop();
	} else if(_day[3][epg].isEmpty()) {
		_currentRequest = epg;
		if(_ready)
			load();
	} else {
		post(epg);
	}
}

void EpgManager::load()
{
	if(_currentRequest != "") {
		_currentLoadEpg = _currentRequest;
		qDebug() << "Request:" << _currentLoadEpg;
		_loader->getSchedule(_currentLoadEpg);
	} else {
		for(int i=0; i<_epgList.size(); i++) {
			if(!_day[0].contains(_epgList[i])) {
				_currentLoadEpg = _epgList[i];
				_loader->getSchedule(_epgList[i]);
				break;
			} else if(i == _epgList.size()-1) {
				_ready = true;
			}
		}
	}
}

void EpgManager::set(const QString &channel, const int &day, const QStringList &epg)
{
	_day[day].insert(channel, epg);
	qDebug() << channel << "day" << day << "gotten";

	if(day == 3) {
		if(_currentLoadEpg == _currentRequest) {
			_currentRequest = "";
			post(_currentLoadEpg);
		}
		load();
	}
}

void EpgManager::post(const QString &e)
{
	_currentEpg = e;
	now();
	emit epg(_day[0][e], 1);
	emit epg(_day[1][e], 2);
	emit epg(_day[2][e], 3);
	emit epg(_day[3][e], 4);
}

void EpgManager::now()
{
	int k;
	QStringList now;
	for(int i = 4; i < _day[0][_currentEpg].size(); i+=3) {
		if(QTime::fromString(_day[0][_currentEpg][i-3], "hh:mm") >=
		   QTime::fromString(_day[0][_currentEpg][i], "hh:mm")) {
			k = i;
			break;
		} else if(QTime::currentTime() > QTime::fromString(_day[0][_currentEpg][i-3], "hh:mm") &&
				  QTime::currentTime() < QTime::fromString(_day[0][_currentEpg][i], "hh:mm")) {
			k = i;
			break;
		}
	}

	now << "<a href=\"" + _day[0][_currentEpg][k-2] + "\">" + _day[0][_currentEpg][k-3] + " - " + _day[0][_currentEpg][k-1] + "</a>"
		<< "<a href=\"" + _day[0][_currentEpg][k+1] + "\">" + _day[0][_currentEpg][k] + " - " + _day[0][_currentEpg][k+2] + "</a>";
	emit epg(now, 0);

	_timer->start(60000);
}

void EpgManager::stop()
{
	_timer->stop();
}
