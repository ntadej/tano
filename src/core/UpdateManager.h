/****************************************************************************
* UpdateManager.h: Update manager
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

#ifndef TANO_UPDATEMANAGER_H_
#define TANO_UPDATEMANAGER_H_

#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtNetwork/QHttp>

#include "xml/UpdateHandler.h"

class UpdateManager : public QHttp
{
Q_OBJECT
public:
	UpdateManager(QObject *parent = 0);
	~UpdateManager();

public slots:
	void getUpdates();

signals:
	void updates(const QStringList);

private slots:
	void readUpdates();

private:
	QTextCodec *_codec;

	UpdateHandler *_handler;
};

#endif // TANO_UPDATEMANAGER_H_
