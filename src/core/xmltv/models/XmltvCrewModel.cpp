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

#include <QtGui/QIcon>
#include <QtSql/QSqlRecord>

#include "xmltv/XmltvSql.h"
#include "xmltv/containers/XmltvCrewMember.h"
#include "xmltv/models/XmltvCrewModel.h"

XmltvCrewModel::XmltvCrewModel(const QString &id,
                               XmltvSql *db,
                               QObject *parent)
    : QSqlQueryModel(parent)
{
    setQuery("SELECT `id`, `name`, `type` FROM `crew` WHERE `programme` = '" + id + "' ORDER BY `name`", db->database());
}

XmltvCrewModel::~XmltvCrewModel() { }

QVariant XmltvCrewModel::data(const QModelIndex &index,
                              int role) const
{
    QVariant v = QSqlQueryModel::data(index, role);
    if (v.isValid() && role == Qt::DisplayRole) {
        QString name = value(index.row(), 1).toString();
        XmltvCrewMember::Type type = XmltvCrewMember::Type(value(index.row(), 2).toInt());

        return QString("%1 (%2)").arg(name, XmltvCrewMember::stringFromType(type));
    } else if (role == Qt::DecorationRole) {
        return QIcon::fromTheme("config-users");
    }
    return QVariant();
}

QVariant XmltvCrewModel::value(const int &row,
                                    const int &type) const
{
    return QSqlQueryModel::data(QSqlQueryModel::index(row, type), Qt::DisplayRole);
}
