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

#ifndef TANO_EPGSLOVENIALOADER_H_
#define TANO_EPGSLOVENIALOADER_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>
#include <QtNetwork/QHttp>

#include "container/EpgShowInfo.h"
#include "epg/EpgSlovenia.h"

class EpgSloveniaLoader : public QHttp
{
Q_OBJECT
public:
	EpgSloveniaLoader(QObject *parent = 0);
	~EpgSloveniaLoader();

	void getSchedule(const QString &arg,
					 const int &day = 0);
	void getShowInfo(const QString &arg);
	void stop();

signals:
	void schedule(QString,
				  int,
				  QStringList);
	void showInfo(EpgShowInfo);

private slots:
	void init();
	void initDone(const bool &error);
	void processSchedule(const int &req,
						 const bool &error);
	void processShowInfo(const bool &error);

private:
	QTextCodec *_codec;

	QString _currentArgument;
	QString _currentRequest;

	bool _init;

	QMap<int, QString> _mapArg;
	QMap<int, int> _mapStep;

	EpgSlovenia *_slovenia;
};

#endif // TANO_EPGSLOVENIALOADER_H_
