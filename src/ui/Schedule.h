/****************************************************************************
* Schedule.h: Schedule widget
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

#ifndef TANO_SCHEDULE_H_
#define TANO_SCHEDULE_H_

#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

#include "epg/EpgManager.h"

namespace Ui
{
	class Schedule;
}

class Schedule : public QWidget
{
Q_OBJECT
public:
	Schedule(QWidget *parent = 0);
	~Schedule();

	void openPlaylist(const QString &p);
	void setEpg(const QStringList &epgList, const QString &epgPlugin) { _epg->setEpg(epgList, epgPlugin); };

protected:
	void changeEvent(QEvent *e);

signals:
	void urlClicked(QString);

private slots:
	void channel(QTreeWidgetItem *item);
	void loadEpg(const QStringList &list, const int &day);

private:
	Ui::Schedule *ui;

	EpgManager *_epg;
};

#endif // TANO_SCHEDULE_H_
