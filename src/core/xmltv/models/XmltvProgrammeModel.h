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

#ifndef TANO_XMLTVPROGRAMMEMODEL_H_
#define TANO_XMLTVPROGRAMMEMODEL_H_

#include <QtSql/QSqlQueryModel>

#include "CoreSharedExport.h"

class XmltvSql;

/*!
    \class XmltvProgrammeModel XmltvProgrammeModel.h core/xmltv/models/XmltvProgrammeModel.h
    \brief XMLTV Programme SQL query model

    Reimplemented QSqlQueryModel to be able to display programmes in a QListView
*/
class TANO_CORE_EXPORT XmltvProgrammeModel : public QSqlQueryModel
{
Q_OBJECT
public:
    /*!
        \brief Model constructor

        This sets channel ID and database to read from.

        \param id channel ID (QString)
        \param db database (XmltvSql)
        \param parent parent object (QObject)
    */
    XmltvProgrammeModel(const QString &id,
                        XmltvSql *db,
                        QObject *parent = 0);
    ~XmltvProgrammeModel();

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
    QVariant value(const int &row,
                   const int &type) const;
};

#endif // TANO_XMLTVPROGRAMMEMODEL_H_
