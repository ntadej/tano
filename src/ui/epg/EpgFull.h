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

#ifndef TANO_EPGFULL_H_
#define TANO_EPGFULL_H_

#include <QtGui/QTreeWidgetItem>
#include <QtGui/QWidget>

#include "core/Common.h"

namespace Ui
{
	class EpgFull;
}

class EpgDayList;

class EpgFull : public QWidget
{
Q_OBJECT
public:
	EpgFull(QWidget *parent = 0);
	~EpgFull();

	void openPlaylist(const QString &p);

protected:
	void changeEvent(QEvent *e);

signals:
	void requestEpg(QString, Tano::Id);
	void urlClicked(QString);

private slots:
	void channel(QTreeWidgetItem *item);
	void setEpg(const EpgDayList &list,
				const Tano::Id &identifier);

private:
	Ui::EpgFull *ui;
};

#endif // TANO_EPGFULL_H_
