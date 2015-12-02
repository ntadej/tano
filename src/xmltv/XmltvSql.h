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

#include <QtCore/QHash>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class XmltvChannel;
class XmltvCrewMember;
class XmltvProgramme;

/*!
    \class XmltvSql XmltvSql.h core/xmltv/XmltvSql.h
    \brief XMLTV SQL database manager

    Connect read and write to XMLTV SQLite database
*/
class XmltvSql
{
public:
    /*!
        \brief XmltvSql constructor
        Constructor with default values
    */
    XmltvSql();

    ~XmltvSql();

    /*!
        \brief Remove old entries
    */
    void clean();

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
        \brief Start transaction
    */
    bool startTransaction();

    /*!
        \brief End transaction
    */
    bool endTransaction();


    /*!
        \brief Add channel to the database
        \param channel channel to add (XmltvChannel)
    */
    void addChannel(XmltvChannel *channel);

    /*!
        \brief Update channel
        \param id channel id (QString)
        \param column column to change (QString)
        \param data new data (QString)
    */
    void updateChannel(const QString &id,
                       const QString &column,
                       const QString &data);

    /*!
        \brief Add programme to the database
        \param programme programme to add (XmltvProgramme)
    */
    void addProgramme(XmltvProgramme *programme);

    /*!
        \brief Update programme
        \param id programme id (QString)
        \param column column to change (QString)
        \param data new data (QString)
    */
    void updateProgramme(const QString &id,
                         const QString &column,
                         const QString &data);

    /*!
        \brief Add crew member to the database
        \param member crew member to add (XmltvCrewMember)
    */
    void addCrewMember(XmltvCrewMember *member);

    /*!
        \brief Read channel from the database
        \param id channel id (QString)
        \return channel (XmltvChannel)
    */
    XmltvChannel *channel(const QString &id);

    /*!
        \brief Channel mapping to IDs
        \return mapping (QHash<QString, QString>)
    */
    QHash<QString, QString> channels();

    /*!
        \brief Read crew from the database for the desired programme
        \param programme programme id (QString)
        \return crew list (QList<XmltvCrewMember *>)
    */
    QList<XmltvCrewMember *> crew(const QString &programme);

    /*!
        \brief Read programme from the database
        \param id programme id (QString)
        \return programme (XmltvProgramme)
    */
    XmltvProgramme *programme(const QString &id);

    /*!
        \brief Read programmes from the database for the desired channel
        \param channel channel id (QString)
        \return programmes list (QList<XmltvProgramme *>)
    */
    QList<XmltvProgramme *> programmes(const QString &channel);

    /*!
        \brief Read current and next programme id for channel
        \param id channel id (QString)
        \return programme (XmltvProgramme *)
    */
    XmltvProgramme *programmeCurrent(const QString &id);

    /*!
        \brief Read current and next programme for channel to be displayed
        \param id channel id (QString)
        \return programme (QStringList)
    */
    QStringList programmeCurrentDisplay(const QString &id);

    /*!
        \brief Read next programme from the database
        \param id programme id (QString)
        \param channel channel id (QString)
        \return programme (XmltvProgramme)
    */
    XmltvProgramme *programmeNext(const QString &id,
                                  const QString &channel);

    /*!
        \brief Read previous programme from the database
        \param id programme id (QString)
        \param channel channel id (QString)
        \return programme (XmltvProgramme)
    */
    XmltvProgramme *programmePrevious(const QString &id,
                                      const QString &channel);

private:
    QSqlDatabase _db;
};

#endif // TANO_SQLDATABASEXMLTV_H_
