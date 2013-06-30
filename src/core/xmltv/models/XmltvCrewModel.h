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

#ifndef TANO_XMLTVCREWMODEL_H_
#define TANO_XMLTVCREWMODEL_H_

#include <QtSql/QSqlQueryModel>

#include "CoreSharedExport.h"

class XmltvSql;

/*!
    \class XmltvCrewModel XmltvCrewModel.h core/xmltv/models/XmltvCrewModel.h
    \brief XMLTV Crew SQL query model

    Reimplemented QSqlQueryModel to be able to display crew members in a QListView
*/
class TANO_CORE_EXPORT XmltvCrewModel : public QSqlQueryModel
{
Q_OBJECT
public:
    /*!
        \brief Model constructor

        This sets programme ID and database to read from.

        \param id programme ID (QString)
        \param db database (XmltvSql)
        \param parent parent object (QObject)
    */
    XmltvCrewModel(const QString &id,
                   XmltvSql *db,
                   QObject *parent = 0);
    ~XmltvCrewModel();

    /*!
        \brief Reimplemented data access function
        \param item an index to item (QModelIndex)
        \param role data role (int)
        \return value of the requested data (QVariant)
    */
    QVariant data(const QModelIndex &item,
                  int role) const;

    /*!
        \brief Convenience function to read data
        \param row model row (int)
        \param type data type (int)
        \return value of the requested data (QVariant)
    */
    QVariant value(int row,
                   int type) const;
};

#endif // TANO_XMLTVCREWMODEL_H_
