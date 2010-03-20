/****************************************************************************
* EpgShedule.h: EPG schedule GUI class
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

#ifndef TANO_EPGSCHEDULE_H_
#define TANO_EPGSCHEDULE_H_

#include <QWidget>

#include <ui_EpgSchedule.h>

class EpgSchedule : public QStackedWidget
{
Q_OBJECT
public:
	EpgSchedule(QWidget *parent = 0);
	~EpgSchedule();

	void clear();
	void setEpg(const QStringList &epgValue, const int &id);
	void setPage(const int &id);

signals:
	void urlClicked(const QString);

private:
	Ui::EpgSchedule ui;
};

#endif // EPGSCHEDULE_H_
