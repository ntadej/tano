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

#ifndef TANO_XMLTVCREWMEMBER_H_
#define TANO_XMLTVCREWMEMBER_H_

#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QString>

#include "ListItem.h"

class XmltvProgramme;

class XmltvCrewMember : public ListItem
{
Q_OBJECT
public:
    enum Roles {
        DisplayRole = Qt::DisplayRole,
        DecorationRole = Qt::DecorationRole,
        NameRole = Qt::UserRole + 1,
        TypeRole,
        ProgrammeRole,
        StartRole
    };

    // Crew member types
    enum Type
    {
        All,
        Director,
        Actor,
        Writer,
        Adapter,
        Producer,
        Composer,
        Editor,
        Presenter,
        Commentator,
        Guest
    };

    explicit XmltvCrewMember(QObject *parent = 0);
    explicit XmltvCrewMember(const QString &name,
                             const Type &type,
                             const QString &programme,
                             const QDateTime &start,
                             QObject *parent = 0);
    explicit XmltvCrewMember(const QString &name,
                             const Type &type,
                             XmltvProgramme *programme);
    ~XmltvCrewMember();

    inline QString id() const { return _programme + "_" + _name; }
    QVariant data(int role) const;
    QString display() const;
    QIcon decoration() const;
    QHash<int, QByteArray> roleNames() const;

    inline QString name() const { return _name; }
    inline Type type() const { return _type; }
    inline QString programme() const { return _programme; }
    inline QDateTime start() const { return _start; }

    static Type typeFromString(const QString &type);
    static QString stringFromType(const Type &type);
    static QString stringFromTypeOutput(const Type &type);

private:
    QString _name;
    XmltvCrewMember::Type _type;
    QString _programme;
    QDateTime _start;
};

#endif // TANO_XMLTVCREWMEMBER_H_
