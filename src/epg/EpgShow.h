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

#include <QtGui/QStackedWidget>

#include "core/GetFile.h"
#include "epg/EpgLoader.h"

namespace Ui
{
	class EpgShow;
}

class EpgShow : public QStackedWidget
{
Q_OBJECT
public:
	EpgShow(QWidget *parent = 0);
	~EpgShow();

protected:
	void changeEvent(QEvent *e);

public slots:
	void open(const QString &url);
	void loadPlugin(const QString &plugin);

private slots:
	void display(const QStringList &list);
	void image(const QString &image);
	void next();
	void previous();

private:
	QString processUrl(const QString &url) const;

	Ui::EpgShow *ui;

	GetFile *_image;
	EpgLoader *_loader;

	QString _epgNext;
	QString _epgPrevious;
};

#endif // TANO_EPGSHOW_H_
