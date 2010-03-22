/****************************************************************************
* Updates.h: Update manager
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

#ifndef TANO_UPDATES_H_
#define TANO_UPDATES_H_

#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtNetwork/QHttp>

class Updates : public QHttp
{
Q_OBJECT
public:
	Updates();
	~Updates();

public slots:
	void getUpdates();

signals:
	void updatesDone(QStringList);

private slots:
	void processUpdates(const QString &v);
	void updatesPrint();

private:
	QTextCodec *_codec;
};

#endif // TANO_UPDATES_H_
