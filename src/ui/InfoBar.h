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
#include <QtGui/QWidget>

#include <ui_InfoBar.h>

class InfoBar : public QWidget
{
Q_OBJECT
public:
	InfoBar(QWidget *parent = 0);
	~InfoBar();

public slots:
	void clear();
	void setInfo(const QString &channel, const QString &language);
	void setEpg(const QString &now, const QString &next);

signals:
	void open(const QString);
	void refresh();

private slots:
	void scroll();

private:
	Ui::InfoBar ui;

	bool _direction;
	QTimer *_timer;
};

#endif // TANO_INFOBAR_H_
