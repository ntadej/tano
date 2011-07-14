/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_UPDATEMANAGER_H_
#define TANO_UPDATEMANAGER_H_

#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtNetwork/QHttp>

#include "container/core/UpdateInfo.h"

class UpdateHandler;

class UpdateManager : public QHttp
{
Q_OBJECT
public:
	UpdateManager(QObject *parent = 0);
	~UpdateManager();

public slots:
	void getUpdates();

signals:
	void updateInfo(const QStringList,
					const UpdateInfo);

private slots:
	void readUpdates();

private:
	QTextCodec *_codec;

	UpdateHandler *_handler;
};

#endif // TANO_UPDATEMANAGER_H_
