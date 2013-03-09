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

#ifndef TANO_SQLDATABASEXMLTV_H_
#define TANO_SQLDATABASEXMLTV_H_

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class XmltvChannel;
class XmltvProgramme;

/*!
    \class XmltvSql XmltvSql.h core/xmltv/XmltvSql.h
    \brief XMLTV SQL database manager

    Connect read and write to XMLTV SQLite database
*/
class XmltvSql
{
public:
    XmltvSql();
    ~XmltvSql();

    /*!
        \brief Close connection to the database
    */
    void close();

    /*!
        \brief Get current database
        \return database (QSqlDatabase)
    */
    inline QSqlDatabase database() { return _db; }

    /*!
        \brief Open connection to the database and init empty tables
    */
    bool open();

    /*!
        \brief Get current database query
        \return query (QSqlQuery)
    */
    QSqlQuery query();


    /*!
        \brief Add channel to the database
        \param channel channel to add (XmltvChannel)
    */
    void addChannel(XmltvChannel *channel);

    /*!
        \brief Add programme to the database
        \param programme programme to add (XmltvProgramme)
    */
    void addProgramme(XmltvProgramme *programme);

    /*!
        \brief Read programme from the database
        \param id programme id (QString)
        \return programme (XmltvProgramme)
    */
    XmltvProgramme *programme(const QString &id);

private:
    QSqlDatabase _db;
};

#endif // TANO_SQLDATABASEXMLTV_H_
