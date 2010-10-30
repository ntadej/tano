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

#include <QtCore/QDate>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "container/EpgDayList.h"
#include "container/EpgItem.h"
#include "epg/EpgManager.h"

EpgManager::EpgManager(QObject *parent)
	: QObject(parent),
	_ready(false),
	_currentEpg(""),
	_currentLoadEpg(""),
	_currentRequest("")
{
	_slovenia = new EpgSloveniaLoader(this);
	_timer = new QTimer(this);
	connect(_timer, SIGNAL(timeout()), this, SLOT(now()));
}

EpgManager::~EpgManager()
{
	delete _slovenia;
	delete _timer;
}

void EpgManager::setEpg(const QStringList &epg,
						const Tano::EpgType &epgType)
{
	_epgList = epg;
	_epgType = epgType;
	if(_epgType == Tano::Slovenia)
		connect(_slovenia, SIGNAL(schedule(QString, int, QStringList)), this, SLOT(set(QString, int, QStringList)));

	load();
}

void EpgManager::clear()
{
	_day[0].clear();
	_day[1].clear();
	_day[2].clear();
	_day[3].clear();
}

void EpgManager::request(const QString &epg,
						 const QString &identifier)
{
	_currentIdentifier = identifier;

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
		_slovenia->getSchedule(_currentLoadEpg);
	} else {
		for(int i=0; i<_epgList.size(); i++) {
			if(!_day[0].contains(_epgList[i])) {
				_currentLoadEpg = _epgList[i];
				_slovenia->getSchedule(_epgList[i]);
				break;
			} else if(i == _epgList.size()-1) {
				_ready = true;
			}
		}
	}
}

void EpgManager::set(const EpgDayList &list)
{
	_day[list.day()].insert(list.channel(), list);

	if(list.day() == 3) {
		qDebug() << list.channel() << "loaded";
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

	if(_currentIdentifier == "main") {
		_currentEpgNow = e;
		current();
	}
	emit epgSchedule(_day[0][e], _currentIdentifier);
	emit epgSchedule(_day[1][e], _currentIdentifier);
	emit epgSchedule(_day[2][e], _currentIdentifier);
	emit epgSchedule(_day[3][e], _currentIdentifier);
}

void EpgManager::current()
{
	int k;
	QStringList now;
	for(int i = 0; i < _day[0][_currentEpgNow].size(); i++) {
		if(_day[0][_currentEpgNow][i-1]->time() >= _day[0][_currentEpgNow][i]->time()) {
			k = i;
			break;
		} else if(QTime::currentTime() > _day[0][_currentEpgNow][i-1]->time() &&
				  QTime::currentTime() < _day[0][_currentEpgNow][i]->time()) {
			k = i;
			break;
		}
	}

	now << "<a href=\"" + _day[0][_currentEpgNow][k-1]->url() + "\">" + _day[0][_currentEpgNow][k-1]->time().toString("hh:mm") + " - " + _day[0][_currentEpgNow][k-1]->title() + "</a>"
		<< "<a href=\"" + _day[0][_currentEpgNow][k]->url() + "\">" + _day[0][_currentEpgNow][k]->time().toString("hh:mm") + " - " + _day[0][_currentEpgNow][k]->title() + "</a>";
	emit epgCurrent(now, "main");

	_timer->start(60000);
}

void EpgManager::stop()
{
	_timer->stop();
}
