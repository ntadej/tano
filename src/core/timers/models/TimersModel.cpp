/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2013 Tadej Novak <tadej@tano.si>
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

#include "timers/TimersSql.h"
#include "timers/containers/Timer.h"

#include "TimersModel.h"

TimersModel::TimersModel(TimersSql *db,
                         QObject *parent)
    : QSqlQueryModel(parent)
{
    setQuery("SELECT * FROM `timers` ORDER BY `name`", db->database());
}

TimersModel::~TimersModel() { }

QVariant TimersModel::data(const QModelIndex &index,
                           int role) const
{
    QVariant v = QSqlQueryModel::data(index, role);
    if (v.isValid() && role == Qt::DisplayRole) {
        Timer::Type type = Timer::Type(value(index.row(), 8).toInt());
        Timer::State state = Timer::State(value(index.row(), 9).toInt());

        if (type != Timer::Once && type != Timer::Instant)
            return QString("%1 (%2) - %3 - %4 %5 %6, %7")
                    .arg(value(index.row(), 1).toString(), Timer::states()[state], value(index.row(), 2).toString(),
                         QDate::fromString(value(index.row(), 5).toString()).toString("dd.M.yyyy"), tr("at"), QTime::fromString(value(index.row(), 6).toString()).toString("hh:mm"),
                         Timer::typesLong()[type]);
        else
            return QString("%1 (%2) - %3 - %4 %5 %6")
                    .arg(value(index.row(), 1).toString(), Timer::states()[state], value(index.row(), 2).toString(),
                         QDate::fromString(value(index.row(), 5).toString()).toString("dd.M.yyyy"), tr("at"), QTime::fromString(value(index.row(), 6).toString()).toString("hh:mm"));
    } else if (role == Qt::DecorationRole) {
        Timer::State state = Timer::State(value(index.row(), 9).toInt());

        if (state == Timer::Finished)
            return QIcon::fromTheme("video-x-generic");
        else if (state == Timer::Recording)
            return QIcon::fromTheme("media-record");
        else
            return QIcon::fromTheme("time-admin");
    }
    return QVariant();
}

QVariant TimersModel::value(int row,
                                    int type) const
{
    return QSqlQueryModel::data(QSqlQueryModel::index(row, type), Qt::DisplayRole);
}
