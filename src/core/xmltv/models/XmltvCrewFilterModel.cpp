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

#include "xmltv/models/XmltvCrewFilterModel.h"
#include "xmltv/models/XmltvCrewModel.h"

XmltvCrewFilterModel::XmltvCrewFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent),
      _model(0) { }

XmltvCrewFilterModel::~XmltvCrewFilterModel() { }

void XmltvCrewFilterModel::setType(const XmltvCrewMember::Type &type)
{
    _type = type;
    invalidateFilter();
}

bool XmltvCrewFilterModel::filterAcceptsRow(int sourceRow,
                                            const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    bool name = _model->value(sourceRow, 1).toString().contains(filterRegExp());
    bool type = (_model->value(sourceRow, 2).toInt() == _type) || !_type;

    return name && type;
}

void XmltvCrewFilterModel::setCrewModel(XmltvCrewModel *model)
{
    _model = model;
    setSourceModel(_model);
    invalidateFilter();
}
