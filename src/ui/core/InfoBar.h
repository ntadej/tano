/****************************************************************************
* InfoBar.h: InfoBar for Channel information display
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

#ifndef TANO_INFOBAR_H_
#define TANO_INFOBAR_H_

#include <QtCore/QTimer>
#include <QtGui/QLabel>
#include <QtGui/QScrollArea>

#include "core/GetFile.h"

class InfoBar : public QScrollArea
{
Q_OBJECT
public:
	InfoBar(QWidget *parent = 0);
	~InfoBar();

public slots:
	void clear();
	void setInfo(const QString &channel,
				 const QString &language);
	void setEpg(const QString &now,
				const QString &next);
	void setLogo(const QString &logo);

signals:
	void open(const QString);

private slots:
	void image(const QString &image);
	void scroll();

private:
	bool _direction;
	GetFile *_image;
	QTimer *_timer;

	QLabel *_labelChannel;
	QLabel *_labelLanguage;
	QLabel *_labelLogo;
	QLabel *_labelNext;
	QLabel *_labelNow;
};

#endif // TANO_INFOBAR_H_
