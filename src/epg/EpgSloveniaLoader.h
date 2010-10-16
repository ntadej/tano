/****************************************************************************
* EpgSloveniaLoader.h: EPG loader and processer for Slovenian providers
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

#ifndef TANO_EPGSLOVENIALOADER_H_
#define TANO_EPGSLOVENIALOADER_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>
#include <QtNetwork/QHttp>

#include "epg/EpgSlovenia.h"

class EpgSloveniaLoader : public QHttp
{
Q_OBJECT
public:
	EpgSloveniaLoader(QObject *parent = 0);
	~EpgSloveniaLoader();

	void getSchedule(const QString &arg,
					 const int &day = 0);
	void getShow(const QString &arg);
	void stop();

signals:
	void schedule(QString,
				  int,
				  QStringList);
	void show(QStringList);

private slots:
	void processSchedule(const int &req,
						 const bool &error);
	void processShow(const bool &error);
	void init();
	void initDone(const bool &error);

private:
	bool _init;

	EpgSlovenia *_slovenia;

	QString _currentArgument;
	QString _currentRequest;

	QMap<int, QString> _mapArg;
	QMap<int, int> _mapStep;

	QTextCodec *_codec;
};

#endif // TANO_EPGSLOVENIALOADER_H_
