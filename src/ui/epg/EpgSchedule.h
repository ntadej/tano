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

#ifndef TANO_EPGSCHEDULE_H_
#define TANO_EPGSCHEDULE_H_

#include <QtGui/QStackedWidget>

#include "core/Common.h"

namespace Ui
{
	class EpgSchedule;
}

class EpgDayList;

class EpgSchedule : public QStackedWidget
{
Q_OBJECT
public:
	EpgSchedule(QWidget *parent = 0);
	~EpgSchedule();

	void clear();

public slots:
	void setEpg(const EpgDayList &epg,
				const Tano::Id &id);
	void setIdentifier(const Tano::Id &identifier) { _id = identifier; }
	void setPage(const int &id);

protected:
	void changeEvent(QEvent *e);

signals:
	void urlClicked(const QString);

private:
	Ui::EpgSchedule *ui;

	Tano::Id _id;
};

#endif // EPGSCHEDULE_H_
