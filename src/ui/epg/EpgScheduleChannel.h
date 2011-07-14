/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2011 Tadej Novak <tadej@tano.si>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*****************************************************************************/

#ifndef TANO_EPGSCHEDULECHANNEL_H_
#define TANO_EPGSCHEDULECHANNEL_H_

#include <QtCore/QDate>
#include <QtCore/QModelIndex>
#include <QtGui/QStackedWidget>

#include "core/Enums.h"

namespace Ui
{
	class EpgScheduleChannel;
}

class XmltvProgramme;
class XmltvProgrammeFilterModel;
class XmltvProgrammeModel;

class EpgScheduleChannel : public QStackedWidget
{
Q_OBJECT
public:
	EpgScheduleChannel(QWidget *parent = 0);
	~EpgScheduleChannel();

public slots:
    void setEpg(XmltvProgrammeModel *epg,
				const Tano::Id &id);
	void setIdentifier(const Tano::Id &identifier) { _id = identifier; }
	void setPage(const int &id);

protected:
	void changeEvent(QEvent *e);

signals:
	void itemClicked(XmltvProgramme *);

private slots:
    void programmeClicked(const QModelIndex &index);
    void processFilters();

private:
	Ui::EpgScheduleChannel *ui;

	Tano::Id _id;
    XmltvProgrammeModel *_model;
    XmltvProgrammeFilterModel *_filterModel;
};

#endif // EPGSCHEDULECHANNEL_H_
