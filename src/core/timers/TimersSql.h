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

#ifndef TANO_TIMERSSQL_H_
#define TANO_TIMERSSQL_H_

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include "core/timers/containers/Timer.h"

/*!
    \class TimersSql TimersSql.h core/timers/TimersSql.h
    \brief Timers SQL database manager

    Connect, read and write to Timers SQLite database
*/
class TimersSql
{
public:
    TimersSql();
    ~TimersSql();

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
        \brief Add timer to the database
        \param timer timer to add (Timer *)
        \return timer id (int)
    */
    int addTimer(Timer *timer);

    /*!
        \brief Create new timer and add it to the database
        \param name timer name (QString)
        \param channel channel name (QString)
        \param url channel url (QString)
        \param type timer type (Timer::Type)
        \return timer (Timer *)
    */
    Timer *createTimer(const QString &name,
                       const QString &channel,
                       const QString &url,
                       const Timer::Type &type = Timer::Once);

    /*!
        \brief Update timer
        \param timer timer to update (Timer *)
    */
    void updateTimer(Timer *timer);

    /*!
        \brief Delete timer
        \param timer timer to delete (Timer *)
    */
    void deleteTimer(Timer *timer);

    /*!
        \brief Read timer from the database
        \param id timer id (QString)
        \return timer (Timer *)
    */
    Timer *timer(const QString &id);

    /*!
        \brief Read timer first timer to record from the database
        \return timer that may need to be recorded (Timer *)
    */
    Timer *timerToRecord();

private:
    void readTimersOld();

    QSqlDatabase _db;
};

#endif // TANO_TIMERSSQL_H_
