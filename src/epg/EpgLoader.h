/****************************************************************************
* EpgLoader.h: EPG loader and processer
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

#ifndef TANO_EPGLOADER_H_
#define TANO_EPGLOADER_H_

#include <QtCore/QString>
#include <QtCore/QTextCodec>
#include <QtCore/QTimer>
#include <QtNetwork/QHttp>

#include "../plugins/EpgPlugins.h"

class EpgLoader : public QHttp
{
Q_OBJECT
public:
	EpgLoader(QObject *parent = 0);
	~EpgLoader();

	void getEpg(const QString &arg, const bool &type = false);
	void loadPlugin(const QString &plugin);
	void stop();

signals:
	void epgDone(QStringList, int);

private slots:
	void epg();
	void schedule(const bool &error);
	void show(const bool &error);
	void init();
	void initDone(const bool &error);

private:
	bool _init;
	bool _show;
	int _step;

	EpgPlugin *_plugin;

	QString _currentArgument;
	QString _currentRequest;

	QTextCodec *_codec;
};

#endif // TANO_EPGLOADER_H_
