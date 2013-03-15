/****************************************************************************
* Tano - An Open IP TV Player
* Copyright (C) 2012 Tadej Novak <tadej@tano.si>
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

#include <QtGui/QFont>
#include <QtGui/QIcon>

#include "xmltv/XmltvCommon.h"
#include "xmltv/models/XmltvProgrammeFilterModel.h"
#include "xmltv/models/XmltvProgrammeModel.h"

XmltvProgrammeFilterModel::XmltvProgrammeFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent),
      _id(Tano::Main),
      _model(0),
      _date(QDate::currentDate()) { }

XmltvProgrammeFilterModel::~XmltvProgrammeFilterModel() { }

void XmltvProgrammeFilterModel::setDate(const QDate &date)
{
    _date = date;
    invalidateFilter();
}

bool XmltvProgrammeFilterModel::filterAcceptsRow(int sourceRow,
                                                 const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    bool name = _model->value(sourceRow, 1).toString().contains(filterRegExp());
    bool start = (QDateTime::fromString(_model->value(sourceRow, 2).toString(), Tano::Xmltv::dateFormat()).date() == _date);

    return (name && start);
}

void XmltvProgrammeFilterModel::setProgrammeModel(XmltvProgrammeModel *model,
                                                  const Tano::Id &id)
{
    if (id == _id) {
        _model = model;
        setSourceModel(_model);
        invalidateFilter();
    }
}
