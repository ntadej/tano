/****************************************************************************
* EpgShow.h: EPG show information
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

#ifndef TANO_EPGSHOW_H_
#define TANO_EPGSHOW_H_

#include <QtCore/QFile>
#include <QtGui/QWidget>

#include <ui_EpgShow.h>

#include "EpgLoader.h"

class EpgShow : public QWidget
{
Q_OBJECT
public:
	EpgShow(QWidget *parent = 0);
	~EpgShow();

public slots:
	void open(const QString &url);
	void loadPlugin(const QString &plugin);

private slots:
	void display(const QStringList &list);
	void downloadFile(const QString &u);
	void httpRequestFinished(const int &requestId, const bool &error);

private:
	Ui::EpgShow ui;

	EpgLoader *_loader;

	QHttp *_http;
	QFile *_file;
	int _httpGetId;
};

#endif // TANO_EPGSHOW_H_
