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

#include <QtCore/QDateTime>
#include <QtGui/QFont>
#include <QtGui/QIcon>
#include <QtSql/QSqlRecord>

#include "xmltv/XmltvCommon.h"
#include "xmltv/XmltvSql.h"
#include "xmltv/models/XmltvProgrammeModel.h"

XmltvProgrammeModel::XmltvProgrammeModel(const QString &id,
                                         XmltvSql *db,
                                         QObject *parent)
    : QSqlQueryModel(parent)
{
    setQuery("SELECT `id`, `title`, `start`, `stop` FROM `programmes` WHERE `channel` = '" + id + "' ORDER BY `start`", db->database());
}

XmltvProgrammeModel::~XmltvProgrammeModel() { }

QVariant XmltvProgrammeModel::data(const QModelIndex &index,
                                   int role) const
{
    QVariant v = QSqlQueryModel::data(index, role);
    if (v.isValid() && role == Qt::DisplayRole) {
        QDateTime start = QDateTime::fromString(value(index.row(), 2).toString(), Tano::Xmltv::dateFormat());
        QString title = value(index.row(), 1).toString();

        return QString("%1 - %2").arg(start.toString(Tano::Xmltv::timeFormatDisplay()), title);
    } else if (role == Qt::DecorationRole) {
        return QIcon::fromTheme("x-office-calendar").pixmap(16);
    } else if (role == Qt::FontRole) {
        QDateTime start = QDateTime::fromString(value(index.row(), 2).toString(), Tano::Xmltv::dateFormat());
        QDateTime stop = QDateTime::fromString(value(index.row(), 3).toString(), Tano::Xmltv::dateFormat());

        QFont font;
        if (QDateTime::currentDateTime() > start && QDateTime::currentDateTime() < stop)
            font.setBold(true);
        return font;
    }
    return QVariant();
}

QVariant XmltvProgrammeModel::value(int row,
                                    int type) const
{
    return QSqlQueryModel::data(QSqlQueryModel::index(row, type), Qt::DisplayRole);
}
