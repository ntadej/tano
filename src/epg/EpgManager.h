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

#ifndef TANO_EPGMANAGER_H_
#define TANO_EPGMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QTimer>

#include "core/Common.h"
#include "epg/EpgSloveniaLoader.h"

class EpgManager : public QObject
{
Q_OBJECT
public:
	EpgManager(QObject *parent = 0);
	~EpgManager();

	void setEpg(const QStringList &epg,
				const Tano::EpgType &epgType);
	void stop();

public slots:
	void request(const QString &epg,
				 const QString &identifier);

signals:
	void epg(QStringList,
			 int,
			 QString);

private slots:
	void now();
	void set(const QString &channel,
			 const int &day,
			 const QStringList &epg);

private:
	void clear();
	void load();
	void post(const QString &e);

	bool _ready;

	QString _currentEpg;
	QString _currentEpgNow;
	QString _currentIdentifier;
	QString _currentLoadEpg;
	QString _currentRequest;
	QStringList _epgList;
	Tano::EpgType _epgType;
	QString _path;

	QMap<QString, QStringList> _day[4];

	EpgSloveniaLoader *_loader;
	QTimer *_timer;
};

#endif // TANO_EPGMANAGER_H_
