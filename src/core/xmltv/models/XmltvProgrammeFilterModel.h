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

#ifndef TANO_XMLTVPROGRAMMEFILTERMODEL_H_
#define TANO_XMLTVPROGRAMMEFILTERMODEL_H_

#include <QtCore/QDate>

#if defined(Qt5)
    #include <QtCore/QSortFilterProxyModel>
#elif defined(Qt4)
    #include <QtGui/QSortFilterProxyModel>
#endif

#include "CoreSharedExport.h"

#include "Common.h"

class XmltvProgrammeModel;

class TANO_CORE_EXPORT XmltvProgrammeFilterModel : public QSortFilterProxyModel
{
Q_OBJECT
public:
    XmltvProgrammeFilterModel(QObject *parent = 0);
    ~XmltvProgrammeFilterModel();

    inline void setId(const Tano::Id &id) { _id = id; }

    inline QDate date() const { return _date; }
    void setDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

public slots:
    void setProgrammeModel(XmltvProgrammeModel *model,
                           const Tano::Id &id);

private:
    Tano::Id _id;

    QDate _date;
};

#endif // TANO_XMLTVPROGRAMMEFILTERMODEL_H_
