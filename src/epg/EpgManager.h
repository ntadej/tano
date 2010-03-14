/****************************************************************************
* EpgManager.h: EPG manager
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

#ifndef TANO_EPGMANAGER_H_
#define TANO_EPGMANAGER_H_

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>

#include "EpgLoader.h"

class EpgManager : public QObject
{
Q_OBJECT
public:
	EpgManager(QObject *parent = 0);
	~EpgManager();

	void setEpg(const QStringList &epg, const QString &epgPlugin);
	void request(const QString &epg);

signals:
	void epg(QStringList, int);

private:
	void clear();
	void init();

	bool _reload;

	QStringList _epgList;
	QString _epgPlugin;
	QString _path;

	QMap<QString, QStringList> _day[4];

	EpgLoader *_loader;
};

#endif // TANO_EPGMANAGER_H_
